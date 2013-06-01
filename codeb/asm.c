#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "asm.h"
#include "reg_management.h"

void asm_func_head (char *func_name)
{
	printf(".globl %1$s\n.type %1$s, @function\n%1$s:\n", func_name);
}

void asm_mov (char *src, char *dest)
{
	#ifdef MY_DEBUG
	printf ("\t# asm_mov(%s, %s)\n", src, dest);
	printf ("\t# src: %i, dest: %i\n", reg_is_param (src), reg_is_param (dest));
	#endif

	if (strcmp (src, dest)) {
		printf("\tmovq %%%s, %%%s\n", src, dest);
		if (reg_is_tmp (src))
			freereg (src);
	}
}
char *asm_imov (long val, char *dest)
{
	#ifdef MY_DEBUG
	printf ("\t# asm_imov (%i, %s)\n", val, dest);
	#endif

	printf ("\tmovq $%i, %%%s\n", val, dest);
	return dest;
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

char *asm_array_write (char *l, char *r)
{
	char *x = newreg ();

	#ifdef MY_DEBUG
	printf ("\t#asm_array_write (%s, %s)\n", l, r);
	#endif

	printf ("\timul $%i, %%%s, %%%s\n", sizeof (long), r, x);
	x = asm_add (x, l);

	if (reg_is_tmp (l))
		freereg (l);
	if (reg_is_tmp (r))
		freereg (r);

	return x;
}
char *asm_array_write_imm (char *l, long r)
{
	char *x = l;

	#ifdef MY_DEBUG
	printf ("\t#asm_array_write_imm (%s, %i)\n", l, r);
	#endif

	if (reg_is_param (l)) {
		x = newreg ();
		asm_mov (l, x);
	}

	asm_add_imm (x, sizeof (long) * r);

	if (reg_is_tmp (l))
		freereg (l);

	return x;
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
	printf ("\t#asm_array_read_const (%s, %i)\n", base, offset);
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

char *asm_or (char *p1, char *p2)
{
	char *r;

	#ifdef MY_DEBUG
	printf ("\t# asm_or (%s, %s)\n", p1, p2);
	printf ("\t# p1: %i, p2: %i\n", reg_is_param (p1), reg_is_param (p2));
	#endif

	if (reg_is_param (p1) && reg_is_param (p2)) {
		// Both operands are parameters
		r = newreg ();
		printf ("\tmovq %%%s, %%%s\n", p1, r);
		printf ("\tor %%%s, %%%s\n", p2, r);
	} else if (reg_is_tmp (p1) && reg_is_tmp (p2)) {
		// Both operands are from tmp registers
		printf ("\tor %%%s, %%%s\n", p2, p1);
		r = p1;
		freereg (p2);
	} else if (reg_is_tmp (p1) && reg_is_param (p2)) {
		// p1: tmp, p2: param
		printf ("\tor %%%s, %%%s\n", p2, p1);
		r = p1;
	} else if (reg_is_param (p1) && reg_is_tmp (p2)) {
		// p1: param, p2: tmp
		printf ("\tor %%%s, %%%s\n", p1, p2);
		r = p2;
	}

	return r;
}

void asm_if (char *r, long label)
{
	if (reg_is_tmp (r))
		freereg (r);
	printf("\tcmp $0, %%%s\n\tjz if_%i_false\n", r, label);
}

void asm_ret (void)
{
	printf ("\tret\n");
}

void asm_loop_eternity (long id)
{
	printf ("loop_eternity_%1$i:\njmp loop_eternity_%1$i\n", id);
}

char *asm_add_imm (char *r, long imm)
{
	#ifdef MY_DEBUG
	printf ("\t#asm_add_imm (%s, %i)\n", r, imm);
	#endif

	if (reg_is_param (r)) {
		char *rt = newreg ();
		asm_mov (r, rt);
		r = rt;
	}

	printf ("\taddq $%i, %%%s\n", imm, r);
	return r;
}
char *asm_mult_imm (char *r, long imm)
{
	#ifdef MY_DEBUG
	printf ("\t#asm_mult_imm (%s, %i)\n", r, imm);
	#endif

	char *rt = r;
	if (reg_is_param (r))
		rt = newreg ();

	printf ("\timul $%i, %%%s, %%%s\n", imm, r, rt);
	return rt;
}
char *asm_cmp_uneq_imm (char *r, long imm)
{
	#ifdef MY_DEBUG
	printf ("\t#asm_cmp_uneq_imm (%s, %i)\n", r, imm);
	#endif

	printf ("\tcmp $%i, %%%s\n", imm, r);
	printf ("\tsetnz %%%s\n", get_8reg (r));
	printf ("\tand $1, %%%s\n", r);
	return r;
}
char *asm_or_imm (char *r, long imm)
{
	#ifdef MY_DEBUG
	printf ("\t#asm_or_imm (%s, %i)\n", r, imm);
	#endif

	if (reg_is_param (r)) {
		char *rt = newreg ();
		asm_mov (r, rt);
		r = rt;
	}

	printf ("\tior $%i, %%%s\n", imm, r);
	return r;
}
