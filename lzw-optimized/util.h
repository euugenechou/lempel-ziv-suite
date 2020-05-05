#ifndef __UTIL_H__
#define __UTIL_H__

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>

#define MAGIC     0xdeadcafe

#define ALPHABET  256
#define BLOCK     4096
#define BYTE      8

#define STOP      256
#define EMPTY     257
#define START     258
#define MAX       (1 << 16)

typedef uint8_t Symbol;
typedef uint32_t Code;

uint64_t bytes(uint64_t bits);

uint8_t bitwidth(Code c);

int read_bytes(int infile, uint8_t *buf, int to_read);

int write_bytes(int infile, uint8_t *buf, int to_read);

void check(bool cond, char *fmt, ...);

#endif
