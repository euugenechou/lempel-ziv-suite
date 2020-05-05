#ifndef __UTIL_H__
#define __UTIL_H__

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>

#define ALPHABET      256

#define BYTE          8
#define HALFWORD      16
#define BLOCK         4096

#define STOP_CODE     256
#define START_CODE    257
#define MAX_CODE      UINT16_MAX

#define MAGIC         0xdeadcafe

uint64_t bytes(uint64_t bits);

uint8_t bitwidth(uint16_t n);

void check(bool cond, char *fmt, ...);

#endif
