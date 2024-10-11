#pragma once

#include <stddef.h>
#include <stdint.h>

typedef int (*qsort_cmp_t)(const void *, const void *);

uint8_t *from_hex(const char *str, size_t *out_len);
char *to_hex(uint8_t *buf, size_t len);

uint8_t *xor_encdec(uint8_t *buf, size_t buflen, uint8_t *key, size_t keylen);
void aes_load_key(uint8_t *key);
uint8_t *aes_enc(uint8_t *buf);
uint8_t *aes_dec(uint8_t *buf);
uint8_t *aes_ecb_enc(uint8_t *buf, size_t len);
uint8_t *aes_ecb_dec(uint8_t *buf, size_t len);
uint8_t *aes_cbc_enc(uint8_t *buf, size_t len, uint8_t *iv);
uint8_t *aes_cbc_dec(uint8_t *buf, size_t len, uint8_t *iv);

uint8_t *pkcs7_pad(uint8_t *buf, size_t len, size_t padlen);
uint8_t *rand_16bytes(void);

double score_english_frequency(uint8_t *pt, size_t len);
int hamming_dist(uint8_t *buf1, size_t len1, uint8_t *buf2, size_t len2);
