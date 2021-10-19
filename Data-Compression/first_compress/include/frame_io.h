#ifndef DATA_COMPRESSION_FRAME_IO_H
#define DATA_COMPRESSION_FRAME_IO_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstring>

const int kHexPerLine = 64;
const int kBytePerLine = 32;
const int kBytePerFrame = 1024;
const int kRowPerFrame = 32;

const std::string kHeaderCode = "00000000 01234567";
const std::vector<unsigned char> kIDCode = {103, 69, 35, 1, 0, 0, 0, 0};

static inline char hex_char_to_int(unsigned char ch) {
    if (ch >= '0' && ch <= '9') {
        return (unsigned char)(ch - '0');
    } else if (ch >= 'a' && ch <= 'f') {
        return (unsigned char)(ch - ('a' - 10));
    }
}

template<typename T>
T bytes_to_integer(const unsigned char* str, int length) {
    T number = 0;
    for (int i=0; i<length; ++i) {
//        number += str[i]*pow(16, length-i-1);
//        T tmp_val = str[i];
//        std::cout << "str: " << (int)str[i] << ", i: " << i << std::endl;
        number += static_cast<T>(str[i])*static_cast<T>(pow(256, i));
//        std::cout << "number: " << number << std::endl;
    }
    return number;
}

template<typename T>
T hexs_to_integer(const unsigned char* str, int length) {
    T number = 0;
    for (int i=0; i<length; ++i) {
//        number += str[i]*pow(16, length-i-1);
        T tmp_val = str[i];
        std::cout << "str: " << (int)tmp_val << ", i: " << i << std::endl;
        number += tmp_val<<(i<<2);
    }
    return number;
}

void remove_blank(std::string& old_line , unsigned char* new_line);

void line_to_bytes(std::string& old_line, unsigned char* new_line);

bool read_header(std::ifstream& in, unsigned char& channel, unsigned char& model, long long& timestamp);

bool read_frame(std::ifstream& in, unsigned char* read_buff,
                unsigned char& channel, unsigned char& model, long long& timestamp);

void write_result(std::ofstream& out, const std::vector<unsigned char>& result);

std::vector<unsigned char> read_file(std::ifstream& in);

int get_header(const unsigned char* data, long long& timestamp, unsigned char& model, unsigned char& channel);

#endif //DATA_COMPRESSION_FRAME_IO_H
