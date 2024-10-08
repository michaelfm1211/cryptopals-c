#include "common.h"
#include <stdio.h>
#include <string.h>

#define IN "YELLOW SUBMARINE"
#define EXPECT "YELLOW SUBMARINE\x04\x04\x04\x04"

int main(void) {
  uint8_t *buf;

  buf = pkcs7_pad((uint8_t *)IN, sizeof(IN) - 1, 20);
  if (memcmp(buf, EXPECT, 20)) {
    printf("pkcs7_pad does not work\n");
    return 1;
  }

  printf("pkcs7_pad works\n");
  return 0;
}
