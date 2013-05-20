#ifndef TABLE_H
#define TABLE_H

struct symbol {
	struct symbol *next;
	struct type *type;
	char *reg;
};

#include "type.h"

struct symbol *tbl_add_symbol (struct symbol *table, struct type *type);
int table_has_symbol (struct symbol *table, const char *name);
void table_print (struct symbol *table);
char *tbl_find_reg (char *name, struct symbol *table);

#endif
