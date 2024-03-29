#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "reg_management.h"

int get_reg_number (char *r)
{
	int i=0;
	for(;i<REG_MAX;i++) {
		if (strcmp (reg[i], r) == 0) {
			return i;
		}
	}

	printf("D'oh! register not found: %s\n", r);
	exit(3);
}

void reg_init (struct symbol* paras)
{
	// The first 6 may be used by parameters
	char *arg_regs[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
	char *other_regs[] = {"rax", "r11"};
	int i=0, j=0;

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
	for (i=0; i < REG_MAX; i++) {
		if (reg[i] == NULL) { break; }
		if (reg_usage[i] == 0) {
			reg_usage[i] = 1;
			return reg[i];
		}
	}

	printf("\nNo more registers available :(\n");
	exit(3);
}

void freereg (char *r)
{
	int nr = get_reg_number(r);
	reg_usage[nr]--;
}

int reg_is_tmp (char *r)
{
	int i;
	for (i=0; i<REG_MAX;i++) {
		if (reg[i] == NULL) { return 0; }
		if (strcmp (reg[i], r) == 0) {
			return 1;
		}
	}
	return 0;
}

int reg_is_param (char *r)
{
	return !reg_is_tmp(r);
}
