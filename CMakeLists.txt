cmake_minimum_required(VERSION 3.17)
project(libmkb)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(libmkb main.cpp lzload.cpp pool.cpp)