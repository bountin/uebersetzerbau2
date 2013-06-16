#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "reg_management.h"

struct reginfo *reg_switch (struct reginfo *info)
{
	if (info == NULL) {
		info = malloc (sizeof (struct reginfo));
	}

	cur_reg = info;
	return info;
}

int get_reg_number (char *r)
{
	int i=0;
	char **reg = cur_reg->reg;
	for(;i<REG_MAX;i++) {
		if (reg[i] == NULL)
			continue;
		if (strcmp (reg[i], r) == 0) {
			return i;
		}
	}

	printf("D'oh! register not found: %s\n", r);
	exit(3);
}

char *get_8reg (char *r)
{
	if (strcmp (r, "rax") == 0)
		return "al";
	else if (strcmp (r, "r11") == 0)
		return "r11b";
	else if (strcmp (r, "rdi") == 0)
		return "dil";
	else if (strcmp (r, "rsi") == 0)
		return "sil";
	else if (strcmp (r, "rdx") == 0)
		return "dl";
	else if (strcmp (r, "rcx") == 0)
		return "cl";
	else if (strcmp (r, "r8") == 0)
		return "r8b";
	else if (strcmp (r, "r9") == 0)
		return "r9b";
	else
		printf("Unknown register: get_8reg(%s)\n", r);
}

void reg_init (struct symbol* paras)
{
	// The first 6 may be used by parameters
	char *arg_regs[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
	char *other_regs[] = {"rax", "r11"};
	int i=0, j=0;
	char **reg = cur_reg->reg;
	int *reg_usage = cur_reg->reg_usage;

	while (paras != NULL) {
		i++;
		paras = paras->next;
	}

	for (;j < REG_MAX; j++) {
		reg[j] = NULL;
		reg_usage[j] = 0;
	}

	reg[0] = "rax";
	reg[1] = "r11";

	j = 2;
	while (i < 6) {
		reg[j++] = arg_regs[i++];
	}
}

char *newreg (void)
{
	int i;
	char **reg = cur_reg->reg;
	int *reg_var = cur_reg->reg_var;
	int *reg_usage = cur_reg->reg_usage;
	for (i=0; i < REG_MAX; i++) {
		if (reg[i] == NULL) { break; }
		if (reg_var[i]) { continue; }
		if (reg_usage[i] == 0) {
			reg_usage[i] = 1;
			printf ("# newreg: %s\n", reg[i]);
			return reg[i];
		}
	}

	printf("\nNo more registers available :(\n");
	exit(3);
}

char *newvarreg (void)
{
	char *r = newreg ();
	set_var_reg (r);
	printf ("# was a var\n");
	return r;
}

void set_var_reg (char *r)
{
	cur_reg->reg_var[get_reg_number (r)] = 1;
}
void clear_var_reg (char *r)
{
	cur_reg->reg_var[get_reg_number (r)] = 0;
}

void freereg (char *r)
{
	int nr = get_reg_number(r);
	cur_reg->reg_usage[nr]--;
	printf ("#freereg %s\n", r);
}

int reg_is_tmp (char *r)
{
	int i;
	char **reg = cur_reg->reg;
	for (i=0; i<REG_MAX;i++) {
		if (reg[i] == NULL) { return 0; }
		if (strcmp (reg[i], r) == 0) {
			return (!cur_reg->reg_var[i]);
		}
	}
	return 0;
}

int reg_is_param (char *r)
{
	return !reg_is_tmp(r);
}

void free_local_vars (struct sym_bucket *table)
{
	while (table != NULL) {
		freereg (table->elem->reg);
		clear_var_reg (table->elem->reg);
		table = table->next;
	}
}

int get_reg_usage (char *r)
{
	int id;
	if (reg_is_param (r))
		return 1;

	id = get_reg_number (r);
	return cur_reg->reg_usage[id];
}
