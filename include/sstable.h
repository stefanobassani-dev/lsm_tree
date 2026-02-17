#ifndef SSTABLE_H
#define SSTABLE_H

#include "memtable.h"

#define SSTABLE_FLUSH_SUCCESS 0
#define SSTABLE_FLUSH_ERROR -1

#define SSTABLE_SEARCH_FOUND 1
#define SSTABLE_SEARCH_NOT_FOUND 0
#define SSTABLE_SEARCH_ERROR -1

int sstable_flush(memtable_t* memtable, int next_file_id);
int sstable_search(int file_id, const char* key, char* result);

#endif
