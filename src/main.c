#include <stdio.h>
#include <string.h>

#include "database.h"


int main(void) {
    char buffer[256];
    char result[256];
    char key[64] = {0};
    char value[128] = {0};
    while (1) {
        printf("LSM > ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        switch (buffer[0]) {
            case 'p':
                if (sscanf(buffer, "p %s %s", key, value) == 2) {
                    write(key, value);
                } else {
                    printf("Invalid format\n");
                }
                break;
            case 'g':
                if (sscanf(buffer, "g %s", key) == 1) {
                    int r = read(result, key);
                    if (r == 1) {
                        printf("%s\n", result);
                    } else {
                        printf("Value not found\n");
                    }
                } else {
                    printf("Invalid format\n");
                }
                break;
            case 'q':
                return 0;
            default:
                printf("comando sconosciuto\n");
        }
    }
    return 0;
}
