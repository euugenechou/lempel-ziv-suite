#include "word.h"
#include <stdlib.h>
#include <string.h>

Word *word_create(Symbol *syms, uint32_t len) {
  Word *w = (Word *)malloc(sizeof(Word));
  check(w, "Failed to malloc() word.\n");

  w->len = len;
  w->syms = (Symbol *)malloc(len * sizeof(Symbol));
  check(w->syms, "Failed to malloc() word byte array.\n");

  memcpy(w->syms, syms, len);
  return w;
}

Word *word_append_sym(Word *w, Symbol s) {
  Word *appended = (Word *)malloc(sizeof(Word));
  check(appended, "Failed to malloc() word.\n");

  appended->len = w->len + 1;
  appended->syms = (Symbol *)malloc(appended->len * sizeof(Symbol));
  check(appended->syms, "Failed to malloc() word byte array.\n");

  memcpy(appended->syms, w->syms, w->len);
  appended->syms[w->len] = s;
  return appended;
}

void word_delete(Word *w) {
  free(w->syms);
  w->syms = NULL;
  free(w);
  return;
}

WordTable *wt_create(void) {
  WordTable *wt = (WordTable *) calloc(MAX, sizeof(Word *));
  check(wt, "Failed to calloc() word table");

  for (uint_fast16_t i = 0; i < ALPHABET; i += 1) {
    wt[i] = word_create((Symbol *)&i, 1);
  }

  return wt;
}

void wt_reset(WordTable *wt) {
  for (uint_fast32_t i = ALPHABET; i < MAX; i += 1) {
    if (wt[i]) {
      word_delete(wt[i]);
      wt[i] = NULL;
    }
  }

  return;
}

void wt_delete(WordTable *wt) {
  for (uint_fast32_t i = 0; i < MAX; i += 1) {
    if (wt[i]) {
      word_delete(wt[i]);
      wt[i] = NULL;
    }
  }

  free(wt);
  return;
}
