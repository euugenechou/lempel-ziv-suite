#ifndef __WORD_H__
#define __WORD_H__

#include "util.h"
#include <inttypes.h>
#include <stdbool.h>

void wt_init(void);

void wt_add(Code prev, Code curr, Code next);

Code wt_at(Code curr);

void wt_backtrack_code(Code curr);

bool wt_resolve_code(Symbol *s);

#endif
