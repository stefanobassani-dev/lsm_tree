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
    if (read_key != 1) {
      fclose(fd);
      return SSTABLE_SEARCH_ERROR;
    }

    int cmp = strncmp(key, buffer, 64);
    // key match
    if (cmp == 0) {
      int read_value = fread(result, 128, 1, fd);
      if (read_value != 1) {
        fclose(fd);
        return SSTABLE_SEARCH_ERROR;
      }

      // check if is deleted
      return strcmp(result, TOMBSTONE_VALUE) == 0 ? SSTABLE_SEARCH_TOMBSTONE
                                                  : SSTABLE_SEARCH_FOUND;
    }

    if (cmp < 0)
      end = mid - 1;
    else
      start = mid + 1;
  }

  fclose(fd);
  return SSTABLE_SEARCH_NOT_FOUND;
}

void two_way_marge(char* filename1, char* filename2) {
  FILE* fd1 = fopen(filename1, "rb");
  if (fd1 == NULL) return;
  FILE* fd2 = fopen(filename2, "rb");
  if (fd2 == NULL) return;

  char* buffer1[192];
  char* buffer2[192];

  int has1 = fread(buffer1, 192, 1, fd1);
  int has2 = fread(buffer2, 192, 1, fd1);

  while (has1 == 1 && has2 == 1) {
    char key1[64];
  }
}