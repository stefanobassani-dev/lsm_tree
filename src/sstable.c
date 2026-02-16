#include "sstable.h"

#include <stdio.h>

#include "memtable.h"

int sstable_flush(memtable_t* memtable, int next_file_id) {
  char filename[64];
  sprintf(filename, "sstable_%d.bin", next_file_id);

  FILE* fd = fopen(filename, "wb");
  if (fd == NULL) {
    perror("Error opening SSTables");
    return SSTABLE_FLUSH_ERROR;
  }
  s_node_t* curr = memtable->head->forward[0];

  printf("Flushing on disk...\n");
  while (curr != NULL) {
    fwrite(curr->key, sizeof(char), 64, fd);
    fwrite(curr->value, sizeof(char), 128, fd);
    curr = curr->forward[0];
  }

  fclose(fd);
  return SSTABLE_FLUSH_SUCCESS;
}