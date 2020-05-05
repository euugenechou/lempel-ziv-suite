#ifndef __IO_H__
#define __IO_H__

#include "word.h"
#include <inttypes.h>
#include <stdbool.h>

extern uint64_t total_syms;
extern uint64_t total_bits;

typedef struct FileHeader {
  uint32_t magic;
  uint16_t protection;
} FileHeader;

int read_bytes(int infile, uint8_t *buf, int to_read);

int write_bytes(int outfile, uint8_t *buf, int to_write);

void read_header(int infile, FileHeader *header);

void write_header(int outfile, FileHeader *header);

bool read_sym(int infile, uint8_t *byte);

void buffer_code(int outfile, uint16_t code, uint8_t bit_len);

void flush_codes(int outfile);

bool read_code(int infile, uint16_t *code, uint8_t bit_len);

void buffer_word(int outfile, Word *w);

void flush_words(int outfile);

#endif

