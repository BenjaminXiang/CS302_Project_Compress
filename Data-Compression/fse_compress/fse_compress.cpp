#include<bits/stdc++.h>
#include "fse_alg.hpp"

using namespace std;

void write(int c, int bits, vector<unsigned char> &result) {
    // bits can be 8, 16 or 32. 
    for (int i = 0; i < bits/8; i++) {
        result.push_back(c%256);
        c /= 256;
    }
}


bool fse_compress(unsigned char *data, int data_size, vector<unsigned char> &result) {
    int nSymbols = 256;

    int byte_offset, state;
    vector<unsigned char> encoded_stream;
    int freqs[nSymbols];
    FSE fse;

    bool success = fse.Compress(data, data_size, nSymbols, encoded_stream, freqs, byte_offset, state);
    assert(success);
    int encoded_size = (int)encoded_stream.size();
    
    for (int i = 0; i < nSymbols; i++) {
        write(freqs[i], 16, result);
    }
    write(byte_offset, 32, result);
    write(state, 32, result);
    write(encoded_size, 32, result);
    write(data_size, 32, result);

    for (int i = 0; i < encoded_size; i++) {
        result.push_back(encoded_stream[i]);
    }
}

int kPageSize = 10000000;
int main(int argc, char* argv[]) {
    if (argc!=3) {
        std::cerr << "Usage: fse_compress <input_file_path> <output_file_path>" << std::endl;
        exit(1);
    }

    std::ifstream in(argv[1], std::ios_base::binary);
    std::ofstream out(argv[2], std::ios_base::binary);
    if (in.fail()) {
        std::cerr << "Error: Open input file Fail!" << std::endl;
        exit(1);
    }
    if (out.fail()) {
        std::cerr << "Error: Open output file Fail!" << std::endl;
        exit(1);
    }

    std::vector<unsigned char> file_data((std::istreambuf_iterator<char>(in)),
                                        std::istreambuf_iterator<char>());

    std::cout << "file size: " << file_data.size() << ", kPage size: "<< kPageSize << std::endl;

    double start_time = clock();
    for (int i=0; i<file_data.size(); i+=kPageSize) {
        std::vector<unsigned char> result;

        int input_length;
        if (file_data.size()-i>kPageSize) {
            input_length = kPageSize;
        } else {
            input_length = file_data.size()-i;
        }
        fse_compress(&file_data[i], input_length, result);
        
        out.write(reinterpret_cast<const char *>(&result[0]), result.size()* sizeof(unsigned char));

    }
    double end_time = clock();
    printf("compress time: %.3f s\n",(end_time-start_time)/1000000);  
    in.close();
    out.close();

    return 0;
}