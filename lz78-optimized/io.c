#include "endian.h"
#include "io.h"
#include "word.h"
#include <string.h>

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

  if (!syms) {
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

void buffer_pair(int fd, Code c, Symbol s, int width) {
  total_bits += BYTE + width;

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

  for (int i = 0; i < BYTE; i += 1) {
    if (s & (1 << i)) {
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

void flush_pairs(int fd) {
  if (bits) {
    write_bytes(fd, bitbuf, bytes(bits));
  }

  return;
}

bool read_pair(int fd, Code *c, Symbol *s, int width) {
  total_bits += BYTE + width;

  *c = 0;
  *s = 0;

  for (int i = 0; i < width; i += 1) {
    if (!bits) {
      read_bytes(fd, bitbuf, BLOCK);
    }

    if (bitbuf[bits / BYTE] & (1 << (bits % BYTE))) {
      *c |= (1 << i);
    }

    bits = (bits + 1) % (BLOCK * BYTE);
  }

  for (int i = 0; i < BYTE; i += 1) {
    if (!bits) {
      read_bytes(fd, bitbuf, BLOCK);
    }

    if (bitbuf[bits / BYTE] & (1 << (bits % BYTE))) {
      *s |= (1 << (i % BYTE));
    }

    bits = (bits + 1) % (BLOCK * BYTE);
  }

  if (big_endian()) {
    *c = swap16(*c);
  }

  return *c != STOP;
}

void buffer_word(int fd, Code c, Symbol s) {
  Symbol t = 0;

  while (wt_resolve_code(c, &t)) {
    symbuf[syms++] = t;

    if (syms == BLOCK) {
      write_bytes(fd, symbuf, BLOCK);
      syms = 0;
    }

    total_syms += 1;
  }

  symbuf[syms++] = s;

  if (syms == BLOCK) {
    write_bytes(fd, symbuf, BLOCK);
    syms = 0;
  }

  total_syms += 1;
  return;
}

void flush_words(int fd) {
  if (syms) {
    write_bytes(fd, symbuf, syms);
  }

  return;
}
