cmake_minimum_required(VERSION 3.28)
project(c_containers C)

set(CMAKE_C_STANDARD 23)

add_library(c_containers STATIC src/linked_list.c)
target_include_directories(c_containers PUBLIC include)
