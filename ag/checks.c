#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "checks.h"

void check_variable (char *name, struct symbol *params, struct symbol *vars) {
	if (table_has_symbol (params, name) || table_has_symbol (vars, name)) {
		return;
	} else {
		printf("Variable '%s' not found.\n", name);
		exit(3);
	}
}

void check_uniqueness (const struct symbol *tbl)
{
	while (tbl != NULL) {
		if (table_has_symbol (tbl->next, tbl->type->name)) {
			printf ("AG error: %s is not unique\n", tbl->type->name);
			exit (3);
		}
		tbl = tbl->next;
	}
}
