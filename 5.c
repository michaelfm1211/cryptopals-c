#include "common.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  uint8_t *ct;
  size_t len;

  if (argc != 3) {
    fprintf(stderr, "usage: %s message key\n", argv[0]);
    return 1;
  }

  len = strlen(argv[1]);
  ct = xor_encdec((uint8_t *)argv[1], len, (uint8_t *)argv[2], strlen(argv[2]));
  printf("%s\n", to_hex(ct, len));

  return 0;
}
