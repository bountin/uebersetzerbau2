#ifndef ASM_H
#define ASM_H

#include "common.h"

char *asm_array_write_2 (char *l, char *r);

void asm_func_head (char *func_name);

void asm_mov (char *src, char *dest);
char *asm_imov (long val);

char* asm_add (char *p1, char *p2);
char* asm_sub (char *p1, char *p2);
char* asm_mult (char *p1, char *p2);

void asm_array_write (char *base, char *offset, char *src);

char *asm_array_read (char *base, char *offset);
char *asm_array_read_const (char *base, long offset);

char *asm_not (char *o);
char *asm_cmp_uneq (char *r1, char *r2);
char *asm_cmp_l (char *r1, char *r2);

void asm_ret (void);

void asm_loop_eternity (long id);

#endif
