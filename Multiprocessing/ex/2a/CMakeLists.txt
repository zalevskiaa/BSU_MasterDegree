cmake_minimum_required(VERSION 3.22)
project(ex_2a)

#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_COMPILER "/usr/bin/mpic++")

add_executable(ex_2a main.cpp)

find_package(MPI) #make it REQUIRED, if you want
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
target_link_libraries(ex_2a ${MPI_C_LIBRARIES})
