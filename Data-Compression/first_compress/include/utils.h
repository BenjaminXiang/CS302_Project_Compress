#ifndef DATA_COMPRESSION_UTILS_H
#define DATA_COMPRESSION_UTILS_H

#include <vector>

template<typename T>
std::vector<unsigned char> integer_to_array(T value) {
    std::vector<unsigned char> array;
    array.reserve(sizeof(value));
//    auto p = reinterpret_cast<const char*>(&value);
    for (int i=0; i<sizeof(value); ++i) {
        array.push_back( (value >> (i*8)) & 0xFF);
//        array.push_back(reinterpret_cast<char>(*(p+i)));
    }
    return array;
}

template<typename T>
T array_to_integer(const unsigned char* array, const int length) {
    T value = 0;
    for (int i=0; i<length; ++i) {
        value += static_cast<T>(array[i])<<(i*8);
    }
    return value;
}

template<typename T>
void push_integer_to_vector(T value, std::vector<unsigned char> &vec) {
    for (int i=0; i<sizeof(T); ++i) {
        vec.push_back( (value >> (i*8)) & 0xFF );
    }
}

#endif //DATA_COMPRESSION_UTILS_H
