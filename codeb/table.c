#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "table.h"

struct symbol* table_add_symbol (struct symbol *table, struct type *type) {
	struct symbol *s = malloc (sizeof(struct symbol));

	s->type = type;
	s->next = table;

	return s;
}

int table_has_symbol (struct symbol *table, const char *name) {
	struct symbol *s = table;

	if (s == (struct symbol *)NULL) /* empty table haz no symbol */
		return 0;

	if (strcmp (s->type->name, name) == 0)
		return 1;

	while (s->next != (struct symbol *)NULL) {
		s = s->next;
		if (strcmp (s->type->name, name) == 0)
			return 1;
	}

	return 0;
}

void table_print (struct symbol *table) {
	struct symbol *s = table;

	printf("# Table dump:\n");

	if (table == (struct symbol *) NULL) {
		printf ("# Empty\n\n");
		return;
	}

	while (s->next != (struct symbol *)NULL) {
		printf ("# %s (D:%d)\n", s->type->name, s->type->depth);
		s = s->next;
	}
	printf ("# %s (D:%d)\n\n", s->type->name, s->type->depth);
}

struct symbol *table_find_symbol (const char *name, struct symbol *s)
{
	while (s != NULL) {
		if (strcmp (s->type->name, name) == 0) {
			return s;
		}
		s = s->next;
	}

	return NULL;
}
