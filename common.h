#pragma once

#include <stddef.h>
#include <stdint.h>

uint8_t *from_hex(const char *str, size_t *out_len);
char *to_hex(uint8_t *buf, size_t len);

double score_english_frequency(uint8_t *pt, size_t len);
