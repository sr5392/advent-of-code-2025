cmake_minimum_required(VERSION 3.28)
project(aoc_08 C)

set(CMAKE_C_STANDARD 23)

add_subdirectory(c-containers)
add_executable(aoc_08 main.c)
target_link_libraries(aoc_08 PRIVATE m)
target_link_libraries(aoc_08 PRIVATE c_containers)
