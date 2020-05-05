#include "util.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

uint64_t bytes(uint64_t bits) {
  return ((bits % 8) || (!bits)) ? bits / 8 + 1 : bits / 8;
}

uint8_t bitwidth(Code c) {
  uint8_t width = 1;

  while ((c >>= 1)) {
    width += 1;
  }

  return width;
}

int read_bytes(int infile, uint8_t *buf, int to_read) {
  int bytes = 0;
  int total = 0;

  do {
    bytes = read(infile, buf + total, to_read - total);
    total += bytes;
  } while (bytes && total != to_read);

  return total;
}

int write_bytes(int outfile, uint8_t *buf, int to_write) {
  int bytes = 0;
  int total = 0;

  do {
    bytes = write(outfile, buf + total, to_write - total);
    total += bytes;
  } while (bytes && total != to_write);

  return total;
}

void check(bool cond, char *fmt, ...) {
  if (!cond) {
    va_list arg;
    va_start(arg, fmt);
    vfprintf(stderr, fmt, arg);
    va_end(arg);
    exit(EXIT_FAILURE);
  }

  return;
}
