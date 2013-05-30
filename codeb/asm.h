#ifndef ASM_H
#define ASM_H

void asm_func_head (char *func_name);

void asm_mov (char *src, char *dest);
char *asm_imov (long val);

char* asm_add (char *p1, char *p2);
char* asm_sub (char *p1, char *p2);
char* asm_mult (char *p1, char *p2);

char *asm_array_read (char *base, char *offset);
char *asm_array_read_const (char *base, long offset);

char *asm_cmp_uneq (char *r1, char *r2);

void asm_ret ();

#endif
