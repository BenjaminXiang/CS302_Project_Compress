#ifndef SECOND_COMPRESS_IO_UTILS_H
#define SECOND_COMPRESS_IO_UTILS_H

#include <iostream>
#include <vector>
#include "bit_map.h"

void lz_code_to_file(std::ofstream& out, BitMap& bit_map, const std::vector<unsigned char>& result);

template<typename T>
T array_to_integer(const unsigned char* array, const int kLength) {
    T value = 0;
    for (int i=0; i<kLength; ++i) {
        value += static_cast<T>(array[i])<<(i*8);
    }
    return value;
}

#endif //SECOND_COMPRESS_IO_UTILS_H
