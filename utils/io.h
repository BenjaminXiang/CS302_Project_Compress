#ifndef CS302_PROJECT_COMPRESS_IO_H
#define CS302_PROJECT_COMPRESS_IO_H

#include <iostream>
#include <fstream>
#include <vector>

std::vector<unsigned char> read_file(std::ifstream& in);

void write_result(std::ofstream& out, const std::vector<unsigned char>& result);

#endif //CS302_PROJECT_COMPRESS_IO_H
