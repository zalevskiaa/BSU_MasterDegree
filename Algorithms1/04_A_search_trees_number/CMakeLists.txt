cmake_minimum_required(VERSION 3.16)
project(04_A_search_trees_number)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address,undefined -fno-sanitize-recover=all"
        CACHE STRING "Compiler flags in asan build"
        FORCE)

add_executable(04_A_search_trees_number main.cpp)