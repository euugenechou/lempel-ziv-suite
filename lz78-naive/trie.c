#include "trie.h"
#include <stdlib.h>

TrieNode *trie_node_create(Code c) {
  TrieNode *n = (TrieNode *) calloc(1, sizeof(TrieNode));
  check(n, "Failed to calloc() TrieNode.\n");

  n->code = c;
  return n;
}

void trie_node_delete(TrieNode *n) {
  free(n);
  return;
}

TrieNode *trie_create(void) {
  return trie_node_create(EMPTY);
}

void trie_reset(TrieNode *root) {
  for (uint_fast16_t i = 0; i < ALPHABET; i += 1) {
    if (root->children[i]) {
      trie_delete(root->children[i]);
      root->children[i] = NULL;
    }
  }

  return;
}

void trie_delete(TrieNode *n) {
  if (n) {
    for (uint_fast16_t i = 0; i < ALPHABET; i += 1) {
      trie_delete(n->children[i]);
    }

    trie_node_delete(n);
    n = NULL;
  }

  return;
}

TrieNode *trie_step(TrieNode *n, Symbol s) {
  return n->children[s];
}
