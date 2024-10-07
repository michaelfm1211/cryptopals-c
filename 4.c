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

int compare_pts(struct pt *a, struct pt *b) {
  return (int)(a->score - b->score);
}

struct pt get_best_pt(uint8_t *buf, size_t len) {
  int i;
  struct pt pts[128];

  for (i = 0; i < 128; i++) {
    pts[i].pt = xor_encdec(buf, len, (uint8_t *)&i, 1);
    pts[i].score = score_english_frequency(pts[i].pt, len);
  }
  qsort(pts, 128, sizeof(struct pt), (qsort_cmp_t)compare_pts);
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
    uint8_t *newline_ch;

    fread(str, 1, 61, file);
    newline_ch = (uint8_t *)strchr(str, '\n');
    if (!newline_ch) {
      fprintf(stderr, "No newline on line %zu\n", i);
    }
    if ((ptrdiff_t)newline_ch - (ptrdiff_t)str < 60) {
      size_t foff;

      foff = ftell(file);
      fseek(file, foff - 60 + (ptrdiff_t)newline_ch - (ptrdiff_t)str, SEEK_SET);
    }
    *newline_ch = '\0';

    buf = from_hex(str, &buflen);
    if (buf == NULL) {
      return 1;
    }
    pts[i] = get_best_pt(buf, buflen);
  }
  qsort(pts, INPUT_LINES, sizeof(struct pt), (qsort_cmp_t)compare_pts);

  printf("%.*s\n", (int)buflen, pts[0].pt);
  printf("score: %f\n", pts[0].score);
  return 0;
}
