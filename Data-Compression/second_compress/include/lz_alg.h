#ifndef SECOND_COMPRESS_LZ_ALG_H
#define SECOND_COMPRESS_LZ_ALG_H
#define BYTE_NUM_0 64
#define BYTE_NUM_1 186
#define BYTE_NUM_2 6
#include "bit_map.h"
#include "io_utils.h"


const int kPageSize = BYTE_NUM_2*65536 + BYTE_NUM_1*256 + BYTE_NUM_0*1;
const int kMaxHopNumber = 256;
const int kMinMatch     = 3;
const int kMaxMatch     = 255;

// new distance table:
//     dis_table[i] means need i extra bytes (0, 1, 2)
//     dis_table[i][0], dis_table[i][1] represent the distance_flag range.
//     dis_table[i][2] mean the start bias. 
//         calculate function: dis_table[i][2] = dis_table[i-1][2] + (dis_table[i-1][1] - dis_table[i-1][0]) * dis_table[i-1][3]
//     dis_table[i][3] means range per flag can represent (equals to 256^i).
const int dis_table[4][4] = {
        {0, BYTE_NUM_0, 1, 1},
        {BYTE_NUM_0, BYTE_NUM_0+BYTE_NUM_1, (BYTE_NUM_0-0)*1 + 1, 256},
        {BYTE_NUM_0+BYTE_NUM_1, BYTE_NUM_0+BYTE_NUM_1+BYTE_NUM_2, BYTE_NUM_1*256 + (BYTE_NUM_0-0)*1 + 1, 65536},
        {-1, -1, kPageSize, -1}, // maximum
};

std::vector<int> lz_scan(const unsigned char *in, const size_t length);

void get_lz_code(const std::vector<int>& tmp_lz_code, BitMap& bit_map, std::vector<unsigned char> &lz_code);

void gen_lz_out(BitMap &bit_map, const std::vector<unsigned char> &kLzCode, std::vector<unsigned char> &out);

void lz_compress(const unsigned char *file_data, const size_t file_size, std::vector<unsigned char> &compressed_file);

void lz_decompress(const unsigned char *kInput, const size_t kInLength, std::vector<unsigned char> &out);

#endif //SECOND_COMPRESS_LZ_ALG_H
