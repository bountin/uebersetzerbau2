#ifndef TYPE_H
#define TYPE_H

struct type {
	const char *name;
	int depth;
};

struct type* create_type (const char *name, const int depth);

#endif
