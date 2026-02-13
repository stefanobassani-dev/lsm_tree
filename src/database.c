#include "database.h"

#include <stdio.h>
#include <string.h>

FILE* open_file() {
    FILE *fd = fopen(DB_FILE, "a");
    if (fd == NULL) {
        perror("Cannot open file");
        return NULL;
    }

    return fd;
}

void write(char *key, char *value) {
    FILE *fd = open_file();

    fprintf(fd, "%s,%s\n", key, value);
    fclose(fd);
}

int read(char *result, char *key) {
    FILE *fd = open_file();

    int found = 0;

    char buffer[256] = {0};
    while (fgets(buffer, sizeof(buffer), fd) != NULL) {
        char *ptr = strchr(buffer, ',');
        if (ptr != NULL) {
            *ptr = '\0';

            char *fileKey = buffer;
            char *fileValue = ptr + 1;

            fileValue[strcspn(fileValue, "\n")] = 0;

            if (strcmp(key, fileKey) == 0) {
                strcpy(result, fileValue);
                found = 1;
            }
        }
    }

    return found;
}