cmake_minimum_required(VERSION 3.10)
project(Data_Compression)

set(CMAKE_CXX_STANDARD 14)

add_executable(compress_engine_1 compress_engine_1.cpp src/frame_io.cpp src/coding_1.cpp)
add_executable(decompress_engine_1 decompress_engine_1.cpp src/frame_io.cpp src/coding_1.cpp)