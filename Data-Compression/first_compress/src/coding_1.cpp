#include "../include/coding_1.h"
#include "../include/frame_io.h"
#include "../include/utils.h"

void delta_coding(const int kSampleSize, long long& last_change_ts, long long current_ts,
                  unsigned char* read_buff, unsigned char* looked_buff, std::vector<unsigned char>& result) {
    for (int sample_start=0; sample_start<kBytePerFrame; sample_start+=kSampleSize) {
        std::vector<unsigned char> sample_coding_list;
        for (int row_start=0, row_idx=0; row_start<kSampleSize; row_start+=kBytePerLine, ++row_idx) {
            sample_coding_list.push_back(kRowFront+row_idx);
            std::vector<unsigned char> group_coding_list;
            for (int group_idx=0; group_idx<kGroupNumber; ++group_idx) {
                unsigned char group_flag = kGroupFront+(group_idx<<4);
                std::vector<unsigned char> changed_list;
                for (int idx=0; idx<kGroupSize; ++idx) {
                    int sample_idx = row_start+group_idx*kGroupSize+idx;
                    if (read_buff[sample_start+sample_idx] != looked_buff[sample_idx]) {
                        group_flag += (1<<idx);
                        changed_list.push_back(read_buff[sample_start+sample_idx]);
                        looked_buff[sample_idx] = read_buff[sample_start+sample_idx];
                    }
                }
                if (!changed_list.empty()) {
                    group_coding_list.push_back(group_flag);
                    for (auto val : changed_list) {
                        group_coding_list.push_back(val);
                    }
                }
                changed_list.clear();
            }
           if (group_coding_list.empty()) {
               sample_coding_list.pop_back();
           } else {
               for (auto row_code : group_coding_list) {
                   sample_coding_list.push_back(row_code);
               }
           }
        }
        if (!sample_coding_list.empty()) {
            long long delta_ts = current_ts - last_change_ts - 1;
            if (delta_ts<16) {
                result.push_back((unsigned char)delta_ts);
            } else {
                unsigned char ts_flag = 16;
                std::vector<unsigned char> delta_ts_code;
                if (delta_ts<=UCHAR_MAX) {
                    delta_ts_code = integer_to_array((unsigned char) delta_ts);
                    ts_flag += 1;
                } else if (delta_ts>UCHAR_MAX && delta_ts<=USHRT_MAX) {
                    delta_ts_code = integer_to_array((unsigned short) delta_ts);
                    ts_flag += 2;
                } else if (delta_ts>USHRT_MAX && delta_ts<=UINT_MAX) {
                    delta_ts_code = integer_to_array((unsigned int) delta_ts);
                    ts_flag += 4;
                } else if (delta_ts>UINT_MAX && delta_ts<=ULLONG_MAX) {
                    delta_ts_code = integer_to_array((unsigned long long) delta_ts);
                    ts_flag += 8;
                }
                result.push_back(ts_flag);
                for (const auto& ts_code : delta_ts_code) {
                    result.push_back(ts_code);
                }
            }
            for (const auto& code : sample_coding_list) {
                result.push_back(code);
            }
            last_change_ts = current_ts;
        }
        ++current_ts;
    }
}

void delta_compress(const unsigned char* data, const size_t kDataSize, std::vector<unsigned char> &result) {
    long long timestamp;
    unsigned char model, channel;
    get_header(&data[0], timestamp, model, channel);
    push_integer_to_vector(channel, result);
    push_integer_to_vector(model, result);
    push_integer_to_vector(timestamp, result);

    const int kSampleSize = kSampleSizeList[model];
    unsigned char looked_buff[kSampleSize];
    memset(looked_buff, 0, kSampleSize*sizeof(unsigned char));

    long long last_change_ts = timestamp-1;
    long long current_ts = timestamp;
    for (size_t i=0; i<kDataSize; i+=kFrameSize) { //处理一个数据帧
        for (size_t pos = i+kBytePerLine; pos<i+kBytePerLine+kBytePerFrame; pos+=kSampleSize) { //处理一个dataframe,pos为一个sample在dataframe中的起始位置
            std::vector<unsigned char> sample_coding_list;
            for (int row_start=0, row_idx=0; row_start<kSampleSize; row_start+=kBytePerLine, ++row_idx) { //处理一个sample
                std::vector<unsigned char> group_coding_list;
                for (int group_idx=0; group_idx<kGroupNumber; ++group_idx) { //处理sample中的1行,每行中有8组
                    unsigned char group_flag = kGroupFront+(group_idx<<4); //组标签 1xxx_xxxx,(128+(group_idx<<4))
                    std::vector<unsigned char> changed_list;
                    for (int idx=0; idx<kGroupSize; ++idx) { //1组有4个bytes
                        int sample_idx = row_start+group_idx*kGroupSize+idx;
                        if (data[pos+sample_idx] != looked_buff[sample_idx]) {
                            group_flag += (1<<idx); //编码组内变化情况
                            changed_list.push_back(data[pos+sample_idx]);
                            looked_buff[sample_idx] = data[pos+sample_idx];
                        }
                    }
                    if (!changed_list.empty()) {
                        group_coding_list.push_back(group_flag); // 添加(组标签)
                        for (auto val : changed_list) { // 组内编码
                            group_coding_list.push_back(val);
                        }
                    }
                    changed_list.clear();
                } // 完成1行
                if (!group_coding_list.empty()) {
                    sample_coding_list.push_back(kRowFront+row_idx); // 添加(行标签)
                    for (auto row_code : group_coding_list) { // 将一行中的编码添加至sample_coding
                        sample_coding_list.push_back(row_code);
                    }
                }
            } // 完成1个sample
            if (!sample_coding_list.empty()) { //  添加(dalta_timestamp)
                long long delta_ts = current_ts - last_change_ts - 1;
                if (delta_ts<16) {
                    result.push_back((unsigned char)delta_ts);
                } else {
                    unsigned char ts_flag = 16;
                    std::vector<unsigned char> delta_ts_code;
                    if (delta_ts<=UCHAR_MAX) {
                        delta_ts_code = integer_to_array((unsigned char) delta_ts);
                        ts_flag += 1;
                    } else if (delta_ts>UCHAR_MAX && delta_ts<=USHRT_MAX) {
                        delta_ts_code = integer_to_array((unsigned short) delta_ts);
                        ts_flag += 2;
                    } else if (delta_ts>USHRT_MAX && delta_ts<=UINT_MAX) {
                        delta_ts_code = integer_to_array((unsigned int) delta_ts);
                        ts_flag += 4;
                    } else if (delta_ts>UINT_MAX && delta_ts<=ULLONG_MAX) {
                        delta_ts_code = integer_to_array((unsigned long long) delta_ts);
                        ts_flag += 8;
                    }
                    result.push_back(ts_flag);
                    for (const auto& ts_code : delta_ts_code) {
                        result.push_back(ts_code);
                    }
                }
                for (const auto& code : sample_coding_list) { // 将1个sample的编码添加到result中
                    result.push_back(code);
                }
                last_change_ts = current_ts;
            }
            ++current_ts;
        } // 完成1个dataframe
    }
    auto tail_ts_code = integer_to_array(current_ts-last_change_ts); // 尾编码,为了防止最后多个采样数据一致造成数据丢失,补一个delta_ts
    for (const auto &code : tail_ts_code) {
        result.push_back(code);
    }
//    delete[] looked_buff;
}

std::vector<unsigned char> gen_header_line(const long long timestamp, const unsigned char model,
                                           const unsigned char channel, const unsigned int packet_count) {
    std::vector<unsigned char> head_line = {0,0,0,0,0,0,16,0,32,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,103,69,35,1,0,0,0,0};
    for (int i=0, j=0; i<=5; ++i, ++j) {
        head_line[i] = (timestamp >> (j*8)) & 0xFF;
//        std::cout << "head_line i: " << i << ", " << (int)head_line[i] << std::endl;
    }

    unsigned short code_model = 16*(1<<model);
//    std::cout << "model_number: " << code_model << std::endl;
    for (int i=6, j=0; i<=7; ++i, ++j) {
        head_line[i] = (code_model >> (j*8)) & 0xFF;
//        std::cout << "head_line i: " << i << ", " << (int)head_line[i] << std::endl;
    }

    for (int i=9, j=0; i<=12; ++i, ++j) {
        head_line[i] = (packet_count >> (j*8)) & 0xFF;
//        std::cout << "head_line i: " << i << ", " << (int)head_line[i] << std::endl;
    }

    head_line[14] = channel+1;

    return head_line;
}

void append_sample(long long &timestamp, const unsigned char kModel, const unsigned char kChannel,
                   const int kSampleLines, size_t &line_count, unsigned int &packet_count,
                   const std::vector<unsigned char> &looked_sample, std::vector<unsigned char>&out) {
    if (line_count%34==0) {
        auto header_line = gen_header_line(timestamp, kModel, kChannel, packet_count);
        out.insert(out.end(), header_line.cbegin(), header_line.cend());
        ++line_count;
    }
    out.insert(out.end(), looked_sample.cbegin(), looked_sample.cend());
    line_count += kSampleLines;
    ++timestamp;
//    int i=0;
//    for (auto &ele: out) {
//        std::cout << (int)ele << ", ";
//        ++i;
//        if (i%32==0) {
//            std::cout << std::endl;
//        }
//    }
//    std::cout << "looked_sample: ";
//    for (auto &ele: looked_sample) {
//        std::cout << (int)ele << ", ";
//    }
//    std::cout << std::endl;
//    std::cout << "line_count: " << line_count << std::endl;
//    std::cout << "timestamp: " << timestamp << std::endl;
    if (line_count%34==33) {
        out.insert(out.end(), kTailLine.cbegin(), kTailLine.cend());
        ++line_count;
        ++packet_count;
    }
}

void delta_decompress(std::vector<unsigned char> &in, std::vector<unsigned char> &out) {
    auto channel = array_to_integer<unsigned char>(&in[0], sizeof(unsigned char));
    auto model = array_to_integer<unsigned char>(&in[1], sizeof(unsigned char));
    auto timestamp  = array_to_integer<long long>(&in[2], sizeof(long long));

    std::cout << "channel: " << (int)channel << ", model: " << (int)model << ", timestamp: " << timestamp << std::endl;

    std::vector<unsigned char> looked_sample(kSampleSizeList[model]);
    const int kSampleLines = kSampleLineList[model];

//    auto header_line = gen_header_line(timestamp, model, channel, 0); //得到第一帧帧头
//    out.insert(out.end(), header_line.cbegin(), header_line.cend()); //将第一帧帧头写入结果文件
//    for (auto &ele: out) {
//        std::cout << (int)ele << ", " ;
//    }
//    std::cout << std::endl;
    unsigned int packet_count = 0;
    size_t line_count = 0;
    bool is_first = true;

    unsigned char row;
    size_t pos = 10;
    while(pos<in.size()-8) {
//        std::cout << "pos: " << pos << ", in[pos]: " << (int)in[pos++] << std::endl;
        if(in[pos]<32) { // 此时读入的是delta timestamp, 将delta_ts间未变化的sample补齐
            long long delta_ts;
            if(in[pos]<16) {
                delta_ts = in[pos]+1;
                ++pos;
            } else {
                int ts_len = in[pos]-16;
                delta_ts = array_to_integer<long long>(&in[pos+1], ts_len);
                pos += ts_len;
            }

//            std::cout << "pos: " << pos << ", delta_ts: " << delta_ts << std::endl;
            if (is_first && delta_ts==1) {
                is_first = false;
            } else {
                for (int i=0; i<delta_ts; ++i) {
//                    std::cout << "timestamp: " << timestamp << ", model: " << (int)model << ", channel: " << (int)channel <<", line_count: " << line_count << ", packet_count: " << packet_count << std::endl;
                    append_sample(timestamp, model, channel, kSampleLines, line_count, packet_count,
                                  looked_sample, out); // 补齐delta_ts间的sample
                }
            }
        } else if (in[pos]>=32 && in[pos]<64) { // 此时读入的是row,由此开始还原出一个sample
            row = in[pos]-kRowFront;
//            std::cout << "row: " << (int)row << ", pos: " << pos << ", in[pos]: " << (int)in[pos] << std::endl;
            ++pos;
        } else if (in[pos]>=128) {
            unsigned char col_group = (in[pos] & 0x70)>>4;
            unsigned char col_flag  = in[pos] & 0x0f;
//            std::cout << "pos: " << pos << ", in[pos]: " << (int)in[pos] << ", col_group: " << (int)col_group << ", col_flag: " << (int)col_flag << std::endl;
            for (int idx=0; idx<kGroupSize; ++idx) {
                if (col_flag & (1<<idx)) {
                    looked_sample[row*kBytePerLine+col_group*kGroupSize+idx] = in[++pos];
//                    std::cout << "looked_sample[" << row*kBytePerLine+col_group*kGroupSize+idx << "]=" << (int)looked_sample[row*kBytePerLine+col_group*kGroupSize+idx]<<std::endl;
                }
            }
            ++pos;
//            if (in[pos+1]<32) {
//                append_sample(timestamp, model, channel, kSampleLines, line_count, packet_count,
//                              looked_sample, out);
//            }
        }
    }
    auto tail_ts = array_to_integer<long long>(&in[pos], 8);
    for (int i=0; i<tail_ts; ++i) {
        if (line_count%34==0) {
            auto header_line = gen_header_line(timestamp, model, channel, packet_count);
            out.insert(out.end(), header_line.cbegin(), header_line.cend());
            ++line_count;
        }
        out.insert(out.end(), looked_sample.cbegin(), looked_sample.cend());
        line_count += kSampleLines;
        ++timestamp;
        if (line_count%34==33) {
            out.insert(out.end(), kTailLine.cbegin(), kTailLine.cend());
            ++line_count;
            ++packet_count;
        }
    }
}
