#include "io.h"
#include "trie.h"
#include "word.h"

void encode(int infile, int outfile) {
  TrieNode *root = trie_create();
  TrieNode *curr_node = root;
  Symbol curr_sym = 0;
  Code next_code = START;

  while (read_sym(infile, &curr_sym)) {
    TrieNode *next_node = trie_step(curr_node, curr_sym);

    if (next_node) {
      curr_node = next_node;
    } else {
      buffer_code(outfile, curr_node->code, bitwidth(next_code));
      curr_node->children[curr_sym] = trie_node_create(next_code++);
      curr_node = root->children[curr_sym];
    }

    if (next_code == MAX) {
      trie_reset(root);
      curr_node = root->children[curr_sym];
      next_code = START;
    }
  }

  if (curr_node != root) {
    buffer_code(outfile, curr_node->code, bitwidth(next_code++));

    if (next_code == MAX) {
      next_code = START;
    }
  }

  buffer_code(outfile, STOP, bitwidth(next_code));
  flush_codes(outfile);
  trie_delete(root);
  return;
}

void decode(int infile, int outfile) {
  WordTable *wt = wt_create();
  Code curr_code = 0;
  Code prev_code = STOP;
  Code next_code = START;

  while (read_code(infile, &curr_code, bitwidth(next_code + 1))) {
    Word *curr_word = wt[curr_code];

    if (prev_code != STOP) {
      Word *prev_word = wt[prev_code];

      if (curr_word) {
        wt[next_code++] = word_append_sym(prev_word, curr_word->syms[0]);
      } else {
        curr_word = word_append_sym(prev_word, prev_word->syms[0]);
        wt[next_code++] = curr_word;
      }
    }

    buffer_word(outfile, curr_word);
    prev_code = curr_code;

    if (next_code + 1 == MAX) {
      wt_reset(wt);
      prev_code = STOP;
      next_code = START;
    }
  }

  flush_words(outfile);
  wt_delete(wt);
  return;
}
