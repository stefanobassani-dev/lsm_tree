cmake_minimum_required(VERSION 4.1)
project(lsm_tree C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

add_library(database STATIC src/database.c)

add_executable(lsm_tree src/main.c)

target_link_libraries(lsm_tree PRIVATE database)
