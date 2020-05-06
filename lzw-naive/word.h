#ifndef __WORD_H__
#define __WORD_H__

#include "util.h"

typedef struct Word {
  Symbol *syms;
  uint32_t len;
} Word;

typedef Word * WordTable;

Word *word_create(Symbol *syms, uint32_t len);

Word *word_append_sym(Word *w, Symbol s);

void word_delete(Word *w);

WordTable *wt_create(void);

void wt_reset(WordTable *wt);

void wt_delete(WordTable *wt);

#endif
