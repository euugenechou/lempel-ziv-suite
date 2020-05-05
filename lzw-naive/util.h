#ifndef __UTIL_H__
#define __UTIL_H__

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>

#define MAGIC         0xdeadcafe

#define ALPHABET      256
#define BLOCK         4096
#define BYTE          8

#define STOP          256
#define START         257
#define MAX           (1 << 16)

typedef uint8_t   Symbol;
typedef uint32_t  Code;

uint64_t bytes(uint64_t bits);

int bitwidth(Code n);

int read_bytes(int fd, uint8_t *buf, int nbytes);

int write_bytes(int fd, uint8_t *buf, int nbytes);

void check(bool cond, char *fmt, ...);

#endif
