#ifndef CLI_H
#define CLI_H

#include <stddef.h>

void run_cli();

void cli_put(char* input, char* key, char* value);
void cli_get(char* input, char* key, char* result, size_t result_size);
void cli_delete(char* input, char* key);

#endif