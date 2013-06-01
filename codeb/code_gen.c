#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "code_gen.h"

void execute_iburg (struct code* code)
{
	burm_label (code);
	burm_reduce (code, 1);
}

void not_supported (char *production)
{
	printf ("Tried to compile an unsupported command: %s\n", production);
	exit (3);
}

static long get_control_label_id (void)
{
	static long id = 0;

	return id++;
}

struct code* create_code (int type, struct code *left_child, struct code *right_child)
{
	struct code *c = malloc (sizeof (struct code));

	OP_LABEL (c)    = type;
	LEFT_CHILD (c)  = left_child;
	RIGHT_CHILD (c) = right_child;

	return c;
}

struct code *create_code_num (long number)
{
	struct code *c = create_code (TT_NUM, NULL, NULL);
	c->val = number;
	return c;
}

struct code* create_code_var (char *name, struct symbol *params, struct symbol *vars)
{
	// --XXX-- THIS CODE ONLY(!) CHECKS PARAMS FOR CODEA
	char *reg;
	struct code *c;

	check_variable (name, params, vars);
	reg = table_find_reg (name, params);
	c = create_code (TT_VARIABLE, NULL, NULL);
	c->name = strdup (name);
	c->reg = reg;
	return c;
}

struct code* create_code_if (struct code *condition)
{
	struct code *c;

	c = create_code (TT_IF, condition, NULL);
	c->val = get_control_label_id ();

	return c;
}

struct code* create_code_while (struct code *condition)
{
	struct code *c;

	c = create_code (TT_WHILE, condition, NULL);
	c->val = get_control_label_id ();

	return c;
}

struct code *create_code_nop ()
{
	return create_code (TT_NOP, NULL, NULL);
}

struct symbol *gen_para_regs (struct symbol *parameters)
{
	char *registers[6] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
	int i=0;
	struct symbol *para_start = parameters;

	while (parameters != NULL) {
		if (i > 5) {
			not_supported ("more than 6 function parameters\n");
		}
		parameters->reg = strdup (registers[i++]);
		parameters = parameters->next;
	}

	#ifdef MY_DEBUG
	printf ("#-- ASSIGNING PARAMETER REGISTERS\n");
	table_print (para_start);
	printf ("\n");
	#endif

	return para_start;
}

void code_print (struct code *c)
{
	printf ("# - CODE INFO\n");
	printf ("#  op: %i\n#  val: %li\n#  name: %s\n#  reg: %s\n#  LC: %p\n#  RC: %p\n", c->op, c->val, c->name, c->reg, LEFT_CHILD(c), RIGHT_CHILD(c));
}
