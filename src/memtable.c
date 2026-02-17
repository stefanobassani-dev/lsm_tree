#include "memtable.h"

#include <stdlib.h>
#include <string.h>

int random_level() {
  int level = 0;

  while (rand() < RAND_MAX / 2 && level < MAX_LEVEL) {
    level++;
  }

  return level;
}

size_t calculate_node_size(int level) {
  size_t struct_size = sizeof(s_node_t);
  size_t pointers_size = (level + 1) * sizeof(s_node_t*);
  return struct_size + pointers_size;
}

s_node_t* create_node(const char* key, const char* value, int level) {
  s_node_t* node = malloc(sizeof(s_node_t));

  strncpy(node->key, key, 63);
  node->key[63] = '\0';

  strncpy(node->value, value, 127);
  node->key[127] = '\0';

  node->forward = calloc(level + 1, sizeof(s_node_t*));

  if (node->forward == NULL) {
    free(node);
    return NULL;
  }

  return node;
}

memtable_t* create_memtable() {
  memtable_t* memtable = malloc(sizeof(memtable_t));

  memtable->level = 0;
  memtable->size = sizeof(s_node_t) + (MAX_LEVEL + 1) * sizeof(s_node_t*);

  memtable->head = create_node("", "", MAX_LEVEL);

  return memtable;
}

char* search(memtable_t* memtable, const char* key) {
  s_node_t* curr = memtable->head;

  for (int i = memtable->level; i >= 0; i--) {
    while (curr->forward[i] != NULL && strcmp(curr->forward[i]->key, key) < 0) {
      curr = curr->forward[i];
    }
  }

  curr = curr->forward[0];
  if (curr != NULL && strcmp(curr->key, key) == 0) {
    return strcmp(curr->value, TOMBSTONE_VALUE) == 0 ? NULL : curr->value;
  }
  return NULL;
}

void clear_memtable(memtable_t* memtable) {
  s_node_t* curr = memtable->head->forward[0];

  while (curr != NULL) {
    s_node_t* next = curr->forward[0];

    free(curr->forward);
    free(curr);

    curr = next;
  }

  for (int i = 0; i <= MAX_LEVEL; i++) {
    memtable->head->forward[i] = NULL;
  }

  memtable->size = sizeof(s_node_t) + (MAX_LEVEL + 1) * sizeof(s_node_t*);
  memtable->level = 0;
}

int insert(memtable_t* memtable, const char* key, const char* value) {
  s_node_t* curr = memtable->head;
  s_node_t* updates[MAX_LEVEL + 1];

  for (int i = memtable->level; i >= 0; i--) {
    while (curr->forward[i] != NULL && strcmp(curr->forward[i]->key, key) < 0) {
      curr = curr->forward[i];
    }
    updates[i] = curr;
  }

  curr = curr->forward[0];

  if (curr != NULL && strcmp(curr->key, key) == 0) {
    strncpy(curr->value, value, 128);
    curr->value[127] = '\0';
    return memtable->size >= MEMTABLE_THRESHOLD ? MEMTABLE_FULL : MEMTABLE_OK;
  }

  int level = random_level();

  if (level > memtable->level) {
    for (int i = memtable->level + 1; i <= level; i++) {
      updates[i] = memtable->head;
    }
    memtable->level = level;
  }

  s_node_t* node = create_node(key, value, level);
  memtable->size += calculate_node_size(level);

  for (int i = 0; i <= level; i++) {
    node->forward[i] = updates[i]->forward[i];
    updates[i]->forward[i] = node;
  }

  return memtable->size >= MEMTABLE_THRESHOLD ? MEMTABLE_FULL : MEMTABLE_OK;
}