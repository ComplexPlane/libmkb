cmake_minimum_required(VERSION 3.17)
project(libmkb)

set(CMAKE_CXX_STANDARD 17)

include_directories(include dep/eigen-3.3.8 dep/catch-2.13.2)

add_library(libmkb
#        lzload.cpp
        src/pool.cpp
#        stagedef_cnv.cpp
        src/endian.cpp
        src/mathutil.cpp
        src/event.cpp
        src/global_state.cpp
        )

add_subdirectory(test)