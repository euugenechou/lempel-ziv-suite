#include "word.h"
#include <string.h>

static Code codes[MAX];
static Symbol syms[MAX];

void wt_init(void) {
  memset(syms, 0, MAX);
  memset(codes, 0, MAX);
  return;
}

void wt_add(Code curr, Symbol s, Code next) {
  syms[next] = s;
  codes[next] = curr;
  return;
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
