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

void check_depth (const struct type *type, const int depth)
{
	if (type->depth != depth) {
		printf ("AG error: Unexpected depth of %d (Exp:%d) - %s\n", type->depth, depth, type->name);
		exit (3);
	}
}

void check_depth_not_zero (const struct type *type)
{
	if (type->depth == 0) {
		printf ("AG error: Expected depth of 0, got %d (%s)\n", type->depth, type->name);
		exit (3);
	}
}
