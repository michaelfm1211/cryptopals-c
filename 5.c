#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t *repeat_xor(uint8_t *msg, size_t msglen, uint8_t *key, size_t keylen) {
  uint8_t *ct;
  size_t i;

  ct = malloc(msglen);
  memcpy(ct, msg, msglen);
  for (i = 0; i < msglen; i++) {
    ct[i] ^= key[i % keylen];
  }
  return ct;
}

int main(int argc, char **argv) {
  uint8_t *ct;
  size_t len;

  if (argc != 3) {
    fprintf(stderr, "usage: %s message key\n", argv[0]);
    return 1;
  }

  len = strlen(argv[1]);
  ct = repeat_xor((uint8_t *)argv[1], len, (uint8_t *)argv[2], strlen(argv[2]));
  printf("%s\n", to_hex(ct, len));

  return 0;
}
