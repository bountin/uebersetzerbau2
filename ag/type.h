#ifndef TYPE_H
#define TYPE_H

#include "table.h"

struct type {
	const char *name;
	int depth;
};

struct type* create_type (const char *name, const int depth);
struct type* get_type (struct symbol *vars, struct symbol *params, const char *name);

#endif
