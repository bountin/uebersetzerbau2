#ifndef TABLE_H
#define TABLE_H

struct symbol {
	struct symbol *next;
	char *name;
};

struct symbol* tbl_add_symbol (struct symbol *table, char *name);
int table_has_symbol (struct symbol *table, char *name);
struct symbol* tbl_merge (struct symbol *A, struct symbol *B);
void tbl_print (struct symbol *table);

#endif
