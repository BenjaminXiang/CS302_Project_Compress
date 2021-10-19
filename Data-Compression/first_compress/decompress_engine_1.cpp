#include <iostream>
#include <vector>
#include <string>
#include "include/frame_io.h"
#include "include/coding_1.h"

int main(int argc, char* argv[]) {
    if (argc!=3) {
        std::cerr << "Usage: decompress_engine_1 <input_file_path> <output_file_path>" << std::endl;
        exit(1);
    }

    std::ifstream  in(argv[1], std::ios_base::binary);
    std::ofstream out(argv[2], std::ios_base::binary);
    if (in.fail()) {
        std::cerr << "Error: Open input file Fail!" << std::endl;
        exit(1);
    }
    if (out.fail()) {
        std::cerr << "Error: Open output file Fail!" << std::endl;
        exit(1);
    }

    std::vector<unsigned char> file_data = read_file(in);

    std::cout << "file size: " << file_data.size() << std::endl;

    std::vector<unsigned char> raw_data;

    delta_decompress(file_data, raw_data);

    write_result(out, raw_data);

    return 0;
}