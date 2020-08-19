cmake_minimum_required(VERSION 3.17)
project(libmkb)

set(CMAKE_CXX_STANDARD 17)

# Specify 32-bit processor for better linting
set(CMAKE_C_FLAGS -m32)
set(CMAKE_CXX_FLAGS -m32)

include_directories(.)

add_executable(libmkb main.cpp lzload.cpp)