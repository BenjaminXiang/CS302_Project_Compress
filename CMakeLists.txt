cmake_minimum_required(VERSION 3.10)
project(CS302_Project_Compress)

set(CMAKE_CXX_STANDARD 14)

include_directories(utils)

add_executable(CS302_Project_Compress
        utils/io.cpp
        main.cpp)

add_executable(bin2number
        utils/io.cpp
        bin2number.cpp)

add_executable(get_sample
        utils/io.cpp
        get_sample.cpp)
