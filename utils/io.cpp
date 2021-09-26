#include "io.h"

std::vector<unsigned char> read_file(std::ifstream& in) {
    std::vector<unsigned char> res((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    return res;
}

void write_result(std::ofstream& out, const std::vector<unsigned char>& result) {
    out.write(reinterpret_cast<const char*>(&result[0]), result.size()*sizeof(unsigned char));
}