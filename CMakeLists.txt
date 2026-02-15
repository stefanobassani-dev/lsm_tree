cmake_minimum_required(VERSION 3.20)
project(lsm_tree C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

add_library(database STATIC src/database.c)
add_library(skip_list STATIC src/skip_list.c)
add_library(cli STATIC src/cli.c)

target_link_libraries(cli PRIVATE skip_list)

add_executable(lsm_tree src/main.c)

target_link_libraries(lsm_tree PRIVATE cli)
