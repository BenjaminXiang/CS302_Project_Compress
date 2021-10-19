#include<bits/stdc++.h>
#include "fse_alg.hpp"

using namespace std;

int read(int bits, vector<unsigned char> &data, int &pos) {
    int bias = 1;
    int num = 0;
    for (int i = 0; i < bits/8; i++) {
        num += bias * int(data[pos]);
        bias *= 256;
        pos += 1;
    }
    return num;
}

int main(int argc, char* argv[]) {
    if (argc!=3) {
        std::cerr << "Usage: fse_decompress <input_file_path> <output_file_path>" << std::endl;
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

    std::vector<unsigned char> compress_file((std::istreambuf_iterator<char>(in)),
                                             std::istreambuf_iterator<char>());

    int pos = 0;
    
    double start_time = clock();
    while (pos<compress_file.size()) {
        int nSymbols = 256;
        int freqs[nSymbols];
        FSE fse;

        for (int i = 0; i < nSymbols; i++) {
            freqs[i] = read(16, compress_file, pos);
        }
        int byte_offset = read(32, compress_file, pos);
        int state = read(32, compress_file, pos);
        int encoded_size = read(32, compress_file, pos);
        int data_size = read(32, compress_file, pos);
        std::vector<unsigned char> raw_data(data_size);
        fse.Decompress(&compress_file[pos], encoded_size, freqs, raw_data, byte_offset, state, nSymbols);
        
        out.write(reinterpret_cast<const char *>(&raw_data[0]), raw_data.size()*sizeof(unsigned char));
        pos += encoded_size;
    }
    double end_time = clock();
    printf("decompress time: %.3f s\n",(end_time-start_time)/1000000);  

    return 0;
}
