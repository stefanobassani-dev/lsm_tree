#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memtable.h"
#include "sstable.h"

void write_formatted_record(FILE* fd, const char* key, const char* value) {
  char record_buffer[192];
  memset(record_buffer, 0, 192);  // Pulisci l'intero buffer

  // Copia la chiave e assicurati il terminatore alla posizione 63
  strncpy(record_buffer, key, 63);
  record_buffer[63] = '\0';

  // Copia il valore e assicurati il terminatore alla posizione 191
  strncpy(record_buffer + 64, value, 127);
  record_buffer[64 + 127] = '\0';  // Indice 191

  fwrite(record_buffer, 192, 1, fd);
}

int main() {
  char file1_name[] = "sstable_test_0001.bin";
  char file2_name[] = "sstable_test_0002.bin";
  char merge_output_name[] = "sstable_test_merged.bin";

  FILE* fd1 = fopen(file1_name, "wb");
  if (fd1 == NULL) {
    perror("Error creating file1");
    return EXIT_FAILURE;
  }
  write_formatted_record(fd1, "key1", "value1_old");
  write_formatted_record(fd1, "key2", "value2_old");
  fclose(fd1);
  printf("Created '%s'\n", file1_name);

  // 3. Creazione del secondo file di input (file più recente)
  FILE* fd2 = fopen(file2_name, "wb");
  if (fd2 == NULL) {
    perror("Error creating file2");
    return EXIT_FAILURE;
  }
  write_formatted_record(fd2, "key1", "value1_new");  // Sovrascrive "key1"
  write_formatted_record(fd2, "key3", "value3_new");  // Nuova chiave
  fclose(fd2);
  printf("Created '%s'\n", file2_name);

  // 4. Chiamata alla funzione two_way_marge
  two_way_marge(file1_name, file2_name, merge_output_name);
  printf("Merge completed. Output written to '%s'.\n", merge_output_name);

  return EXIT_SUCCESS;
}