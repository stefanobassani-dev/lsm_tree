#include "skip_list.h"

#include <stdlib.h>
#include <string.h>

Node* create_node(const char* key, const char* value, int level) {
  Node* node = malloc(sizeof(Node));

  strncpy(node->key, key, 63);
  node->key[63] = '\0';

  strncpy(node->value, value, 127);
  node->key[127] = '\0';

  node->forward = calloc(level + 1, sizeof(Node*));

  if (node->forward == NULL) {
    free(node);
    return NULL;
  }

  return node;
}

SkipList* create_skiplist() {
  SkipList* list = malloc(sizeof(SkipList));

  list->level = 0;

  list->head = create_node("", "", MAX_LEVEL);

  return list;
}

char* search(SkipList* list, char* key, char* value) {
  Node* curr = list->head;

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