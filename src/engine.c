#include "engine.h"

#include <stdio.h>
#include <string.h>

#include "memtable.h"
#include "sstable.h"

static memtable_t* memtable = NULL;
static int next_file_id = 0;

void engine_init() {
  if (memtable == NULL) {
    memtable = create_memtable();
  }
}

void put(const char* key, const char* value) {
  engine_init();
  int status = insert(memtable, key, value);
  printf("Memtable size: %zu byte\n", memtable->size);

  if (status == MEMTABLE_FULL) {
    if (sstable_flush(memtable, next_file_id) == SSTABLE_FLUSH_SUCCESS) {
      next_file_id++;
      clear_memtable(memtable);
    }
  }
}

int get(const char* key, char* result) {
  engine_init();
  char* value = search(memtable, key);

  if (value != NULL) {
    strncpy(result, value, 128);
    result[127] = '\0';
  } else {
    for (int i = next_file_id - 1; i >= 0; i--) {
      int found = sstable_search(i, key, result);
      if (found == SSTABLE_SEARCH_FOUND) return SUCCESS;
      if (found == SSTABLE_SEARCH_ERROR) return ERROR;
    }
    result[0] = '\0';
    return NOT_FOUND;
  }

  return SUCCESS;
}

void tombstone(const char* key) { put(key, TOMBSTONE_VALUE); }