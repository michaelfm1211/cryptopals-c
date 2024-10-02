#include "common.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define IN "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d"

int main(void) {
  uint8_t *buf;
  size_t buflen;

  buf = from_hex(IN, &buflen);

  fwrite(buf, 1, buflen, stdout);

  free(buf);
  return 0;
}
