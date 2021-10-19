#include <cstring>
#include<bits/stdc++.h>
#include "../include/lz_alg.h"
using namespace std;
int hash_head[256*256*256];
int hash_val[kPageSize+5];
int hash_next[kPageSize+5];

std::vector<int> lz_scan(const unsigned char *in, const size_t kLength) {
    std::vector<int> out;
    out.reserve(kLength);
    memset(hash_head, 0, 256*256*256*sizeof(int));
    memset(hash_val, 0, (kPageSize+5)*sizeof(int));
    memset(hash_next, 0, (kPageSize+5)*sizeof(int));

    for (int i=0; i<kLength; ++i) {
        out.push_back(in[i]);
    }

    int already_match = 0;
    // Start from 2, traverse the whole page
    for (int i = 2; i < kLength; i++) {
//        out.push_back(in[i]);
        hash_val[i] = in[i-2] * 65536 + in[i-1] * 256 + in[i];

        if (already_match <= i) {
            int curr = hash_head[hash_val[i]];
            int longest_match = 0;
            int longest_pos = -1;
            // at most jump for 256 in hash_list, can be modified
            int hop = kMaxHopNumber;
            // Here traverse the hash_list with up to hop matching objects
            while (curr!=0 && hop--) {
                int extra_match = 0;
                // Find the match length
                while (extra_match <= kMaxMatch-kMinMatch+1 && i+extra_match+1 < kLength &&
                       in[curr+extra_match+1] == in[i+extra_match+1]) {
                    ++extra_match;
                }
                if (extra_match + kMinMatch > longest_match) {
                    longest_match = extra_match + kMinMatch;
                    longest_pos = curr;
                }
                curr = hash_next[curr];
                if (longest_match > kMaxMatch-3) break;
            }
            longest_match = min(kMaxMatch, longest_match);
            // Find a match, modify result array, tag those to be unshow (value to -1)
            if (longest_match > kMinMatch) {
                for (int j = i-2; j < i-2+longest_match; ++j) {
                    out[j] = -1;
                }
                out[i] = (i-longest_pos) * 256 + longest_match;
//  debug
//                std::cout << "i: " << i << ", longest_pos: " << longest_pos << ", longest_match: " << longest_match << std::endl;
                already_match = i + longest_match;
            }
        }
// debug
//        std::cout << "i: " << i << std::endl;
        // update hash of curr string
        hash_next[i] = hash_head[hash_val[i]];
        hash_head[hash_val[i]] = i;
    }
    return out;
}

//void get_lz_code(const int tmp_lz_code[], const size_t length, BitMap& bit_map, std::vector<unsigned char>& lz_code) {
void get_lz_code(const std::vector<int>& tmp_lz_code, BitMap& bit_map, std::vector<unsigned char> &lz_code) {
    for (const auto &code : tmp_lz_code) {
        if (code == -1) {
            continue;
        }

        if (code < 256) {
            bit_map.push_back(0);
            lz_code.push_back(static_cast<unsigned char>(code));
        } else {
            bit_map.push_back(1);
            int distance  = code / 256;
            int match_len = code % 256;
            for (int extra_byte = 0; extra_byte <= 2; extra_byte++) {
                if (distance<dis_table[extra_byte+1][2]) {
                    int distance_flag = dis_table[extra_byte][0] + (distance-dis_table[extra_byte][2])/dis_table[extra_byte][3];
                    lz_code.push_back(distance_flag);
                    int bias = distance - dis_table[extra_byte][2];
                    for (int j=0; j<extra_byte; ++j) {
                        lz_code.push_back( (bias >> (j*8)) & 0xFF );
                    }
                    break;
                }
            }
            lz_code.push_back(match_len);
        }
    }
}

void gen_lz_out(BitMap &bit_map, const std::vector<unsigned char> &kLzCode, std::vector<unsigned char> &out) {
    int bitmap_size = static_cast<int>(bit_map.capacity());
    int code_size   = static_cast<int>(kLzCode.size());
    for (int i=0; i<sizeof(bitmap_size); ++i) {
        out.push_back( (bitmap_size >> (i*8)) & 0xFF );
    }
    for (int i=0; i<sizeof(code_size); ++i) {
        out.push_back( (code_size >> (i*8)) & 0xFF );
    }
    bit_map.append_to_vector(out);
    out.insert(out.end(), kLzCode.cbegin(), kLzCode.cend());
}

void lz_compress(const unsigned char *file_data, const size_t kFileSize, std::vector<unsigned char> &compressed_file) {
    std::vector<int> tmp_lz_code = lz_scan(file_data, kFileSize);
    // debug
//    std::cout << "tmp_lz" << std::endl;
//    for (int i=0; i<tmp_lz_code.size(); ++i) {
//        std::cout << i << ": " << (int)tmp_lz_code[i] << std::endl;
//    }

    BitMap bit_map;
    std::vector<unsigned char> lz_code;
    get_lz_code(tmp_lz_code, bit_map, lz_code);
    // debug
//    std::cout << std::endl << "bit_map" << std::endl;
//    while(!bit_map.at_end()) {
//        std::cout << bit_map.get() << std::endl;
//    }

//    std::cout << std::endl << "lz_code" << std::endl;
//    for (int i=0; i<lz_code.size(); ++i) {
//        std::cout << i << ": " << (int)lz_code[i] << std::endl;
//    }

    gen_lz_out(bit_map, lz_code, compressed_file);
    // debug
//    std::cout << std::endl << "file_res" << std::endl;
//    for (int i=0; i<compressed_file.size(); ++i) {
//        std::cout << i << ": " << (int)compressed_file[i] << std::endl;
//    }
}

void lz_decompress(const unsigned char *kInput, const size_t kInLength, std::vector<unsigned char> &out) {
    int bitmap_size;
    bitmap_size = array_to_integer<int>(&kInput[0], 4);

    const size_t kCodeSize = kInLength - bitmap_size - 8;
    BitMap bit_map(&kInput[8], bitmap_size);
    const unsigned char *lz_code = &kInput[8 + bitmap_size];

    int pos = 0;
    while (pos < kCodeSize) {
        if (!bit_map.get()) {
            out.push_back(lz_code[pos++]);
        } else {
            unsigned char distance_flag = lz_code[pos++];
            int distance;
            int length;
            for (int extra_byte = 0; extra_byte <= 2; extra_byte++) {
                if (distance_flag<dis_table[extra_byte][1]) {
                    distance = (distance_flag - dis_table[extra_byte][0]) * dis_table[extra_byte][3] + dis_table[extra_byte][2];
                    for (int ext = 0; ext < extra_byte; ext++) distance += lz_code[pos++] * dis_table[ext][3];
                    break;
                }
            }
            length = lz_code[pos++];
            for (int i=0; i<length; ++i) {
                out.push_back(out[out.size()-distance]);
            }
        }
    }
}