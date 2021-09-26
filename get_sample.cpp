#include <iostream>
#include <vector>
#include "utils/io.h"

const int kFrameSize = 1088;

int main(int argc, char* argv[]) {
    std::ifstream  in(argv[1], std::ios_base::binary);
    std::ofstream out(argv[2], std::ios_base::binary);
    int frame_number = atoi(argv[3]);
    int sample_size = frame_number * kFrameSize;

    if (in.fail()) {
        std::cerr << "Error: Open input file Fail!" << std::endl;
        exit(1);
    }
    if (out.fail()) {
        std::cerr << "Error: Open output file Fail!" << std::endl;
        exit(1);
    }

    std::vector<unsigned char> file_data = read_file(in);
    std::vector<unsigned char> sample_data(file_data.begin(), file_data.begin()+sample_size);

    write_result(out, sample_data);

    return 0;
}



