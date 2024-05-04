cmake_minimum_required(VERSION 3.16)
project(zalevskiaa)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address,undefined -fno-sanitize-recover=all"
        CACHE STRING "Compiler flags in asan build"
        FORCE)

add_subdirectory("${PROJECT_SOURCE_DIR}/00_hello_world")
add_subdirectory("${PROJECT_SOURCE_DIR}/00_hello_world/hello_world")
add_subdirectory("${PROJECT_SOURCE_DIR}/00_hello_world/bad_max_int")

add_subdirectory("${PROJECT_SOURCE_DIR}/01_A_longest_subsequence")
add_subdirectory("${PROJECT_SOURCE_DIR}/01_B_bracket_sequence")
add_subdirectory("${PROJECT_SOURCE_DIR}/01_E_sliding_window_max")

add_subdirectory("${PROJECT_SOURCE_DIR}/02_A_three_sequences")
add_subdirectory("${PROJECT_SOURCE_DIR}/02_B_football_team")
add_subdirectory("${PROJECT_SOURCE_DIR}/02_C_merge_sorted_sequences")
add_subdirectory("${PROJECT_SOURCE_DIR}/02_D_one_dimensional_postman")

add_subdirectory("${PROJECT_SOURCE_DIR}/03_A_fixed_set")
add_subdirectory("${PROJECT_SOURCE_DIR}/03_C_memory_manager")

add_subdirectory("${PROJECT_SOURCE_DIR}/04_A_search_trees_number")

add_subdirectory("${PROJECT_SOURCE_DIR}/05_B_post_robot")
add_subdirectory("${PROJECT_SOURCE_DIR}/05_C_min_road_cost")

add_subdirectory("${PROJECT_SOURCE_DIR}/06_D_magic")
add_subdirectory("${PROJECT_SOURCE_DIR}/06_E_segments_union_len")


