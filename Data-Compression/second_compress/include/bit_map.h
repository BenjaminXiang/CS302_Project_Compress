#ifndef SECOND_COMPRESS_BIT_MAP_H
#define SECOND_COMPRESS_BIT_MAP_H
#include <vector>
#include <iostream>
#include <fstream>

class BitMap {
public:
    BitMap():bit_number_(0), visited_number_(0) {}
    BitMap(const unsigned char *kArr, const size_t kArrLength) {
        data_.reserve(kArrLength);
        for (int i=0; i<kArrLength; ++i) {
            data_.push_back(kArr[i]);
        }
        bit_number_ = kArrLength*8;
        visited_number_ = 0;
    }

    void push_back(int val) {
        ++bit_number_;
        if (bit_number_%8==1) {
            unsigned char tmp = 0;
            data_.push_back(tmp);
        }
        unsigned long long byte_number = (bit_number_ - 1) / 8;
        unsigned long long byte_bias   = (bit_number_ - 1) % 8;
        if (val) {
            data_[byte_number] += pow2[byte_bias];
        }
    }
    int get() {
        unsigned long long byte_number = visited_number_ / 8;
        unsigned long long byte_bias   = visited_number_ % 8;
        if (byte_number>data_.size()) {
            std::cerr << "byte_number: " << byte_number << ", data_.size(): " << data_.size() << std::endl;
            std::cerr << "BitMap out of range" << std::endl;
            exit(1);
        }
        if (visited_number_>=bit_number_) {
            std::cerr << "visited_number_>bit_number_" << std::endl;
            exit(1);
        }
        unsigned char tmp = data_[byte_number];
        int result = (tmp>>byte_bias)&0x01;
        ++visited_number_;
        return result;
    }
    auto size() const {
        return bit_number_;
    }
    auto capacity() const {
        return data_.size();
    }
    bool empty() const {
        if (bit_number_==0) {
            return true;
        } else {
            return false;
        }
    }
    bool at_end() const {
        if (visited_number_==bit_number_) {
            return true;
        } else {
            return false;
        }
    }
    void append_to_vector(std::vector<unsigned char> &v) {
        v.insert(v.end(), data_.cbegin(), data_.cend());
    }
    void write_to_file(std::ofstream& file) {
        file.write(reinterpret_cast<const char *>(&data_[0]), data_.size()*sizeof(unsigned char));
    }
//    void Clear() {
//        data_.clear();
//        bit_number_ = 0;
//        visited_number_ = 0;
//    }

private:
    std::vector<unsigned char> data_;
    unsigned long long bit_number_;
    unsigned long long visited_number_;
    const int pow2[8] = {1, 2, 4, 8, 16, 32, 64, 128};
};


#endif //SECOND_COMPRESS_BIT_MAP_H
