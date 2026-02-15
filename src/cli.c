#include <stdio.h>

#include "skip_list.h"

void run_cli() {
  char buffer[256];
  char result[256];
  char key[64] = {0};
  char value[128] = {0};

  skip_list_t* list = create_skiplist();

  while (1) {
    printf("LSM > ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
    switch (buffer[0]) {
      case 'p':
        if (sscanf(buffer, "p %s %s", key, value) == 2) {
          insert(list, key, value);
        } else {
          printf("Invalid format\n");
        }
        break;
      case 'g':
        if (sscanf(buffer, "g %s", key) == 1) {
          char* res = search(list, key);
          if (res != NULL) {
            printf("%s\n", res);
          } else {
            printf("Value not found\n");
          }
        } else {
          printf("Invalid format\n");
        }
        break;
      case 'q':
        return;
      default:
        printf("Invalid command\n");
    }
  }
}