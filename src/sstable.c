#include "sstable.h"

#include <stdio.h>
#include <string.h>

#include "memtable.h"

int sstable_flush(memtable_t* memtable, int next_file_id) {
  char filename[64];
  sprintf(filename, "sstable_%04d.bin", next_file_id);

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

int sstable_search(int file_id, const char* key, char* result) {
  char filename[64];
  sprintf(filename, "sstable_%04d.bin", file_id);

  FILE* fd = fopen(filename, "rb");
  if (fd == NULL) return SSTABLE_SEARCH_NOT_FOUND;
  fseek(fd, 0, SEEK_END);
  int size = ftell(fd);
  int rows = size / 192;

  int start = 0;
  int end = rows - 1;

  char buffer[64];

  while (start <= end) {
    int mid = start + (end - start) / 2;
    fseek(fd, mid * 192, SEEK_SET);

    int read_key = fread(buffer, 64, 1, fd);

    if (read_key == 1) {
      int cmp = strcmp(key, buffer);
      if (cmp == 0) {
        int read_value = fread(result, 128, 1, fd);
        fclose(fd);
        return read_value == 1 ? SSTABLE_SEARCH_FOUND : SSTABLE_SEARCH_ERROR;
      } else {
        if (cmp < 0) {
          end = mid - 1;
        } else {
          start = mid + 1;
        }
      }
    } else {
      fclose(fd);
      return SSTABLE_SEARCH_ERROR;
    }
  }

  return SSTABLE_SEARCH_NOT_FOUND;
}