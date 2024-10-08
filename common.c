#include "common.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wmmintrin.h> /* AES */

static const double ideal_freqs[] = {
    0.082,  0.015,  0.028, 0.043,  0.127, 0.022,  0.02,  0.061,   0.07,
    0.0015, 0.0077, 0.04,  0.024,  0.067, 0.075,  0.019, 0.00095, 0.06,
    0.063,  0.091,  0.028, 0.0098, 0.024, 0.0015, 0.02,  0.00074};

static __m128i aes_key_sched[20];

uint8_t *from_hex(const char *str, size_t *out_len) {
  size_t len;
  uint8_t *buf;
  size_t i;

  len = strlen(str);
  buf = calloc(len / 2 + len % 2, 1);
  if (!buf) {
    perror("calloc()");
    return NULL;
  }

  for (i = 0; i < len; i++) {
    uint8_t nibble;

    if (str[i] >= '0' && str[i] <= '9') {
      nibble = str[i] - '0';
    } else if (str[i] >= 'a' && str[i] <= 'f') {
      nibble = 10 + str[i] - 'a';
    } else if (str[i] >= 'A' && str[i] <= 'F') {
      nibble = 10 + str[i] - 'A';
    } else {
      fprintf(stderr,
              "from_hex(): Invalid character '%c' (0x%x) at position %zu\n",
              str[i], str[i], i);
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
  return nibble - 10 + 'a';
}

char *to_hex(uint8_t *buf, size_t len) {
  char *str;
  size_t i;

  str = malloc(len * 2 + 1);
  if (!buf) {
    perror("malloc()");
    return NULL;
  }
  str[len * 2] = '\0';

  for (i = 0; i < len; i++) {
    str[i * 2] = nibble_to_hex_char(buf[i] >> 4);
    str[i * 2 + 1] = nibble_to_hex_char(buf[i] & 0xf);
  }

  return str;
}

uint8_t *xor_encdec(uint8_t *buf, size_t buflen, uint8_t *key, size_t keylen) {
  uint8_t *ct;
  size_t i;

  ct = malloc(buflen);
  memcpy(ct, buf, buflen);
  for (i = 0; i < buflen; i++) {
    ct[i] ^= key[i % keylen];
  }
  return ct;
}

static __m128i _aes_key_expand(__m128i key, __m128i keygened) {
  keygened = _mm_shuffle_epi32(keygened, _MM_SHUFFLE(3, 3, 3, 3));
  key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
  key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
  key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
  return _mm_xor_si128(key, keygened);
}
#define aes_key_expand(key, rcon)                                              \
  _aes_key_expand(key, _mm_aeskeygenassist_si128(key, rcon))

void aes_load_key(uint8_t *key) {
  aes_key_sched[0] = _mm_loadu_si128((const __m128i *)key);
  aes_key_sched[1] = aes_key_expand(aes_key_sched[0], 0x01);
  aes_key_sched[2] = aes_key_expand(aes_key_sched[1], 0x02);
  aes_key_sched[3] = aes_key_expand(aes_key_sched[2], 0x04);
  aes_key_sched[4] = aes_key_expand(aes_key_sched[3], 0x08);
  aes_key_sched[5] = aes_key_expand(aes_key_sched[4], 0x10);
  aes_key_sched[6] = aes_key_expand(aes_key_sched[5], 0x20);
  aes_key_sched[7] = aes_key_expand(aes_key_sched[6], 0x40);
  aes_key_sched[8] = aes_key_expand(aes_key_sched[7], 0x80);
  aes_key_sched[9] = aes_key_expand(aes_key_sched[8], 0x1B);
  aes_key_sched[10] = aes_key_expand(aes_key_sched[9], 0x36);

  aes_key_sched[11] = _mm_aesimc_si128(aes_key_sched[9]);
  aes_key_sched[12] = _mm_aesimc_si128(aes_key_sched[8]);
  aes_key_sched[13] = _mm_aesimc_si128(aes_key_sched[7]);
  aes_key_sched[14] = _mm_aesimc_si128(aes_key_sched[6]);
  aes_key_sched[15] = _mm_aesimc_si128(aes_key_sched[5]);
  aes_key_sched[16] = _mm_aesimc_si128(aes_key_sched[4]);
  aes_key_sched[17] = _mm_aesimc_si128(aes_key_sched[3]);
  aes_key_sched[18] = _mm_aesimc_si128(aes_key_sched[2]);
  aes_key_sched[19] = _mm_aesimc_si128(aes_key_sched[1]);
}

uint8_t *aes_dec(uint8_t *buf) {
  __m128i m;
  uint8_t *out;

  m = _mm_loadu_si128((__m128i *)buf);
  m = _mm_xor_si128(m, aes_key_sched[10]);
  m = _mm_aesdec_si128(m, aes_key_sched[11]);
  m = _mm_aesdec_si128(m, aes_key_sched[12]);
  m = _mm_aesdec_si128(m, aes_key_sched[13]);
  m = _mm_aesdec_si128(m, aes_key_sched[14]);
  m = _mm_aesdec_si128(m, aes_key_sched[15]);
  m = _mm_aesdec_si128(m, aes_key_sched[16]);
  m = _mm_aesdec_si128(m, aes_key_sched[17]);
  m = _mm_aesdec_si128(m, aes_key_sched[18]);
  m = _mm_aesdec_si128(m, aes_key_sched[19]);
  m = _mm_aesdeclast_si128(m, aes_key_sched[0]);

  out = malloc(16);
  _mm_storeu_si128((__m128i *)out, m);
  return out;
}

double score_english_frequency(uint8_t *pt, size_t len) {
  size_t i;
  int freq[26], space_cnt;
  double score;

  bzero(freq, sizeof(freq));
  score = 0;
  space_cnt = 0;
  for (i = 0; i < len; i++) {
    if (pt[i] < 32) {
      score += 1;
    } else if (pt[i] >= 'A' && pt[i] <= 'Z') {
      freq[pt[i] - 'A'] += 1;
    } else if (pt[i] >= 'a' && pt[i] <= 'z') {
      freq[pt[i] - 'a'] += 1;
    } else if (pt[i] == ' ') {
      space_cnt += 1;
    }
  }

  for (i = 0; i < 26; i++) {
    score += fabs(freq[i] - ideal_freqs[i] * len);
  }
  // average word length is 4.8, so we should have 1 space per 4.8 chars
  score += fabs(space_cnt - len / 4.8);
  return score;
}

int hamming_dist(uint8_t *buf1, size_t len1, uint8_t *buf2, size_t len2) {
  int diff;
  size_t i;

  if (len1 < len2) {
    size_t temp;

    temp = len1;
    len1 = len2;
    len2 = temp;
  }

  diff = 0;
  for (i = 0; i < len2; i++) {
    diff += __builtin_popcount(buf1[i] ^ buf2[i]);
  }
  diff += (len1 - len2) * 8;
  return diff;
}
