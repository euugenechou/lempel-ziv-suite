#include "word.h"

static Code codes[MAX];
static Symbol syms[MAX];
static Symbol first[MAX];

void wt_init(void) {
  for (uint_fast16_t i = 0; i < ALPHABET; i += 1) {
    syms[i]  = i;
    first[i] = i;
    codes[i] = EMPTY;
  }

  for (uint_fast16_t i = ALPHABET; i < MAX; i += 1) {
    codes[i] = STOP;
  }

  return;
}

void wt_add(Code prev, Code curr, Code next) {
  syms[next]  = first[curr];
  first[next] = first[prev];
  codes[next] = prev;
  return;
}

Code wt_at(Code curr) {
  return codes[curr];
}

bool wt_resolve_code(Code c, Symbol *s) {
  static bool chained = false;
  static Code stack[MAX];
  static Code ptr = 0;

  if (!chained) {
    while (c != EMPTY) {
      stack[ptr++] = c;
      c = codes[c];
    }

    chained = true;
  }

  if (!ptr) {
    return chained = false;
  }

  *s = syms[stack[--ptr]];

  return true;
}
