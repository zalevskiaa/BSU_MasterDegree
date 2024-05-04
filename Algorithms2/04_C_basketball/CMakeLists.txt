cmake_minimum_required(VERSION 3.16)
project(04_C_basketball)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address,undefined -fno-sanitize-recover=all"
        CACHE STRING "Compiler flags in asan build"
        FORCE)

add_executable(04_C_basketball main.cpp)
add_executable(04_C_basketball_test test.cpp)