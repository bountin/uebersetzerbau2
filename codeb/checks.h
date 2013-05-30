#ifndef CHECKS_H
#define CHECKS_H

#include "type.h"
#include "table.h"

void check_variable (char *name, struct symbol *params, struct symbol *vars);
void check_uniqueness (const struct symbol *tbl);
void check_depth (const struct type *type, const int depth);
void check_depth_not_zero (const struct type *type);

#endif
