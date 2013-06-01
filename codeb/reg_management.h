#ifndef REG_MANAGEMENT_H
#define REG_MANAGEMENT_H

#include "table.h"

#define REG_MAX 8

char *get_8reg (char *r);

void reg_init (struct symbol *params);
char *newreg (void);
void freereg (char *reg);

char *newvarreg (void);
void set_var_reg (char *r);

int reg_is_tmp (char *r);
int reg_is_param (char *r);

#endif
