#ifndef ASM_H
#define ASM_H

void asm_func_head (char *func_name);

void asm_mov (char *src, char *dest);
char *asm_imov (long val);

char* asm_add (char *p1, char *p2);
char* asm_sub (char *p1, char *p2);
char* asm_mult (char *p1, char *p2);

char *asm_array_read (char *base, char *offset);

void asm_ret ();

#endif
