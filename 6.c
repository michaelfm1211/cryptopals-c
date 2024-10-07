#include "common.h"
#include <stdio.h>
#include <stdlib.h>

#define START_KEYSIZE 2
#define NUM_KEYSIZES 39
#define CHECK_KEYSIZES 12

struct hdist_entry {
  double hdist;
  size_t keylen;
};

struct pt {
  uint8_t *pt;
  double score;
  uint8_t key;
};

int compare_hdists(struct hdist_entry *a, struct hdist_entry *b) {
  return (a->hdist > b->hdist) - (a->hdist < b->hdist);
}

int compare_pts(struct pt *a, struct pt *b) {
  return (a->score > b->score) - (a->score < b->score);
}

uint8_t *try_keysize(uint8_t *buf, size_t buflen, size_t keylen) {
  size_t i;
  uint8_t transposes[keylen][buflen / keylen + 1];
  size_t transpose_lens[keylen];
  uint8_t *key;

  key = malloc(keylen);

  // transpose
  for (i = 0; i < keylen; i++) {
    size_t j;

    for (j = i; j < buflen; j += keylen) {
      transposes[i][j / keylen] = buf[j];
    }
    transpose_lens[i] = j / keylen + !(j % keylen == 0);
  }

  // break a single-char XOR for each transpose
  for (i = 0; i < keylen; i++) {
    size_t j;
    struct pt pts[128];

    for (j = 0; j < 128; j++) {
      pts[j].pt =
          xor_encdec(transposes[i], transpose_lens[i], (uint8_t *)&j, 1);
      pts[j].score = score_english_frequency(pts[j].pt, transpose_lens[i]);
      pts[j].key = j;
    }
    qsort(pts, 128, sizeof(struct pt), (qsort_cmp_t)compare_pts);
    key[i] = pts[0].key;
  }

  return key;
}

int main(void) {
  FILE *file;
  size_t len, i;
  uint8_t *buf;
  struct hdist_entry norm_hdists[NUM_KEYSIZES];
  struct pt pts[CHECK_KEYSIZES];

  file = fopen("6.bin", "r");
  fseek(file, 0, SEEK_END);
  len = ftell(file);
  rewind(file);

  buf = malloc(len);
  fread(buf, len, 1, file);

  for (i = START_KEYSIZE; i < START_KEYSIZE + NUM_KEYSIZES; i++) {
    size_t j;

    for (j = 0; j < len / i - 1; j++) {
      norm_hdists[i - START_KEYSIZE].hdist +=
          (double)hamming_dist(buf + i * j, i, buf + (j + 1) * i, i) / i;
    }
    norm_hdists[i - START_KEYSIZE].hdist /= (double)(len) / (double)i - 1;

    norm_hdists[i - START_KEYSIZE].keylen = i;
  }
  qsort(norm_hdists, NUM_KEYSIZES, sizeof(struct hdist_entry),
        (qsort_cmp_t)compare_hdists);

  for (i = 0; i < CHECK_KEYSIZES; i++) {
    uint8_t *key, *dec;

    key = try_keysize(buf, len, norm_hdists[i].keylen);

    dec = xor_encdec(buf, len, key, norm_hdists[i].keylen);
    pts[i].pt = dec;
    pts[i].score = score_english_frequency(dec, len);
    pts[i].key = norm_hdists[i].keylen;
  }
  qsort(pts, CHECK_KEYSIZES, sizeof(struct pt), (qsort_cmp_t)compare_pts);
  printf("\n");
  printf("BEST score: %f, keylen: %d\n", pts[0].score, pts[0].key);
  printf("%.*s\n", (int)len, pts[0].pt);

  return 0;
}
