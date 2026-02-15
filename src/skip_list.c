#include "skip_list.h"

#include <stdlib.h>
#include <string.h>

int random_level() {
  int level = 0;

  while (rand() < RAND_MAX / 2 && level < MAX_LEVEL) {
    level++;
  }

  return level;
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

skip_list_t* create_skiplist() {
  skip_list_t* list = malloc(sizeof(skip_list_t));

  list->level = 0;

  list->head = create_node("", "", MAX_LEVEL);

  return list;
}

char* search(skip_list_t* list, char* key) {
  s_node_t* curr = list->head;

  for (int i = list->level; i >= 0; i--) {
    while (curr->forward[i] != NULL && strcmp(curr->forward[i]->key, key) < 0) {
      curr = curr->forward[i];
    }
  }

  curr = curr->forward[0];
  if (curr != NULL && strcmp(curr->key, key) == 0) {
    return curr->value;
  }
  return NULL;
}

void insert(skip_list_t* list, char* key, char* value) {
  s_node_t* curr = list->head;
  s_node_t* updates[MAX_LEVEL + 1];

  for (int i = list->level; i >= 0; i--) {
    while (curr->forward[i] != NULL && strcmp(curr->forward[i]->key, key) < 0) {
      curr = curr->forward[i];
    }
    updates[i] = curr;
  }

  curr = curr->forward[0];

  int level = random_level();

  if (level > list->level) {
    for (int i = list->level + 1; i <= level; i++) {
      updates[i] = list->head;
    }
    list->level = level;
  }

  s_node_t* node = create_node(key, value, level);

  for (int i = 0; i <= level; i++) {
    node->forward[i] = updates[i]->forward[i];
    updates[i]->forward[i] = node;
  }
}