#ifndef __TRIE_H__
#define __TRIE_H__

#include "util.h"

void trie_init(void);

void trie_add(Code p, Symbol s, Code n);

Code trie_step(Code c, Symbol s);

#endif
