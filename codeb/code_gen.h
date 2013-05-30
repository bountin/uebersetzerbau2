#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "table.h"

enum {
	TT_RETURN=1,
	TT_NOP,
	TT_NUM,
	TT_ADD,
	TT_SUB,
	TT_MULT,
	TT_VARIABLE,
	TT_ARR_READ,
	TT_IF,
	TT_WHILE,
	TT_CMP_UNEQ,
	TT_CMP_L,
	TT_NOT,

	// Later maybe
	TT_IDENTIFIER,
	TT_MINUS,
	TT_AND,
	TT_READ
};

#ifndef CODE
	typedef struct burm_state *STATEPTR_TYPE;
#endif

/** code tree for iburg **/
struct code {
	int op;
	struct code *kids[2];
	STATEPTR_TYPE state;
	long val;
	char *name;
	char *reg;
};

/** iburg macros **/
#define OP_LABEL(p) ((p)->op)
#define LEFT_CHILD(p) ((p)->kids[0])
#define RIGHT_CHILD(p) ((p)->kids[1])
#define STATE_LABEL(p) ((p)->state)
#define PANIC printf
#define NODEPTR_TYPE struct code*

#define LC(p) (LEFT_CHILD(p))
#define RC(p) (RIGHT_CHILD(p))

#define    REG(p) ((p)->reg)
#define LC_REG(p) (LEFT_CHILD(p)->reg)
#define RC_REG(p) (RIGHT_CHILD(p)->reg)

#define    VAL(p) ((p)->val)
#define LC_VAL(p) (LEFT_CHILD(p)->val)
#define RC_VAL(p) (RIGHT_CHILD(p)->val)

void execute_iburg (struct code *code);
void not_supported (char *production);

struct code* create_code (int type, struct code *left_child, struct code *right_child);
struct code* create_code_num (long number);
struct code* create_code_var (char *name, struct symbol *params, struct symbol *vars);
struct code* create_code_arr_read (void);

struct code* create_code_if (struct code *condition);
struct code* create_code_while (struct code *condition);

struct code* create_code_nop (void);

struct symbol* gen_para_regs (struct symbol *parameters);

void code_print (struct code *code);

#endif
