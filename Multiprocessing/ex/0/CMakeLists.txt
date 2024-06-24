cmake_minimum_required(VERSION 3.22)
project(ex_0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "/usr/bin/mpic++")

add_executable(ex_0 main.cpp)

find_package(MPI) #make it REQUIRED, if you want
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
target_link_libraries(ex_0 ${MPI_C_LIBRARIES})
