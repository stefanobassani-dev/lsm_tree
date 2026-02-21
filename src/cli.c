#include "cli.h"

#include <stdio.h>

#include "engine.c"

void run_cli() {
  char input[256];

  char key[64] = {0};
  char value[128] = {0};

  char result[256];

  while (1) {
    memset(key, 0, sizeof(key));
    memset(value, 0, sizeof(value));

    printf("LSM > ");
    if (fgets(input, sizeof(input), stdin) == NULL) break;

    switch (input[0]) {
      case 'p':
        cli_put(input, key, value);
        break;
      case 'g':
        cli_get(input, key, result, sizeof(result));
        break;
      case 'd':
        cli_delete(input, key);
        break;
      case 'q':
        return;
      default:
        printf("Invalid command\n");
    }
  }
}

void cli_put(char* input, char* key, char* value) {
  if (sscanf(input, "p %63s %127s", key, value) == 2) {
    put(key, value);
  } else {
    printf("Invalid format\n");
  }
}

void cli_get(char* input, char* key, char* result, size_t result_size) {
  memset(result, 0, result_size);
  if (sscanf(input, "g %63s", key) == 1) {
    int status = get(key, result);
    if (status == SUCCESS) {
      printf("Value: %s\n", result);
    } else {
      printf("Value not found\n");
    }
  } else {
    printf("Invalid format\n");
  }
}

void cli_delete(char* input, char* key) {
  if (sscanf(input, "d %63s", key) == 1) {
    tombstone(key);
  } else {
    printf("Invalid format\n");
  }
}