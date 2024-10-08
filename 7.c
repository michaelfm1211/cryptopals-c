#include "common.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define KEY "YELLOW SUBMARINE"

int main(void) {
  FILE *file;
  uint8_t *buf;
  size_t len, i;

  file = fopen("7.bin", "r");
  if (!file) {
    perror("7.bin");
    return 1;
  }

  fseek(file, 0, SEEK_END);
  len = ftell(file);
  rewind(file);

  buf = malloc(len);
  fread(buf, len, 1, file);

  aes_load_key((uint8_t *)KEY);
  for (i = 0; i < len; i += 16) {
    printf("%.*s", 16, aes_dec(buf + i));
  }

  return 0;
}
