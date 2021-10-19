/*
 * @Author         : Long XIANG
 * @AuthorMail     : xiangl3@mail.sustech.edu.cn
 * @LastEditor     : Long XIANG
 * @LastEditorMail : xiangl3@mail.sustech.edu.cn
 * @Version        : 2.0
 * @Description    : 压缩引擎一，使用增量编码（delta coding）剔除数据帧格式数据中，连续采样之间未发生变化的数据以实现数据压缩的目的
 *                 : 并且保留增量数据，周期数据中高频出现的数据模式（frequency pattern）为后一级的压缩做好数据准备
 *                 : 此版本将delta_coding部分完全独立出来供上板实现
 */

#include <iostream>
#include <vector>
#include <string>
#include "include/frame_io.h"
#include "include/coding_1.h"

int main(int argc, char* argv[]) {
    if (argc!=3) {
        std::cerr << "Usage: compress_engine_1 <input_file_path> <output_file_path>" << std::endl;
        exit(1);
    }

    std::ifstream  in(argv[1], std::ios_base::binary);
    std::ofstream out(argv[2], std::ios_base::binary);
    if (in.fail()) {
        std::cerr << "Error: Open input file Fail!" << std::endl;
        exit(1);
    }
    if (out.fail()) {
        std::cerr << "Error: Open output file Fail!" << std::endl;
        exit(1);
    }

    std::vector<unsigned char> file_data = read_file(in);

    const size_t kDataSize = file_data.size();

//    std::cout << "file size: " << kDataSize << std::endl;
    std::vector<unsigned char> result;
    result.reserve(kDataSize*2);

    delta_compress(static_cast<unsigned char *>(&file_data[0]), kDataSize, result);

    write_result(out, result);

    return 0;
}
