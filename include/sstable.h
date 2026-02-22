#ifndef SSTABLE_H
#define SSTABLE_H

#include <stdbool.h>

#include "memtable.h"

#define SSTABLE_FLUSH_SUCCESS 0
#define SSTABLE_FLUSH_ERROR -1

#define SSTABLE_SEARCH_FOUND 1
#define SSTABLE_SEARCH_NOT_FOUND 0
#define SSTABLE_SEARCH_TOMBSTONE -1
#define SSTABLE_SEARCH_ERROR -2

#define TOMBSTONE_VALUE "#__deleted__#"

int sstable_flush(memtable_t* memtable, char* filename);
int sstable_search(char* filename, const char* key, char* result);
void two_way_marge(char* filename1, char* filename2, char* merge_filename,
                   bool is_temp);

#endif
