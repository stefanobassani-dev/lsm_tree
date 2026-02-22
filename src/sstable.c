#include "sstable.h"

#include <stdio.h>
#include <string.h>

#include "memtable.h"

int sstable_flush(memtable_t* memtable, char* filename) {
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

int sstable_search(char* filename, const char* key, char* result) {
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

void two_way_marge(char* filename1, char* filename2, char* merge_filename) {
  FILE* fd_old = fopen(filename1, "rb");
  if (fd_old == NULL) return;
  FILE* fd_new = fopen(filename2, "rb");
  if (fd_new == NULL) {
    fclose(fd_old);
    return;
  }
  FILE* fd_merge = fopen(merge_filename, "wb");
  if (fd_merge == NULL) {
    fclose(fd_old);
    fclose(fd_new);
    return;
  }

  char buffer1[192];
  char buffer2[192];

  int has1 = fread(buffer1, 192, 1, fd_old);
  int has2 = fread(buffer2, 192, 1, fd_new);

  while (has1 == 1 && has2 == 1) {
    buffer1[63] = '\0';
    buffer2[63] = '\0';

    int cmp = strcmp(buffer1, buffer2);

    char* target = (cmp < 0) ? buffer1 : buffer2;

    target[191] = '\0';
    char* value = target + 64;

    if (cmp < 0 || strcmp(value, TOMBSTONE_VALUE) != 0) {
      fwrite(target, 192, 1, fd_merge);
    }

    if (cmp <= 0) has1 = fread(buffer1, 192, 1, fd_old);
    if (cmp >= 0) has2 = fread(buffer2, 192, 1, fd_new);
  }

  while (has1 == 1) {
    buffer1[191] = '\0';
    fwrite(buffer1, 192, 1, fd_merge);
    has1 = fread(buffer1, 192, 1, fd_old);
  }

  while (has2 == 1) {
    buffer2[191] = '\0';
    fwrite(buffer2, 192, 1, fd_merge);
    has2 = fread(buffer2, 192, 1, fd_new);
  }

  fclose(fd_old);
  fclose(fd_new);
  fclose(fd_merge);
}