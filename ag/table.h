#ifndef TABLE_H
#define TABLE_H

#include "type.h"

struct symbol {
	struct symbol *next;
	struct type *type;
};

struct symbol* tbl_add_symbol (struct symbol *table, struct type *type);
int table_has_symbol (struct symbol *table, const char *name);
struct symbol* tbl_merge (struct symbol *A, struct symbol *B);
void tbl_print (struct symbol *table);

#endif
