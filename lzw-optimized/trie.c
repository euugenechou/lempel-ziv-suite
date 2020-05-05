#include "trie.h"
#include "util.h"
#include <stdint.h>

static Code trie[MAX][ALPHABET];

void trie_init(void) {
  for (uint32_t i = 0; i < MAX; i += 1) {
    for (uint16_t j = 0; j < ALPHABET; j += 1) {
      if (i == EMPTY) {
        trie[i][j] = j;
      } else {
        trie[i][j] = STOP;
      }
    }
  }

  return;
}

void trie_add(Code curr, Symbol sym, Code next) {
  trie[curr][sym] = next;
  return;
}

Code trie_step(Code prev, Symbol sym) {
  return trie[prev][sym];
}
