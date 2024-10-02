#include "common.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IN \
  "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736"

struct pt {
  uint8_t *pt;
  double score;
};

uint8_t *xor(uint8_t *ct, size_t len, uint8_t key) {
  uint8_t *pt;
  size_t i;

  pt = malloc(len);
  memcpy(pt, ct, len);
  for (i = 0; i < len; i++)
    pt[i] ^= key;
  return pt;
}

int compare_pts(struct pt *a, struct pt *b) {
  return (int)(a->score - b->score);
}

int main(void) {
  uint8_t *buf;
  size_t buflen, i;
  struct pt pts[128];

  buf = from_hex(IN, &buflen);
  for (i = 0; i < 128; i++) {
    pts[i].pt = xor(buf, buflen, i);
    pts[i].score = score_english_frequency(pts[i].pt, buflen);
  }
  qsort(pts, 128, sizeof(struct pt),
        (int (*)(const void *, const void *))compare_pts);

  /* for (i = 0; i < 128; i++) { */
  /*   printf("%.*s\n", (int)buflen, pts[i].pt); */
  /* } */
  printf("%.*s\n", (int)buflen, pts[0].pt);

  return 0;
}
