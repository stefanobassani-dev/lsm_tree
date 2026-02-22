#include "engine.h"

#include <stdio.h>
#include <string.h>

#include "memtable.h"
#include "sstable.h"

static memtable_t* memtable = NULL;
static int next_file_id = 1;

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
    char filename[64];
    sprintf(filename, "sstable_%04d.bin", next_file_id);
    if (sstable_flush(memtable, filename) == SSTABLE_FLUSH_SUCCESS) {
      if (next_file_id % COMPACTION_THRESHOLD == 0) {
        compaction();
      }
      next_file_id++;
      clear_memtable(memtable);
    }
  }
}

int get(const char* key, char* result) {
  engine_init();
  int status = search(memtable, key, result);

  if (status == MEMTABLE_TOMBSTONE) {
    result[0] = '\0';
    return NOT_FOUND;
  }

  if (status == MEMTABLE_NOT_FOUND) {
    // search on disk
    char filename[64];
    for (int i = next_file_id - 1; i >= 0; i--) {
      sprintf(filename, "sstable_%04d.bin", i);
      int found = sstable_search(filename, key, result);

      if (found == SSTABLE_SEARCH_FOUND) return SUCCESS;
      if (found == SSTABLE_SEARCH_ERROR) return ERROR;
      if (found == SSTABLE_SEARCH_TOMBSTONE) {
        result[0] = '\0';
        return NOT_FOUND;
      }
    }
    result[0] = '\0';
    return NOT_FOUND;
  }

  result[127] = '\0';
  return SUCCESS;
}

void tombstone(const char* key) { put(key, TOMBSTONE_VALUE); }

void compaction() {
  char old_filename[64];
  char new_filename[64];
  char temp_merge_result_1[64];
  char temp_merge_result_2[64];
  char final_output_filename[64];

  sprintf(old_filename, "sstable_%04d.bin", next_file_id - 3);
  sprintf(new_filename, "sstable_%04d.bin", next_file_id - 2);
  sprintf(temp_merge_result_1, "sstable_temp_%04d.bin", next_file_id - 2);
  perform_merge_and_cleanup(old_filename, new_filename, temp_merge_result_1);

  sprintf(old_filename, "sstable_%04d.bin", next_file_id - 1);
  sprintf(new_filename, "sstable_%04d.bin", next_file_id);
  sprintf(temp_merge_result_2, "sstable_temp_%04d.bin", next_file_id);
  perform_merge_and_cleanup(old_filename, new_filename, temp_merge_result_2);

  sprintf(final_output_filename, "sstable_%04d.bin", next_file_id);
  two_way_marge(temp_merge_result_1, temp_merge_result_2,
                final_output_filename);

  if (remove(temp_merge_result_1) != 0) {
    perror("Error removing final temporary file 1");
  }
  if (remove(temp_merge_result_2) != 0) {
    perror("Error removing final temporary file 2");
  }
}

void perform_merge_and_cleanup(char* file1_path, char* file2_path,
                               char* output_path) {
  two_way_marge(file1_path, file2_path, output_path);

  if (remove(file1_path) != 0) {
    perror("Error removing old file after merge");
  }
  if (remove(file2_path) != 0) {
    perror("Error removing new file after merge");
  }
}