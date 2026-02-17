#ifndef MEMTABLE_H
#define MEMTABLE_H

#include <stddef.h>

#define MAX_LEVEL 3

#define MEMTABLE_THRESHOLD 512

#define MEMTABLE_OK 0
#define MEMTABLE_FULL 1
#define MEMTABLE_ERROR -1

#define TOMBSTONE_VALUE "#__deleted__#"

typedef struct s_node_t {
  char key[64];
  char value[128];
  struct s_node_t** forward;
} s_node_t;

typedef struct memtable_t {
  int level;
  s_node_t* head;
  size_t size;
} memtable_t;

int random_level();

size_t calculate_node_size(int level);

s_node_t* create_node(const char* key, const char* value, int level);

memtable_t* create_memtable();

char* search(memtable_t* memtable, const char* key);

int insert(memtable_t* memtable, const char* key, const char* value);

int delete (memtable_t* memtable, const char* key);

void clear_memtable(memtable_t* memtable);

#endif
