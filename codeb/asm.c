#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "asm.h"
#include "reg_management.h"

#define MY_DEBUG

void asm_func_head (char *func_name)
{
	printf(".globl %1$s\n.type %1$s, @function\n%1$s:\n", func_name);
}

void asm_mov (char *src, char *dest)
{
	#ifdef MY_DEBUG
	printf("\t# asm_mov(%s, %s)\n", src, dest);
	#endif

	if (strcmp (src, dest)) {
		printf("\tmovq %%%s, %%%s\n", src, dest);
	}
}
char *asm_imov (long val)
{
	char *r = newreg ();

	#ifdef MY_DEBUG
	printf ("\t# asm_imov (%i)\n", val);
	#endif

	printf ("\tmovq $%i, %%%s\n", val, r);
	return r;
}

char *asm_add (char *p1, char *p2)
{
	char *r;

	#ifdef MY_DEBUG
	printf ("\t# asm_add (%s, %s)\n", p1, p2);
	printf ("\t# p1: %i, p2: %i\n", reg_is_param (p1), reg_is_param (p2));
	#endif

	if (reg_is_param (p1) && reg_is_param (p2)) {
		// Both operands are parameters
		r = newreg ();
		printf ("\tmovq %%%s, %%%s\n", p1, r);
		printf ("\taddq %%%s, %%%s\n", p2, r);
	} else if (reg_is_tmp (p1) && reg_is_tmp (p2)) {
		// Both operands are from tmp registers
		printf ("\taddq %%%s, %%%s\n", p2, p1);
		r = p1;
		freereg (p2);
	} else if (reg_is_tmp (p1) && reg_is_param (p2)) {
		// p1: tmp, p2: param
		printf ("\taddq %%%s, %%%s\n", p2, p1);
		r = p1;
	} else if (reg_is_param (p1) && reg_is_tmp (p2)) {
		// p1: param, p2: tmp
		printf ("\taddq %%%s, %%%s\n", p1, p2);
		r = p2;
	}

	return r;
}
char *asm_sub (char *p1, char *p2)
{
	char *r;

	#ifdef MY_DEBUG
	printf ("\t# asm_sub (%s, %s)\n", p1, p2);
	printf ("\t# p1: %i, p2: %i\n", reg_is_param (p1), reg_is_param (p2));
	#endif

	if (reg_is_param (p1) && reg_is_param (p2)) {
		// Both operands are parameters
		r = newreg ();
		printf ("\tmovq %%%s, %%%s\n", p1, r);
		printf ("\tsubq %%%s, %%%s\n", p2, r);
	} else if (reg_is_tmp (p1) && reg_is_tmp (p2)) {
		// Both operands are from tmp registers
		printf ("\tsubq %%%s, %%%s\n", p2, p1);
		r = p1;
		freereg (p2);
	} else if (reg_is_tmp (p1) && reg_is_param (p2)) {
		// p1: tmp, p2: param
		printf ("\tsubq %%%s, %%%s\n", p2, p1);
		r = p1;
	} else if (reg_is_param (p1) && reg_is_tmp (p2)) {
		// p1: param, p2: tmp
		r = newreg ();
		printf ("\tmovq %%%s, %%%s\n", p1, r);
		printf ("\tsubq %%%s, %%%s\n", p2, r);
		freereg (p2);
	}

	return r;
}
char *asm_mult (char *p1, char *p2)
{
	char *r;

	#ifdef MY_DEBUG
	printf("\t# asm_mult (%s, %s)\n", p1, p2);
	printf("\t# p1: %i, p2: %i\n", reg_is_param (p1), reg_is_param (p2));
	#endif

        if (reg_is_param (p1) && reg_is_param (p2)) {
                // Both operands are parameters
		r = newreg ();
		asm_mov (p1, r);
		printf ("\timul %%%s, %%%s\n", p2, r);
        } else if (reg_is_tmp (p1) && reg_is_tmp (p2)) {
                // Both operands are from tmp registers
                r = p1;
                freereg (p2);
		printf ("\timul %%%s, %%%s\n", p2, p1);
        } else if (reg_is_tmp (p1) && reg_is_param (p2)) {
                // p1: tmp, p2: param
                r = p1;
		printf ("\timul %%%s, %%%s\n", p2, p1);
        } else if (reg_is_param (p1) && reg_is_tmp (p2)) {
                // p1: param, p2: tmp
                r = p2;
		printf ("\timul %%%s, %%%s\n", p1, p2);
        }

	return r;
}

char *asm_array_read (char *base, char *offset)
{
	char *r;

	#ifdef MY_DEBUG
	printf ("\t#asm_array_read (%s, %s)\n", base, offset);
	#endif

	if (reg_is_tmp (base))
		r = base;
	else
		if (reg_is_tmp (offset))
			r = offset;
		else
			r = newreg ();

	printf ("\tmovq (%%%s, %%%s, %i), %%%s\n", base, offset, sizeof (long), r);

	return r;
}

char *asm_array_read_const (char *base, long offset)
{
	char *r;

	#ifdef MY_DEBUG
	printf ("\t#asm_array_read_const (%s, %l)\n", base, offset);
	#endif

	if(reg_is_tmp (base))
		r = base;
	else
		r = newreg ();

	printf ("\tmovq %i(%%%s), %%%s\n", sizeof (long) * offset, base, r);

	return r;
}

char *asm_not (char *o)
{
	char *r;
	static long id = 0;

	#ifdef MY_DEBUG
	printf ("\t# asm_not (%s)\n", o);
	printf ("\t# p(o): %i\n", reg_is_param (o));
	#endif

	if (reg_is_tmp (o))
		r = o;
	else
		r = newreg ();

	printf ("\tcmp $0, %%%s\n", o);
	printf ("\tjz not_%i_yes\n", id);
	printf ("not_%i_no:\n", id);
	printf ("\tmov $0, %%%s\n", r);
	printf ("\tjmp not_%i_after\n", id);
	printf ("not_%i_yes:\n", id);
	printf ("\tmov $1, %%%s\n", r);
	printf ("\nnot_%i_after:\n", id);

	id++;

	return r;
}

char *asm_cmp_uneq (char *r1, char *r2)
{
	char *r;

	#ifdef MY_DEBUG
	printf ("\t# asm_cmp_uneq (%s, %s)\n", r1, r2);
	printf ("\t# r1: %i, r2: %i\n", reg_is_param (r1), reg_is_param (r2));
	#endif

	if (reg_is_tmp (r1)) {
		r = r1;
		if (reg_is_tmp (r2))
			freereg (r2);
	} else if (reg_is_tmp (r2))
		r = r2;
	else
		r = newreg ();

	printf ("\tcmp %%%s, %%%s\n", r1, r2);
	printf ("\tsetnz %%%s\n", get_8reg (r));
	printf ("\tand $1, %%%s\n", r);

	return r;
}

char *asm_cmp_l (char *r1, char *r2)
{
	char *r;

	#ifdef MY_DEBUG
	printf ("\t# asm_cmp_l (%s, %s)\n", r1, r2);
	printf ("\t# r1: %i, r2: %i\n", reg_is_param (r1), reg_is_param (r2));
	#endif

	if (reg_is_tmp (r1)) {
		r = r1;
		if (reg_is_tmp (r2))
			freereg (r2);
	} else if (reg_is_tmp (r2))
		r = r2;
	else
		r = newreg ();

	printf ("\tcmp %%%s, %%%s\n", r2, r1);
	printf ("\tsetl %%%s\n", get_8reg (r));
	printf ("\tand $1, %%%s\n", r);

	return r;
}

void asm_ret (void)
{
	printf ("\tret\n");
}

void asm_loop_eternity (long id)
{
	printf ("loop_eternity_%1$i:\njmp loop_eternity_%1$i\n", id);
}
