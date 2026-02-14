#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#define MAX_LEVEL 3

typedef struct {
  char key[64];
  char value[128];
  Node** forward;
} Node;

typedef struct {
  int level;
  Node* head;
} SkipList;

Node* create_node(char* key, char* value, int level);

SkipList* create_skiplist();

void insert(SkipList*, char*, char*);

#endif
