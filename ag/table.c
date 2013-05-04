#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "table.h"

struct symbol* tbl_add_symbol (struct symbol *table, char *name) {
	struct symbol *s = (struct symbol *) malloc (sizeof(struct symbol));

	if (table_has_symbol (table, name)) {
		printf("AG ERROR: Tried to declare a symbol twice.\n");
		exit(3);
	}

	s->name = name;
	s->next = table;

	return s;
}

int table_has_symbol (struct symbol *table, char *name) {
	struct symbol *s = table;

	if (s == (struct symbol *)NULL) /* empty tbl haz no symbol */
		return 0;

	if (strcmp (s->name, name) == 0)
		return 1;

	while (s->next != (struct symbol *)NULL) {
		s = s->next;
		if (strcmp (s->name, name) == 0)
			return 1;
	}

	return 0;
}

struct symbol* tbl_merge (struct symbol *A, struct symbol *B) {
	struct symbol *s = B;
	struct symbol *R = A;

	if (B == (struct symbol *)NULL) /* B haz no elementz */ 
		return A;

	R = tbl_add_symbol (R, B->name);

	while (s->next != (struct symbol *)NULL) {
		s = s->next;
		R = tbl_add_symbol (R, s->name);
	}

	return R;
}

void tbl_print (struct symbol *table) {
	struct symbol *s = table;

	printf("Tbl dump:\n");

	if (table == (struct symbol *) NULL) {
		printf ("Empty\n\n");
		return;
	}

	while (s->next != (struct symbol *)NULL) {
		printf ("%s\n", s->name);
		s = s->next;
	}
	printf ("%s\n\n", s->name);
}
