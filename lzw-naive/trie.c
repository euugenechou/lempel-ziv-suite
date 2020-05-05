#include "trie.h"
#include "util.h"
#include <stdlib.h>

TrieNode *trie_node_create(uint16_t code) {
  TrieNode *n = (TrieNode *) calloc(1, sizeof(TrieNode));
  check(n, "Failed to calloc() TrieNode.");

  n->code = code;
  return n;
}

void trie_node_delete(TrieNode *n) {
  free(n);
  return;
}

TrieNode *trie_create(void) {
  TrieNode *root = trie_node_create(0);

  for (uint16_t i = 0; i < ALPHABET; i += 1) {
    root->children[i] = trie_node_create(i);
  }

  return root;
}

void trie_reset(TrieNode *root) {
  for (uint16_t i = 0; i < ALPHABET; i += 1) {
    for (uint16_t j = 0; j < ALPHABET; j += 1) {
      if (root->children[i]->children[j]) {
        trie_delete(root->children[i]->children[j]);
        root->children[i]->children[j] = NULL;
      }
    }
  }

  return;
}

void trie_delete(TrieNode *n) {
  if (n) {
    for (uint16_t i = 0; i < ALPHABET; i += 1) {
      trie_delete(n->children[i]);
    }

    trie_node_delete(n);
    n = NULL;
  }

  return;
}

TrieNode *trie_step(TrieNode *n, uint8_t sym) {
  return n->children[sym];
}
