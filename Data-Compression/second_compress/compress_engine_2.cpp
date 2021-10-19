#include <iostream>
#include <vector>
#include <bitset>
#include "include/io_utils.h"
#include "include/lz_alg.h"

int main(int argc, char* argv[]) {
    if (argc!=3) {
        std::cerr << "Usage: compress_engine_2 <input_file_path> <output_file_path>" << std::endl;
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

    std::cout << "file size: " << file_data.size() << std::endl;

    double start_time = clock();
    for (size_t i=0; i<file_data.size(); i+=kPageSize) {
        std::vector<unsigned char> result;

        size_t input_length;
        if (file_data.size()-i>kPageSize) {
            input_length = kPageSize;
        } else {
            input_length = file_data.size()-i;
        }

//        std::cout << "input_length: " << input_length << std::endl;

//        lz_compress(static_cast<const unsigned char *>(&file_data[i]), input_length, result);
        lz_compress(&file_data[i], input_length, result);

        out.write(reinterpret_cast<const char *>(&result[0]), result.size()* sizeof(unsigned char));

//        lz_compress(&file_data[i], input_length, tmp_result);
//        get_lz_code(tmp_result, input_length, bit_map, result);
//        lz_code_to_file(out, bit_map, result);
//        bit_map.Clear();
    }
    double end_time = clock();
    printf("compress time: %.3f s\n",(end_time-start_time)/1000000);
    
    in.close();
    out.close();

    return 0;
}