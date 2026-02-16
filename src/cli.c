#include <stdio.h>

#include "engine.c"

void run_cli() {
  char input[256];

  char key[64] = {0};
  char value[128] = {0};

  char result[256];

  while (1) {
    printf("LSM > ");
    if (fgets(input, sizeof(input), stdin) == NULL) break;
    switch (input[0]) {
      case 'p':
        if (sscanf(input, "p %s %s", key, value) == 2) {
          put(key, value);
        } else {
          printf("Invalid format\n");
        }
        break;
      case 'g':
        if (sscanf(input, "g %s", key) == 1) {
          int status = get(key, result);
          if (status == SUCCESS) {
            printf("Value: %s\n", result);
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