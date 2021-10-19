#include "../include/frame_io.h"

void remove_blank(std::string& old_line , unsigned char* new_line) {
    for (int i=0, j=0; i < kHexPerLine; ++i) {
        new_line[i] = hex_char_to_int(old_line[i+j]);
        if (i % 8 == 7) {
            ++j;
        }
    }
}

void line_to_bytes(std::string& old_line, unsigned char* new_line) {
    unsigned char tmp_line[kHexPerLine];
    remove_blank(old_line, tmp_line);
    for (int i=0, j=0; i < kHexPerLine; ++i) {
        tmp_line[i] = hex_char_to_int(old_line[i+j]);
        if (i % 8 == 7) {
            ++j;
        }
    }
    for (int i=0; i < kBytePerLine; ++i) {
        new_line[i] = (tmp_line[i*2]<<4)+tmp_line[i*2+1];
    }
}

bool read_header(std::ifstream& in, unsigned char& channel, unsigned char& model, long long& timestamp) {
    std::string line;
    std::getline(in, line);
    if (in.eof()) {
        return false;
    }

    if (line.substr(0, 17) != kHeaderCode) {
        std::cerr << "Error: Read Error Dataframe Headerline" << std::endl;
        std::cerr << line << std::endl;
        exit(1);
    }

    unsigned char header_line[kHexPerLine];
    remove_blank(line, header_line);

    channel = hexs_to_integer<unsigned char>(&header_line[34], 2) - 1;
    model = log(hexs_to_integer<unsigned char>(&header_line[48], 4)) / log(2) - 4;
    timestamp = hexs_to_integer<long long>(&header_line[52], 12);
//    cout << "channel: " << channel << ", model: " << model << ", timestamp: " << timestamp << endl;

    return true;
}

bool read_frame(std::ifstream& in, unsigned char* read_buff,
                unsigned char& channel, unsigned char& model, long long& timestamp) {
    std::string line;
    if (read_header(in, channel, model, timestamp)) {
        for (int i=0; i < kRowPerFrame; ++i) {
            if (in.eof()) {
                return false;
            }
            std::getline(in, line);
            line_to_bytes(line, &read_buff[i * kBytePerLine]);
        }
        if (in.eof()) {
            return false;
        }
        std::getline(in, line);
        return true;
    } else {
        return false;
    }
}

void write_result(std::ofstream& out, const std::vector<unsigned char>& result) {
//    for (const auto& ch : result) {
//        out << ch;
//    }
    out.write(reinterpret_cast<const char*>(&result[0]), result.size()*sizeof(unsigned char));
}

std::vector<unsigned char> read_file(std::ifstream& in) {
     std::vector<unsigned char> tmp((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
     return tmp;
}

//unsigned char get_model(short value) {
//    std::cout << "model value: " << value << std::endl;
////    unsigned short tmp = value >> 4;
//    unsigned char result = 0;
////    while(tmp) {
////        tmp >>= 1;
////        ++result;
////    }
//    while(value) {
//        value >>= 1;
//        ++result;
//    }
//
//    std::cout << "model res: " << (int)result << std::endl;
//
//    return result-1;
//}

int get_header(const unsigned char* data, long long& timestamp, unsigned char& model, unsigned char& channel) {
    for (int i=0; i<8; ++i) {
        if (kIDCode[i] != data[i+24]) {
            return 0;
        }
    }
    timestamp = bytes_to_integer<long long>(data, 6);
//    std::cout << "in get_header timestamp: " << timestamp << std::endl;
    model = log(bytes_to_integer<short>(&data[6], 2))/log(2) - 4;
    channel = bytes_to_integer<unsigned char>(&data[14], sizeof(unsigned char)) - 1;

    return kBytePerLine;
}
