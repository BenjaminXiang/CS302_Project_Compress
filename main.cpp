#include <iostream>
#include <fstream>
#include <vector>
#include "utils/io.h"

const int kPageSize = 1024; //

int main(int argc, char* argv[]) {
    if (argc!=3) {
        std::cerr << "Usage: compress_engine <input_file_path> <output_file_path>" << std::endl;
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

    std::vector<unsigned char> result;
    double start_time = clock();
    for (size_t i=0; i<file_data.size(); i+=kPageSize) {
        std::vector<unsigned char> temp_result;

        size_t input_length;
        if (file_data.size()-i>kPageSize) {
            input_length = kPageSize;
        } else {
            input_length = file_data.size()-i;
        }

        compress(&file_data[i], input_length, temp_result);
        result.insert(result.end(), temp_result.begin(), temp_result.end());
    }
    double end_time = clock();
    printf("compress time: %.3f s\n",(end_time-start_time)/1000000);

    out.write(reinterpret_cast<const char *>(&result[0]), result.size()* sizeof(unsigned char));

    in.close();
    out.close();



    return 0;
}