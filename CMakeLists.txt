cmake_minimum_required(VERSION 3.17)
project(libmkb)

set(CMAKE_CXX_STANDARD 17)

include_directories(. dep/eigen-3.3.8)

add_executable(libmkb
        src/main.cpp
#        lzload.cpp
        src/pool.cpp
#        stagedef_cnv.cpp
        src/endian.cpp
        src/mathutil.h
        src/mathutil.cpp
        src/vecutil.h)