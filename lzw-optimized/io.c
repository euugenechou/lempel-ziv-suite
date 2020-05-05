#include "endian.h"
#include "io.h"
#include "util.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

uint64_t total_syms = 0;
uint64_t total_bits = 0;

static int syms = 0;
static uint8_t symbuf[BLOCK] = { 0 };

static int bits = 0;
static uint8_t bitbuf[BLOCK] = { 0 };

void read_header(int fd, FileHeader *fh) {
  read_bytes(fd, (uint8_t *)fh, sizeof(FileHeader));

  if (big_endian()) {
    fh->magic = swap32(fh->magic);
    fh->protection = swap16(fh->protection);
  }

  total_bits += (BYTE * sizeof(FileHeader));
  return;
}

void write_header(int fd, FileHeader *fh) {
  total_bits += (BYTE * sizeof(FileHeader));

  if (big_endian()) {
    fh->magic = swap32(fh->magic);
    fh->protection = swap16(fh->protection);
  }

  write_bytes(fd, (uint8_t *)fh, sizeof(FileHeader));
  return;
}

bool read_sym(int fd, Symbol *s) {
  static int end = -1;

  if (syms == 0) {
    int bytes = read_bytes(fd, symbuf, BLOCK);

    if (bytes != BLOCK) {
      end = bytes + 1;
    }
  }

  *s = symbuf[syms];
  syms = (syms + 1) % BLOCK;

  if (syms != end) {
    total_syms += 1;
  }

  return syms != end;
}

void write_code(int fd, Code c, int width) {
  total_bits += width;

  if (big_endian()) {
    c = swap32(c);
  }

  for (int i = 0; i < width; i += 1) {
    if (c & (1 << i)) {
      bitbuf[bits / BYTE] |= (1 << (bits % BYTE));
    }

    bits += 1;

    if (bits / BYTE == BLOCK) {
      write_bytes(fd, bitbuf, BLOCK);
      memset(bitbuf, 0, BLOCK);
      bits = 0;
    }
  }

  return;
}

void flush_codes(int fd) {
  if (bits) {
    write_bytes(fd, bitbuf, bytes(bits));
  }

  return;
}

bool read_code(int fd, Code *c, int width) {
  total_bits += width;

  *c = 0;

  for (int i = 0; i < width; i += 1) {
    if (!bits) {
      read_bytes(fd, bitbuf, BLOCK);
    }

    if (bitbuf[bits / BYTE] & (1 << (bits % BYTE))) {
      *c |= (1 << i);
    }

    bits = (bits + 1) % (BLOCK * BYTE);
  }

  if (big_endian()) {
    *c = swap32(*c);
  }

  return *c != STOP;
}

void write_word(int fd, Code c) {
  Symbol s = 0;

  while (wt_resolve_code(c, &s)) {
    symbuf[syms++] = s;

    if (syms == BLOCK) {
      write_bytes(fd, symbuf, BLOCK);
      syms = 0;
    }

    total_syms += 1;
  }

  return;
}

void flush_words(int fd) {
  if (syms) {
    write_bytes(fd, symbuf, syms);
  }

  return;
}
