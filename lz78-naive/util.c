#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define IS_POW_2(x)   (!(x & (x - 1)))

uint64_t bytes(uint64_t bits) {
  return ((bits % 8) || (!bits)) ? bits / 8 + 1 : bits / 8;
}

int bitwidth(Code c) {
  static int width = 2;

  if (c == START) {
    width = 2;
  } else if (IS_POW_2(c)) {
    width += 1;
  }

  return width;
}

int read_bytes(int fd, uint8_t *buf, int nbytes) {
  int bytes = 0;
  int total = 0;

  do {
    bytes = read(fd, buf + total, nbytes - total);
    total += bytes;
  } while (bytes && total != nbytes);

  return total;
}

int write_bytes(int fd, uint8_t *buf, int nbytes) {
  int bytes = 0;
  int total = 0;

  do {
    bytes = write(fd, buf + total, nbytes - total);
    total += bytes;
  } while (bytes && total != nbytes);

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
