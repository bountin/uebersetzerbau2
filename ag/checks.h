#include "table.h"

#ifndef CHECKS_H
#define CHECKS_H

void check_variable (char *name, struct symbol *params, struct symbol *vars);
void check_uniqueness (const struct symbol *tbl);

#endif
