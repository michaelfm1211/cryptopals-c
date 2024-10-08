#include "common.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 320
#define NUM_LINES 204

int block_cmp(const char *a, const char *b) {
  return memcmp(a, b, 32);
}

int main(void) {
  FILE *file;
  size_t i;

  file = fopen("8.txt", "r");
  if (!file) {
    perror("8.txt");
    return 1;
  }

  for (i = 0; i < NUM_LINES; i++) {
    char line[LINE_LEN + 1], line_copy[LINE_LEN + 1];
    size_t j;

    fread(line, 1, LINE_LEN + 1, file);
    line[LINE_LEN] = '\0';
    memcpy(line_copy, line, LINE_LEN + 1);

    qsort(line, LINE_LEN / 32, 32, (qsort_cmp_t)block_cmp);
    for (j = 32; j < LINE_LEN; j += 32) {
      if (!memcmp(line + j - 32, line + j, 32)) {
        printf("%.*s\n", 32, line + j - 32);
        printf("%s\n", line_copy);
        return 0;
      }
    }
  }

  printf("no result\n");
  return 1;
}
