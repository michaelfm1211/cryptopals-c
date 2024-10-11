#include "common.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define IN "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"

int cmp_blocks(uint8_t *a, uint8_t *b) {
  return memcmp(a, b, 16);
}

// length of output is len+16
uint8_t *encryption_oracle(uint8_t *buf, size_t len) {
  uint8_t *out;
  int pre_len, post_len;

  aes_load_key(rand_16bytes());

  pre_len = (rand() % 6) + 5;
  post_len = 16 - pre_len;
  out = malloc(pre_len + len + post_len);
  memset(out, 'A', pre_len);
  memcpy(out + pre_len, buf, len);
  memset(out, 'Z', post_len);

  if (rand() > RAND_MAX / 2) {
    printf("encryption oracle: CBC\n");
    out = aes_cbc_enc(out, len + 16, rand_16bytes());
  } else {
    printf("encryption oracle: ECB\n");
    out = aes_ecb_enc(out, len + 16);
  }

  return out;
}

// Returns 1 for ECB, 0 for CBC`
int ct_ecb_or_cbc(uint8_t *buf, size_t len) {
  size_t i;

  qsort(buf, len / 16, 16, (qsort_cmp_t)cmp_blocks);
  for (i = 16; i < len; i += 16) {
    if (!memcmp(buf + i - 16, buf + i, 16)) {
      return 1;
    }
  }
  return 0;
}

int main(void) {
  srand(time(NULL));

  if (ct_ecb_or_cbc(encryption_oracle((uint8_t *)IN, sizeof(IN) - 1),
                    sizeof(IN) + 15)) {
    printf("ECB\n");
  } else {
    printf("CBC\n");
  }

  return 0;
}
