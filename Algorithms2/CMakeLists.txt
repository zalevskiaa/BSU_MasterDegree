cmake_minimum_required(VERSION 3.16)
project(zalevskiaa)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address,undefined -fno-sanitize-recover=all"
        CACHE STRING "Compiler flags in asan build"
        FORCE)

add_subdirectory("${PROJECT_SOURCE_DIR}/00_A_hello_world")
add_subdirectory("${PROJECT_SOURCE_DIR}/00_B_sum")

#add_subdirectory("${PROJECT_SOURCE_DIR}/01_A_prefix_function")
#add_subdirectory("${PROJECT_SOURCE_DIR}/01_B_z_function")
#add_subdirectory("${PROJECT_SOURCE_DIR}/01_C_inaccurate_match")
#add_subdirectory("${PROJECT_SOURCE_DIR}/01_D_max_repeating_sequence")

add_subdirectory("${PROJECT_SOURCE_DIR}/02_A_aho_corasick")
add_subdirectory("${PROJECT_SOURCE_DIR}/02_A_aho_corasick_3")
add_subdirectory("${PROJECT_SOURCE_DIR}/02_B_banned_strings")
add_subdirectory("${PROJECT_SOURCE_DIR}/02_C_lz77")
#add_subdirectory("${PROJECT_SOURCE_DIR}/02_D_bwt") # failed :(
add_subdirectory("${PROJECT_SOURCE_DIR}/02_E_number_substrings")

add_subdirectory("${PROJECT_SOURCE_DIR}/03_A_fuzzy_dictionary")
add_subdirectory("${PROJECT_SOURCE_DIR}/03_B_min_hamming_distance")
add_subdirectory("${PROJECT_SOURCE_DIR}/03_C_nenokku")
add_subdirectory("${PROJECT_SOURCE_DIR}/03_D_close_relatives")

add_subdirectory("${PROJECT_SOURCE_DIR}/04_A_simple_max_flow")
add_subdirectory("${PROJECT_SOURCE_DIR}/04_B_relocation")
add_subdirectory("${PROJECT_SOURCE_DIR}/04_C_basketball")

add_subdirectory("${PROJECT_SOURCE_DIR}/05_B_equivalence")
add_subdirectory("${PROJECT_SOURCE_DIR}/05_D_archieve")

