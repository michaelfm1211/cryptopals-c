#include "common.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define IN                                                                     \
  "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736"

struct pt {
  uint8_t *pt;
  double score;
};

int compare_pts(struct pt *a, struct pt *b) {
  return (int)(a->score - b->score);
}

int main(void) {
  uint8_t *buf;
  size_t buflen, i;
  struct pt pts[128];

  buf = from_hex(IN, &buflen);
  for (i = 0; i < 128; i++) {
    pts[i].pt = xor_encdec(buf, buflen, (uint8_t *)&i, 1);
    pts[i].score = score_english_frequency(pts[i].pt, buflen);
  }
  qsort(pts, 128, sizeof(struct pt), (qsort_cmp_t)compare_pts);

  printf("%.*s\n", (int)buflen, pts[0].pt);
  return 0;
}
