#ifndef __IO_H__
#define __IO_H__

#include "io.h"
#include "word.h"

extern uint64_t total_syms;
extern uint64_t total_bits;

typedef struct FileHeader {
  uint32_t magic;
  uint16_t protection;
} FileHeader;

void read_header(int fd, FileHeader *fh);

void write_header(int fd, FileHeader *fh);

bool read_sym(int fd, Symbol *s);

void buffer_pair(int fd, Code c, Symbol s, int width);

void flush_pairs(int fd);

bool read_pair(int fd, Code *c, Symbol *s, int width);

void buffer_word(int fd, Word *w);

void flush_words(int fd);

#endif

