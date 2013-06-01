#ifndef TABLE_H
#define TABLE_H

#include "common.h"

struct symbol {
	struct symbol *next;
	struct type *type;
	char *reg;
};

#include "type.h"

struct symbol *table_add_symbol (struct symbol *table, struct type *type);
int table_has_symbol (struct symbol *table, const char *name);
void table_print (struct symbol *table);
char *table_find_reg (char *name, struct symbol *table);

#endif
