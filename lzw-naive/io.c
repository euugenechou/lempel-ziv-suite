#include "endian.h"
#include "io.h"
#include "util.h"
#include "word.h"
#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>

uint64_t total_syms = 0;
uint64_t total_bits = 0;

static uint8_t sym_buf[BLOCK] = { 0 };
static uint16_t sym_cnt = 0;
static uint16_t sym_buf_end = UINT16_MAX;

static uint8_t bit_buf[BLOCK] = { 0 };
static uint16_t bit_cnt = 0;

int read_bytes(int infile, uint8_t *buf, int to_read) {
  int bytes_to_read = to_read;
  int bytes_read = 0;
  int total_read = 0;

  do {
    bytes_read = read(infile, buf + total_read, bytes_to_read);
    check(bytes_read != -1, "Failed to read from input file.\n");
    bytes_to_read -= bytes_read;
    total_read += bytes_read;
  } while (bytes_read && total_read != to_read);

  return total_read;
}

int write_bytes(int outfile, uint8_t *buf, int to_write) {
  int bytes_to_write = to_write;
  int bytes_written = 0;
  int total_written = 0;

  do {
    bytes_written = write(outfile, buf + total_written, bytes_to_write);
    check(bytes_written != -1, "Failed to write to output file.\n");
    bytes_to_write -= bytes_written;
    total_written += bytes_written;
  } while (bytes_written && total_written != to_write);

  return total_written;
}

void read_header(int infile, FileHeader *header) {
  read_bytes(infile, (uint8_t *) header, sizeof(FileHeader));

  if (big_endian()) {
    header->magic = swap32(header->magic);
    header->protection = swap16(header->protection);
  }

  total_bits += (BYTE * sizeof(FileHeader));
  return;
}

void write_header(int outfile, FileHeader *header) {
  total_bits += (BYTE * sizeof(FileHeader));

  if (big_endian()) {
    header->magic = swap32(header->magic);
    header->protection = swap16(header->protection);
  }

  write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader));
  return;
}

bool read_sym(int infile, uint8_t *sym) {
  if (!sym_cnt) {
    int bytes_read = read_bytes(infile, sym_buf, BLOCK);
    sym_buf_end = (bytes_read == BLOCK) ? sym_buf_end : bytes_read + 1;
  }

  *sym = sym_buf[sym_cnt];
  sym_cnt = (sym_cnt + 1) % BLOCK;
  total_syms = (sym_cnt != sym_buf_end) ? total_syms + 1 : total_syms;

  return sym_cnt != sym_buf_end;
}

void buffer_code(int outfile, uint16_t code, uint8_t bit_len) {
  total_bits += bit_len;

  if (big_endian()) {
    code = swap16(code);
  }

  for (uint8_t i = 0; i < bit_len; i += 1) {
    if (code & (1 << (i % HALFWORD))) {
      bit_buf[bit_cnt / BYTE] |= (1 << (bit_cnt % BYTE));
    } else {
      bit_buf[bit_cnt / BYTE] &= ~(1 << (bit_cnt % BYTE));
    }

    bit_cnt += 1;

    if (bit_cnt / BYTE == BLOCK) {
      write_bytes(outfile, bit_buf, BLOCK);
      bit_cnt = 0;
    }
  }

  return;
}

void flush_codes(int outfile) {
  if (bit_cnt) {
    write_bytes(outfile, bit_buf, bytes(bit_cnt));
  }

  return;
}

bool read_code(int infile, uint16_t *code, uint8_t bit_len) {
  total_bits += bit_len;
  *code = 0;

  for (uint8_t i = 0; i < bit_len; i += 1) {
    if (!bit_cnt) {
      read_bytes(infile, bit_buf, BLOCK);
    }

    if (bit_buf[bit_cnt / BYTE] & (1 << (bit_cnt % BYTE))) {
      *code |= (1 << (i % HALFWORD));
    } else {
      *code &= ~(1 << (i % HALFWORD));
    }

    bit_cnt = (bit_cnt + 1) % (BLOCK * BYTE);
  }

  if (big_endian()) {
    *code = swap16(*code);
  }

  return *code != STOP_CODE;
}

void buffer_word(int outfile, Word *w) {
  total_syms += w->len;

  for (uint32_t i = 0; i < w->len; i += 1) {
    sym_buf[sym_cnt++] = w->syms[i];

    if (sym_cnt == BLOCK) {
      write_bytes(outfile, sym_buf, BLOCK);
      sym_cnt = 0;
    }
  }

  return;
}

void flush_words(int outfile) {
  if (sym_cnt) {
    write_bytes(outfile, sym_buf, sym_cnt);
  }

  return;
}
