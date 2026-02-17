#ifndef ENGINE_H
#define ENGINE_H

#define SUCCESS 1
#define NOT_FOUND 0
#define ERROR 0

void engine_init();

void put(const char* key, const char* value);
int get(const char* key, char* result);
void tombstone(const char* key);

#endif