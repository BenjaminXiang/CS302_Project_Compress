#include <iostream>
#include <vector>
#include "utils/io.h"

int main(int argc, char* argv[]) {
    if (argc!=2) {
        std::cerr << "Usage: compress_engine_1 <input_file_path>" << std::endl;
        exit(1);
    }

    std::ifstream  in(argv[1], std::ios_base::binary);
    if (in.fail()) {
        std::cerr << "Error: Open input file Fail!" << std::endl;
        exit(1);
    }

    std::vector<unsigned char> file_data = read_file(in);

    for (int i=0; i<file_data.size(); ++i) {
        std::cout << (int)file_data[i];
        if (i%32==31) {
            std::cout << std::endl;
        } else {
            std::cout << ", ";
        }
    }

    return 0;
}

