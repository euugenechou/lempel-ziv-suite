#include "trie.h"
#include <string.h>

static Code table[MAX][ALPHABET];

void trie_init(void) {
  memset(table, 0, sizeof(table));
  return;
}

void trie_add(Code curr, Symbol s, Code next) {
  table[curr][s] = next;
  return;
}

Code trie_step(Code curr, Symbol s) {
  return table[curr][s];
}
