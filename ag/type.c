#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "type.h"

struct type* create_type (const char *name, const int depth)
{
	struct type *type = malloc (sizeof (struct type));
	type->name = name;
	type->depth = depth;

	return type;
}
