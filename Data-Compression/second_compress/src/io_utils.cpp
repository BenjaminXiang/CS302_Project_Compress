#include "../include/io_utils.h"

void lz_code_to_file(std::ofstream& out, BitMap& bit_map, const std::vector<unsigned char>& result) {
    int header = bit_map.capacity();
    out.write(reinterpret_cast<const char *>(&header), sizeof header);
    bit_map.write_to_file(out);
    out.write(reinterpret_cast<const char *>(&result[0]), result.size()*sizeof(unsigned char));
}