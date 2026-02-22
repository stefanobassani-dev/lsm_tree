cmake_minimum_required(VERSION 3.20)
project(lsm_tree C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

add_library(memtable STATIC src/memtable.c)
add_library(cli STATIC src/cli.c)
add_library(sstable STATIC src/sstable.c)
add_library(engine STATIC src/engine.c)

target_link_libraries(sstable PRIVATE memtable)
target_link_libraries(engine PRIVATE sstable)
target_link_libraries(engine PRIVATE memtable)
target_link_libraries(cli PRIVATE engine)

add_executable(lsm_tree src/main.c)

target_link_libraries(lsm_tree PRIVATE cli)

# Aggiungi l'eseguibile per il test del merge
add_executable(test_merge_runner src/test_merge.c)
target_link_libraries(test_merge_runner PRIVATE sstable memtable)
