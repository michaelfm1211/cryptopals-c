#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define KEY "YELLOW SUBMARINE"

int main(void) {
  FILE *file;
  size_t len;
  uint8_t *buf, *dec;
  uint8_t iv[16];

  file = fopen("10.bin", "r");
  if (!file) {
    perror("10.bin");
    return 1;
  }

  fseek(file, 0, SEEK_END);
  len = ftell(file);
  rewind(file);

  buf = malloc(len);
  fread(buf, len, 1, file);

  aes_load_key((uint8_t *)KEY);
  bzero(iv, 16);
  dec = aes_cbc_dec(buf, len, iv);
  printf("%.*s\n", (int)len, dec);

  return 0;
}
