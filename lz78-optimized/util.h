#ifndef __UTIL_H__
#define __UTIL_H__

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>

#define MAGIC     0x8badbeef

#define ALPHABET  256
#define BLOCK     4096
#define BYTE      8

#define STOP      0
#define EMPTY     1
#define START     2
#define MAX       (1 << 16)

typedef uint8_t Symbol;
typedef uint32_t Code;

uint64_t bytes(uint64_t bits);

int bitwidth(Code c);

int read_bytes(int fd, uint8_t *buf, int nbytes);

int write_bytes(int fd, uint8_t *buf, int nbytes);

void check(bool cond, char *fmt, ...);

#endif
