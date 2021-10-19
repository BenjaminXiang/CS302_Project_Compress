#ifndef DATA_COMPRESSION_CODING_1_H
#define DATA_COMPRESSION_CODING_1_H

#include <climits>
#include <cmath>
#include <vector>

const int kGroupNumber = 8;
const int kGroupSize   = 4;
const int kRowFront    = 32;
const int kGroupFront  = 128;
const int kFrameSize   = 1088;

const int kSampleSizeList[] = {32, 64, 128, 256, 512, 1024};
const int kSampleLineList[] = {1, 2, 4, 8, 16, 32};

//int kPartHeaderLine[] = {0,1,1,1,0,0,0,0,0,0,0,0,103,69,35,1,0,0,0,0};
//const unsigned char kTailLine[] =   {0,0,0,0,0,0, 0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0, 0, 0,0,0,0,0,0};
const std::vector<unsigned char> kTailLine(32, 0);

void delta_coding(const int kSampleSize, long long& last_change_ts, long long current_ts,
                  unsigned char* read_buff, unsigned char* looked_buff, std::vector<unsigned char>& result);

void delta_compress(const unsigned char* data, const size_t kDataSize, std::vector<unsigned char>& result);

std::vector<unsigned char> gen_header_line(const long long timestamp, const unsigned char model,
                                           const unsigned char channel, const unsigned int packet_count);

void delta_decompress(std::vector<unsigned char>&in, std::vector<unsigned char>&out);

#endif //DATA_COMPRESSION_CODING_1_H
