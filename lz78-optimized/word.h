#ifndef __WORD_H__
#define __WORD_H__

#include "util.h"

void wt_init(void);

void wt_add(Code code, Symbol s, Code next);

bool wt_resolve_code(Code c, Symbol *s);

#endif

