cmake_minimum_required(VERSION 3.10)
project(second_compress)

set(CMAKE_CXX_STANDARD 14)

add_executable(compress_engine_2 compress_engine_2.cpp src/io_utils.cpp include/bit_map.h include/lz_alg.h src/lz_alg.cpp)
add_executable(decompress_engine_2 decompress_engine_2.cpp src/io_utils.cpp include/bit_map.h include/lz_alg.h src/lz_alg.cpp)