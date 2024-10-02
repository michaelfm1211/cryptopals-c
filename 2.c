#include "common.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define IN1 "1c0111001f010100061a024b53535009181c"
#define IN2 "686974207468652062756c6c277320657965"

int main(void) {
  uint8_t *buf1, *buf2, *buf3;
  size_t buflen, i;
  char *hexstr;

  buf1 = from_hex(IN1, &buflen);
  buf2 = from_hex(IN2, NULL);

  buf3 = malloc(buflen);
  for (i = 0; i < buflen; i++) {
    buf3[i] = buf1[i] ^ buf2[i];
  }

  hexstr = to_hex(buf3, buflen);
  printf("%s\n", hexstr);

  return 0;
}

