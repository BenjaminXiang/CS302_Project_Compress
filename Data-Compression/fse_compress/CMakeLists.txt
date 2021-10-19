cmake_minimum_required(VERSION 3.10)
project(Data_Compression)

set(CMAKE_CXX_STANDARD 14)

add_executable(fse_compress fse_compress.cpp)
add_executable(fse_decompress fse_decompress.cpp)