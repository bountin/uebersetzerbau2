#ifndef ASM_H
#define ASM_H

#include "common.h"
#include "code_gen.h"

void asm_func_head (char *func_name);

void asm_mov (char *src, char *dest);
char *asm_imov (long val, char *dest);

char* asm_add (char *p1, char *p2);
char* asm_sub (char *p1, char *p2);
char* asm_mult (char *p1, char *p2);

char *asm_array_write (char *l, char *r);
char *asm_array_write_imm (char *l, long r);
char *asm_array_read (char *base, char *offset);
char *asm_array_read_const (char *base, long offset);

char *asm_not (char *o);
char *asm_cmp_uneq (char *r1, char *r2);
char *asm_cmp_l (char *r1, char *r2);

void asm_or_between (int id, char *reg);
void asm_or_after (int id, char *reg);

void asm_if (char *r, long label);

void asm_ret (void);

void asm_loop_eternity (long id);

char *asm_add_imm (char *r, long imm);
char *asm_mult_imm (char *r, long imm);
char *asm_cmp_uneq_imm (char *r, long imm);

char *asm_cmp_l_imm (char *r1, char *r2);

char *asm_or_imm (char *r, long imm);

char *asm_func_call (struct code *code);

#endif
