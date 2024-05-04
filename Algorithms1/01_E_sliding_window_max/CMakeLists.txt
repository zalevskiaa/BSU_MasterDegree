cmake_minimum_required(VERSION 3.16)
project(01_E_sliding_window_max)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address,undefined -fno-sanitize-recover=all"
        CACHE STRING "Compiler flags in asan build"
        FORCE)

add_executable(01_E_sliding_window_max main.cpp)