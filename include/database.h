#ifndef DATABASE_H
#define DATABASE_H

#define DB_FILE "database.db"

void write(char *key, char *value);
int read(char *result, char *key);

#endif