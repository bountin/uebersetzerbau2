#ifndef TABLE_H
#define TABLE_H

#include "common.h"

struct symbol {
	struct symbol *next;
	struct type *type;
	char *reg;
};

struct sym_bucket {
	struct symbol *elem;
	struct sym_bucket *next;
};

#include "type.h"

struct symbol *table_add_symbol (struct symbol *table, struct type *type);
struct sym_bucket *table_add_symbol_p (struct sym_bucket *table, struct symbol *elem);
int table_has_symbol (struct symbol *table, const char *name);
void table_print (struct symbol *table);
struct symbol *table_find_symbol (const char *name, struct symbol *table);

#endif
