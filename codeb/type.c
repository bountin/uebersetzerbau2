#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "table.h"
#include "type.h"

struct type* create_type (const char *name, const int depth)
{
	struct type *type = malloc (sizeof (struct type));
	type->name = name;
	type->depth = depth;

	if (depth < 0) {
		printf ("AG: non-positive depth is not allowed (name: %s, depth: %i)\n", name, depth);
		exit (3);
	}

	return type;
}

struct type* get_type (struct symbol *vars, struct symbol *params, const char *name)
{
	while (vars != NULL) {
		if (strcmp (vars->type->name, name) == 0)
			return vars->type;
		vars = vars->next;
	}

	while (params != NULL) {
		if (strcmp (params->type->name, name) == 0)
			return params->type;
		params = params->next;
	}

	printf ("Variable %s not found in get_type\n");
	exit (3);
}
