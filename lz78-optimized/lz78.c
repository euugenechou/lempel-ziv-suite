#include "io.h"
#include "trie.h"
#include "util.h"
#include "word.h"

void encode(int infile, int outfile) {
  trie_init();
  Symbol prev_sym = 0;
  Symbol curr_sym = 0;
  Code temp_code = 0;
  Code prev_code = 0;
  Code curr_code = EMPTY;
  Code next_code = START;

  while (read_sym(infile, &curr_sym)) {
    if ((temp_code = trie_step(curr_code, curr_sym))) {
      prev_code = curr_code;
      curr_code = temp_code;
    } else {
      buffer_pair(outfile, curr_code, curr_sym, bitwidth(next_code));
      trie_add(curr_code, curr_sym, next_code++);
      curr_code = EMPTY;
    }

    if (next_code == MAX) {
      trie_init();
      curr_code = EMPTY;
      next_code = START;
    }

    prev_sym = curr_sym;
  }

  if (curr_code != EMPTY) {
    buffer_pair(outfile, prev_code, prev_sym, bitwidth(next_code++));
    next_code = (next_code == MAX) ? START : next_code;
  }

  buffer_pair(outfile, STOP, 0, bitwidth(next_code));
  flush_pairs(outfile);
  return;
}

void decode(int infile, int outfile) {
  wt_init();
  Symbol curr_sym = 0;
  Code curr_code = 0;
  Code next_code = START;

  while (read_pair(infile, &curr_code, &curr_sym, bitwidth(next_code))) {
    wt_add(curr_code, curr_sym, next_code++);
    buffer_word(outfile, curr_code, curr_sym);

    if (next_code == MAX) {
      wt_init();
      next_code = START;
    }
  }

  flush_words(outfile);
  return;
}
