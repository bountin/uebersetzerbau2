#ifndef REG_MANAGEMENT_H
#define REG_MANAGEMENT_H

#include "table.h"

#define REG_MAX 8

struct reginfo {
	char *reg[REG_MAX];
	int reg_usage[REG_MAX];
	int reg_var[REG_MAX];
};

struct reginfo *cur_reg;
struct reginfo *reg_switch (struct reginfo *);

char *get_8reg (char *r);

void reg_init (struct symbol *params);
char *newreg (void);
void freereg (char *reg);

char *newvarreg (void);
void set_var_reg (char *r);

int reg_is_tmp (char *r);
int reg_is_param (char *r);

void free_local_vars (struct sym_bucket *table);

int get_reg_usage (char *r);

#endif
