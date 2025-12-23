cmake_minimum_required(VERSION 3.28)
project(aoc_05 C)

set(CMAKE_C_STANDARD 23)

add_subdirectory(c-containers)
add_executable(aoc_05 main.c)
target_link_libraries(aoc_05 PRIVATE c_containers)
