#ifndef SSTABLE_H
#define SSTABLE_H

#include "memtable.h"

#define SSTABLE_FLUSH_SUCCESS 0
#define SSTABLE_FLUSH_ERROR -1

int sstable_flush(memtable_t* memtable, int next_file_id);

#endif
