cmake_minimum_required(VERSION 3.17)
project(libmkb)

set(CMAKE_CXX_STANDARD 17)

include_directories(. dep/eigen-3.3.8)

add_executable(libmkb
        main.cpp
#        lzload.cpp
        pool.cpp 
#        stagedef_cnv.cpp
        endian.cpp
        mathutil.h
        mathutil.cpp
        vecutil.h)