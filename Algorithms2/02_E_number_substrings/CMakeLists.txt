cmake_minimum_required(VERSION 3.16)
project(02_E_number_substrings)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address,undefined -fno-sanitize-recover=all"
        CACHE STRING "Compiler flags in asan build"
        FORCE)

add_executable(02_E_number_substrings main.cpp)
add_executable(02_E_number_substrings_test test.cpp)