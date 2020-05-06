#ifndef __IO_H__
#define __IO_H__

#include "util.h"
#include "word.h"

extern uint64_t total_syms;
extern uint64_t total_bits;

typedef struct FileHeader {
  uint32_t magic;
  uint16_t protection;
} FileHeader;

void read_header(int fd, FileHeader *header);

void write_header(int fd, FileHeader *header);

bool read_sym(int fd, Symbol *s);

void buffer_code(int fd, Code c, int width);

void flush_codes(int fd);

bool read_code(int fd, Code *c, int width);

void buffer_word(int fd, Word *w);

void flush_words(int fd);

#endif
