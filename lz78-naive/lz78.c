#include "io.h"
#include "trie.h"
#include "word.h"
#include <stdio.h>

void encode(int infile, int outfile) {
  TrieNode *root = trie_create();
  TrieNode *curr_node = root;
  TrieNode *prev_node = NULL;
  Symbol curr_sym = 0;
  Symbol prev_sym = 0;
  Code next_code = START;

  while (read_sym(infile, &curr_sym)) {
    TrieNode *next_node = trie_step(curr_node, curr_sym);

    if (next_node) {
      prev_node = curr_node;
      curr_node = next_node;
    } else {
      buffer_pair(outfile, curr_node->code, curr_sym, bitwidth(next_code));
      curr_node->children[curr_sym] = trie_node_create(next_code++);
      curr_node = root;
    }

    if (next_code == MAX) {
      trie_reset(root);
      curr_node = root;
      next_code = START;
    }

    prev_sym = curr_sym;
  }

  if (curr_node != root) {
    buffer_pair(outfile, prev_node->code, prev_sym, bitwidth(next_code));
    next_code = (next_code + 1) % MAX;
  }

  buffer_pair(outfile, STOP, 0, bitwidth(next_code));
  flush_pairs(outfile);
  trie_delete(root);
  return;
}

void decode(int infile, int outfile) {
  WordTable *wt = wt_create();
  Symbol curr_sym = 0;
  Code curr_code = 0;
  Code next_code = START;

  while (read_pair(infile, &curr_code, &curr_sym, bitwidth(next_code))) {
    wt[next_code] = word_append_sym(wt[curr_code], curr_sym);
    buffer_word(outfile, wt[next_code++]);

    if (next_code == MAX) {
      wt_reset(wt);
      next_code = START;
    }
  }

  flush_words(outfile);
  wt_delete(wt);
  return;
}
