#include "io.h"
#include "trie.h"
#include "util.h"
#include "word.h"

void encode(int infile, int outfile) {
  trie_init();
  Symbol s = 0;
  Code prev = EMPTY;
  Code curr = EMPTY;
  Code next = START;

  while (read_sym(infile, &s)) {
    if ((curr = trie_step(prev, s)) != STOP) {
      prev = curr;
    } else {
      write_code(outfile, prev, bitwidth(next));
      trie_add(prev, s, next++);
      prev = s;
    }

    if (next == MAX) {
      trie_init();
      prev = s;
      next = START;
    }
  }

  if (prev != EMPTY) {
    write_code(outfile, prev, bitwidth(next++));
    next = (next == MAX) ? START : next;
  }

  write_code(outfile, STOP, bitwidth(next));
  flush_codes(outfile);
  return;
}

void decode(int infile, int outfile) {
  wt_init();
  Code prev = STOP;
  Code curr = EMPTY;
  Code next = START;

  while (read_code(infile, &curr, bitwidth(next + 1))) {
    if (prev != STOP) {
      if (wt_at(curr) != STOP) {
        wt_add(prev, curr, next++);
      } else {
        wt_add(prev, prev, next++);
      }
    }

    write_word(outfile, curr);
    prev = curr;

    if (next + 1 == MAX) {
      wt_init();
      prev = STOP;
      next = START;
    }
  }

  flush_words(outfile);
  return;
}
