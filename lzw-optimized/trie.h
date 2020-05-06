#ifndef __TRIE_H__
#define __TRIE_H__

#include "util.h"

void trie_init(void);

void trie_add(Code curr, Symbol s, Code next);

Code trie_step(Code curr, Symbol s);

#endif
