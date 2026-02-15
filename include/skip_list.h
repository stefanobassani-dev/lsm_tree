#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#define MAX_LEVEL 3

typedef struct s_node_t {
  char key[64];
  char value[128];
  struct s_node_t** forward;
} s_node_t;

typedef struct skip_list_t {
  int level;
  s_node_t* head;
} skip_list_t;

int random_level();

s_node_t* create_node(const char* key, const char* value, int level);

skip_list_t* create_skiplist();

char* search(skip_list_t* list, char* key);

void insert(skip_list_t*, char* key, char* value);

#endif
