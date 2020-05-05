#include "util.h"
#include "word.h"
#include <stdbool.h>

static Code stack[MAX];
static uint32_t ptr = 0;

#define stack_push(x)   (stack[ptr++] = x)
#define stack_pop()     (stack[--ptr])
#define stack_empty()   (!ptr)

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

void wt_backtrack_code(Code curr) {
  Code chain = curr;

  while (chain != EMPTY) {
    stack_push(chain);
    chain = codes[chain];
  }

  return;
}

bool wt_resolve_code(Symbol *s) {
  if (stack_empty()) {
    return false;
  }

  *s = syms[stack_pop()];
  return true;
}
