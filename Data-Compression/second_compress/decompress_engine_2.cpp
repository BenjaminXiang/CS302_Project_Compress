#include <iostream>
#include <vector>
#include "include/io_utils.h"
#include "include/lz_alg.h"

int main(int argc, char* argv[]) {
    if (argc!=3) {
        std::cerr << "Usage: decompress_engine_2 <input_file_path> <output_file_path>" << std::endl;
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

    size_t pos = 0;
    double start_time = clock();
    while (pos<compress_file.size()) {
        int bitmap_size = array_to_integer<int>(&compress_file[pos], 4);
        int code_size   = array_to_integer<int>(&compress_file[pos+4], 4);

        std::vector<unsigned char> raw_data;
        size_t length = 8+bitmap_size+code_size;
        lz_decompress(&compress_file[pos], length, raw_data);

        out.write(reinterpret_cast<const char *>(&raw_data[0]), raw_data.size()*sizeof(unsigned char));
        // std::cout<<raw_data.size()<<std::endl;
        pos += length;
    }
    double end_time = clock();
    printf("decompress time: %.3f s\n",(end_time-start_time)/1000000);  

    return 0;
}