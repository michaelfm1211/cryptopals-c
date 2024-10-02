#include "common.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const double ideal_freqs[] = {
    0.082,  0.015,  0.028, 0.043,  0.127, 0.022,  0.02,  0.061,   0.07,
    0.0015, 0.0077, 0.04,  0.024,  0.067, 0.075,  0.019, 0.00095, 0.06,
    0.063,  0.091,  0.028, 0.0098, 0.024, 0.0015, 0.02,  0.00074};

uint8_t *from_hex(const char *str, size_t *out_len) {
  size_t len;
  uint8_t *buf;
  size_t i;

  len = strlen(str);
  buf = calloc(len / 2 + len % 2, 1);
  if (!buf)
    return NULL;

  for (i = 0; i < len; i++) {
    uint8_t nibble;

    if (str[i] >= '0' && str[i] <= '9') {
      nibble = str[i] - '0';
    } else if (str[i] >= 'a' && str[i] <= 'f') {
      nibble = 10 + str[i] - 'a';
    } else if (str[i] >= 'A' && str[i] <= 'F') {
      nibble = 10 + str[i] - 'A';
    } else {
      return NULL;
    }

    buf[i / 2] |= nibble << 4 * (i % 2 == 0);
  }

  if (out_len)
    *out_len = len / 2 + len % 2;
  return buf;
}

static char nibble_to_hex_char(uint8_t nibble) {
  if (nibble <= 9)
    return nibble + '0';
  return nibble + 'a';
}

char *to_hex(uint8_t *buf, size_t len) {
  char *str;
  size_t i;

  str = malloc(len * 2 + 1);
  if (!buf)
    return NULL;
  str[len * 2] = '\0';

  for (i = 0; i < len; i++) {
    str[i * 2] = nibble_to_hex_char(buf[i] >> 4);
    str[i * 2 + 1] = nibble_to_hex_char(buf[i] & 0xf);
  }

  return str;
}

double score_english_frequency(uint8_t *pt, size_t len) {
  size_t i;
  int freq[26];
  double score;

  bzero(freq, sizeof(freq));
  score = 0;
  for (i = 0; i < len; i++) {
    if (pt[i] < 32) {
      score += 1;
    } else if (pt[i] >= 'A' && pt[i] <= 'Z') {
      freq[pt[i] - 'A'] += 1;
    } else if (pt[i] >= 'a' && pt[i] <= 'z') {
      freq[pt[i] - 'a'] += 1;
    }
  }

  for (i = 0; i < 26; i++) {
    score += fabs(freq[i] - ideal_freqs[i] * len);
  }
  return score;
}
