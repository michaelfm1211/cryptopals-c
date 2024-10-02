/*
 * NOTE: delete line 94 in the input file. It is too short and not the answer
 */

#include "common.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LINES 326

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

struct pt get_best_pt(uint8_t *buf, size_t len) {
  int i;
  struct pt pts[128];
  
  for (i = 0; i < 128; i++) {
    pts[i].pt = xor(buf, len, i);
    pts[i].score = score_english_frequency(pts[i].pt, len);
  }
  qsort(pts, 128, sizeof(struct pt),
      (int (*)(const void *, const void *))compare_pts);
  return pts[0];
}

int main(void) {
  FILE *file;
  size_t i, buflen;
  struct pt pts[INPUT_LINES];

  file = fopen("4.txt", "r");
  if (!file) {
    perror("4.txt");
    return 1;
  }

  for (i = 0; i < INPUT_LINES; i++) {
    char str[61];
    uint8_t *buf;

    fread(str, 1, 61, file);
    str[59] = '\0';
    buf = from_hex(str, &buflen);
    pts[i] = get_best_pt(buf, buflen);
  }
  qsort(pts, INPUT_LINES, sizeof(struct pt),
      (int (*)(const void *, const void *))compare_pts);

  printf("%.*s\n", (int)buflen, pts[0].pt);
  printf("score: %f\n", pts[0].score);
  return 0;
}
