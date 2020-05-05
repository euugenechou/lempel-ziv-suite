#include "util.h"
#include <stdbool.h>

static Code codes[MAX];     // Each code points to the code it succeeds.
static Symbol syms[MAX];    // Holds the symbol that was appended to each code.
static Symbol first[MAX];   // Holds the first symbol of each word.

void wt_init(void) {
  for (Code i = 0; i < ALPHABET; i += 1) {
    syms[i]  = i;
    first[i] = i;
    codes[i] = EMPTY;
  }

  for (Code i = ALPHABET; i < MAX; i += 1) {
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

  return *s = syms[stack[--ptr]];
}
