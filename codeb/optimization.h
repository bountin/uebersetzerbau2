#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include "code_gen.h"

struct code *optimize_immediate (struct code *code, int l_imm, int r_imm);

#endif
