
%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "table.h"
#include "checks.h"
#include "type.h"
#include "code_gen.h"
#include "optimization.h"

int yyerror (const char *msg)
{
	fprintf (stderr, "Error: %s\n", msg);
	exit (2);
}

int yywrap () { return 1; }
main () { yyparse(); }

#define YYERROR_VERBOSE

%}

%token T_VAR T_RETURN T_NUM
%token T_ARRAY T_OF T_INT
%token T_IF T_THEN T_ELSE T_WHILE T_DO T_END
%token T_NOT T_OR
%token T_ASSIGN T_IDENTIFIER

/** Distribute tables by default **/
@autoinh params vars local_vars_in
@autosyn immediate

@attributes {int val;}		T_NUM
@attributes {char *name;} 	T_IDENTIFIER

@attributes {struct reginfo *reginfo; } function
@attributes {struct symbol *params, *vars; struct sym_bucket *local_vars, *local_vars_in;}					stats
@attributes {struct symbol *params, *vars; struct code *code;}									else
@attributes {struct symbol *params, *vars, *vars_out; struct sym_bucket *local_vars, *local_vars_in; struct code *code;}	stat
@attributes {struct symbol *params_out, *params_in; }			 							parameters parameters_with_vardefs

@attributes {struct symbol *params, *vars; struct code *code; int immediate; }				expression_add expression_mult expression_sub term_boolean boolean
@attributes {struct symbol *params, *vars; struct code *code; }						call_parameters
@attributes {struct symbol *params, *vars; struct type *type; struct code *code; int immediate; }	term expression
@attributes {struct symbol *params, *vars; struct type *type; struct code *code; }			l_expression

@attributes {int depth; }		type
@attributes {struct type *type; }	vardef

/** Checking symbol tables **/
@traversal @preorder t

@traversal @preorder z
@traversal @preorder asm

%%

program:
	  functions
		@{	@asm printf (".text\n");
		@}
	;

functions:
	  function functions
	|
	;

function:
	  T_IDENTIFIER '(' parameters ')' stats T_END ';'
		@{	@i @parameters.params_in@ = NULL;
			@i @stats.params@ = gen_para_regs (@parameters.params_out@);
			@i @stats.vars@ = NULL;
			@i @stats.local_vars_in@ = NULL;
			@i @function.reginfo@ = reg_switch (NULL);

			@t check_uniqueness (@parameters.params_out@);

			@z reg_switch (@function.reginfo@);
			@asm reg_switch (@function.reginfo@);

			@z reg_init (@stats.params@);
			@asm asm_func_head(@T_IDENTIFIER.name@);
		@}
	;

parameters:
	  parameters_with_vardefs
		@{	@i @parameters_with_vardefs.params_in@ = @parameters.params_in@;
			@i @parameters.params_out@ = @parameters_with_vardefs.params_out@;
		@}
	| 
		@{	@i @parameters.params_out@ = @parameters.params_in@;
		@}
	;
parameters_with_vardefs:
	  vardef ',' parameters_with_vardefs
		@{	@i @parameters_with_vardefs.1.params_in@ = @parameters_with_vardefs.0.params_in@;
			@i @parameters_with_vardefs.0.params_out@ = table_add_symbol (@parameters_with_vardefs.1.params_out@, @vardef.type@);
		@}
	| vardef
		@{	@i @parameters_with_vardefs.params_out@ = table_add_symbol (@parameters_with_vardefs.params_in@, @vardef.type@);
		@}
	;
vardef:
	  T_IDENTIFIER ':' type
		@{	@i @vardef.type@ = create_type (@T_IDENTIFIER.name@, @type.depth@);
		@}
	;
type:
	  T_ARRAY T_OF type
		@{ 	@i @type.0.depth@ = @type.1.depth@ + 1;
		@}
	| T_INT
		@{ 	@i @type.0.depth@ = 0;
		@}
	;

else:
	  T_ELSE stats T_END
		@{	@i @stats.local_vars_in@ = NULL;
			@asm printf ("\tjmp if_%i_after\n", @else.code@->val);
			@asm printf ("if_%i_false:\n", @else.code@->val);
		@}
	;

stats:
	  stat ';' stats
		@{	@i @stats.1.vars@ = @stat.vars_out@;
			@i @stat.local_vars_in@ = @stats.local_vars_in@;
			@i @stats.1.local_vars_in@ = @stat.local_vars@;
			@i @stats.local_vars@ = @stats.1.local_vars@;
		@}
	|
		@{	@i @stats.local_vars@ = @stats.local_vars_in@;
		@}
	;
stat:
	  T_RETURN expression
		@{	@i @stat.vars_out@ = @stat.vars@;
			@i @stat.local_vars@ = @stat.local_vars_in@;
			@i @stat.code@ = create_code (TT_RETURN, @expression.code@, NULL);

			@asm execute_iburg (@stat.code@);
		@}
	| T_IF boolean T_THEN stats T_END
		@{	@i @stat.vars_out@ = @stat.vars@;
			@i @stat.local_vars@ = @stat.local_vars_in@;
			@i @stat.code@ = create_code_if (@boolean.code@);

			@z @revorder (1) free_local_vars (@stats.local_vars@);

			@asm execute_iburg (@stat.code@);
			@asm printf ("\nif_%i_true:\n", @stat.code@->val);
			@asm @revorder (1) printf ("\nif_%i_false:\n", @stat.code@->val);
		@}
	| T_IF boolean T_THEN stats else
		@{	@i @stat.vars_out@ = @stat.vars@;
			@i @stat.local_vars@ = @stat.local_vars_in@;
			@i @stats.local_vars_in@ = NULL;
			@i @stat.code@ = create_code_if (@boolean.code@);
			@i @else.code@ = @stat.code@;

			@asm execute_iburg (@stat.code@);
			@asm printf ("\nif_%i_true:\n", @stat.code@->val);
			@asm @revorder (1) printf ("\nif_%i_after:\n", @stat.code@->val);
		@}
	| T_WHILE boolean T_DO stats T_END
		@{	@i @stat.vars_out@ = @stat.vars@;
			@i @stat.local_vars@ = @stat.local_vars_in@;
			@i @stats.local_vars_in@ = NULL;
			@i @stat.code@ = create_code_while (@boolean.code@);

			@asm printf ("\nwhile_%i_before:\n", @stat.code@->val);
			@asm execute_iburg (@stat.code@);
			@asm @revorder(1) printf ("\n\tjmp while_%1$i_before\n\nwhile_%1$i_after:\n", @stat.code@->val);
		@}
	| T_VAR vardef T_ASSIGN expression
		@{	@i @stat.vars_out@ = table_add_symbol (@stat.vars@, @vardef.type@);
			@i @stat.local_vars@ = table_add_symbol_p (@stat.local_vars_in@, @stat.vars_out@);
			@i @stat.code@ = create_code_definition (@expression.code@, @stat.vars_out@);

			@t check_depth (@expression.type@, @vardef.type@->depth);

			@z @stat.vars_out@->reg = newvarreg (); @stat.code@->reg = @stat.vars_out@->reg;

			@asm execute_iburg (@stat.code@);
		@}
	| l_expression T_ASSIGN expression
		@{	@i @stat.vars_out@ = @stat.vars@;
			@i @stat.local_vars@ = @stat.local_vars_in@;

			@i @stat.code@ = create_code (TT_ASSIGN, @l_expression.code@, @expression.code@);
			@t check_depth (@expression.type@, @l_expression.type@->depth);

			@asm execute_iburg (@stat.code@);
		@}
	| term
		@{	@i @stat.vars_out@ = @stat.vars@;
			@i @stat.local_vars@ = @stat.local_vars_in@;

			@i @stat.code@ = @term.code@;

			@asm execute_iburg (@stat.code@);
		@}
	;

boolean:
	  term_boolean T_OR boolean
		@{	@i @boolean.code@ = create_code_or (@term_boolean.code@, @boolean.1.code@);
			@i @boolean.immediate@ = @term_boolean.immediate@ && @boolean.1.immediate@;

			@z @term_boolean.code@->val = @boolean.code@->val;
		@}
	| term_boolean
		@{	@i @boolean.code@ = @term_boolean.code@;
		@}
	;
term_boolean:
	  '(' boolean ')'
		@{	@i @term_boolean.code@ = @boolean.code@;
		@}
	| T_NOT term_boolean
		@{	@i @term_boolean.code@ = optimize_not (create_code (TT_NOT, @term_boolean.1.code@, NULL));
		@}
	| expression '<' expression
		@{	@i @term_boolean.code@ = create_code (TT_CMP_L, @expression.0.code@, @expression.1.code@);
			@i @term_boolean.immediate@ = @expression.0.immediate@ && @expression.1.immediate@;
			@t check_depth (@expression.0.type@, 0);
			@t check_depth (@expression.1.type@, 0);
		@}
	| expression '#' expression
		@{	@i @term_boolean.code@ = create_code (TT_CMP_UNEQ, @expression.0.code@, @expression.1.code@);
			@i @term_boolean.immediate@ = @expression.0.immediate@ && @expression.1.immediate@;
			@t check_depth (@expression.0.type@, 0);
			@t check_depth (@expression.1.type@, 0);
		@}
	;

l_expression:
	  T_IDENTIFIER
		@{	@i @l_expression.type@ = create_type ("", get_type (@l_expression.vars@, @l_expression.params@, @T_IDENTIFIER.name@)->depth);
			@i @l_expression.code@ = create_code_var (@T_IDENTIFIER.name@, @l_expression.params@, @l_expression.vars@);

			@t check_variable (@T_IDENTIFIER.name@, @l_expression.params@, @l_expression.vars@);

			@z if (@l_expression.code@->reg == NULL) { /* !parameter but var */ @l_expression.code@->reg = table_find_symbol (@T_IDENTIFIER.name@, @l_expression.vars@)->reg; printf ("# %s >> %s\n", @T_IDENTIFIER.name@, @l_expression.code@->reg); }
		@}
	| term '[' expression ']'
		@{	@i @l_expression.type@ = create_type ("", @term.type@->depth - 1);
			@i @l_expression.code@ = create_code (TT_ARRAY, @term.code@, @expression.code@);

			@t check_depth_not_zero (@term.type@);
			@t check_depth (@expression.type@, 0);
		@}
	;

expression:
	  expression_sub
		@{	@i @expression.type@ = create_type ("", 0);
			@i @expression.code@ = @expression_sub.code@;
		@}
	| expression_add
		@{	@i @expression.type@ = create_type ("", 0);
			@i @expression.code@ = @expression_add.code@;
		@}
	| expression_mult
		@{	@i @expression.type@ = create_type ("", 0);
			@i @expression.code@ = @expression_mult.code@;
		@}
	| term
		@{	@i @expression.type@ = @term.type@;
			@i @expression.code@ = @term.code@;
		@}
	;
expression_sub:
	  expression_sub '-' term
		@{	@t check_depth (@term.type@, 0);
			@i @expression_sub.code@ = optimize_immediate (create_code (TT_SUB, @expression_sub.1.code@, @term.code@), @expression_sub.1.immediate@, @term.immediate@);
			@i @expression_sub.0.immediate@ = @expression_sub.1.immediate@ && @term.immediate@;
		@}
	| term '-' term
		@{	@t check_depth (@term.0.type@, 0); check_depth (@term.1.type@, 0);
			@i @expression_sub.code@ = create_code (TT_SUB, @term.0.code@, @term.1.code@);
			@i @expression_sub.0.immediate@ = @term.0.immediate@ && @term.1.immediate@;
		@}
	;
expression_add:
	  term '+' expression_add
		@{	@t check_depth (@term.type@, 0);
			@i @expression_add.code@ = optimize_immediate (create_code (TT_ADD, @term.code@, @expression_add.1.code@), @term.immediate@, @expression_add.1.immediate@);
			@i @expression_add.0.immediate@ = @expression_add.1.immediate@ && @term.immediate@;
		@}
	| term '+' term
		@{	@t check_depth (@term.0.type@, 0); check_depth (@term.1.type@, 0);
			@i @expression_add.code@ = create_code (TT_ADD, @term.0.code@, @term.1.code@);
			@i @expression_add.0.immediate@ = @term.0.immediate@ && @term.1.immediate@;
		@}
	;
expression_mult:
	  term '*' expression_mult
		@{	@t check_depth (@term.type@, 0);
			@i @expression_mult.code@ = optimize_immediate (create_code (TT_MULT, @term.code@, @expression_mult.1.code@), @term.immediate@, @expression_mult.1.immediate@);
			@i @expression_mult.0.immediate@ = @expression_mult.1.immediate@ && @term.immediate@;
		@}
	| term '*' term
		@{	@t check_depth (@term.0.type@, 0); check_depth (@term.1.type@, 0);
			@i @expression_mult.code@ = create_code (TT_MULT, @term.0.code@, @term.1.code@);
			@i @expression_mult.0.immediate@ = @term.0.immediate@ && @term.1.immediate@;
		@}
	;

term:
	  '(' expression ')'
		@{	@i @term.type@ = @expression.type@;
			@i @term.code@ = @expression.code@;
		@}
	| T_NUM
		@{	@i @term.type@ = create_type ("", 0);
			@i @term.code@ = create_code_num (@T_NUM.val@);
			@i @term.immediate@ = 1;
		@}
	| term '[' expression ']'
		@{	@i @term.0.type@ = create_type ("", @term.1.type@->depth - 1);
			@i @term.code@ = create_code (TT_ARR_READ, @term.1.code@, @expression.code@);
			@i @term.immediate@ = 0;
		@}
	| T_IDENTIFIER
		@{	@i @term.type@ = create_type ("", get_type (@term.vars@, @term.params@, @T_IDENTIFIER.name@)->depth);
			@i @term.code@ = create_code_var (@T_IDENTIFIER.name@, @term.params@, @term.vars@);
			@i @term.immediate@ = 0;

			@t check_variable (@T_IDENTIFIER.name@, @term.params@, @term.vars@);

			@z if (@term.code@->reg == NULL) { /* !parameter but var */ @term.code@->reg = table_find_symbol (@T_IDENTIFIER.name@, @term.vars@)->reg; printf ("# %s >> %s\n", @T_IDENTIFIER.name@, @term.code@->reg); }
		@}
	| T_IDENTIFIER '(' ')' ':' type
		@{	@i @term.type@ = create_type ("", @type.depth@);
			@i @term.code@ = create_code_func (@T_IDENTIFIER.name@, NULL);
			@i @term.immediate@ = 0;
		@}
	| T_IDENTIFIER '(' call_parameters ')' ':' type
		@{	@i @term.type@ = create_type ("", @type.depth@);
			@i @term.code@ = create_code_func (@T_IDENTIFIER.name@, @call_parameters.code@);
			@i @term.immediate@ = 0;
		@}
	;
call_parameters:
	  expression ',' call_parameters
		@{	@i @call_parameters.code@ = create_code(TT_FUNC_PARAM, @expression.code@, @call_parameters.1.code@);
		@}
	| expression
		@{	@i @call_parameters.code@ = @expression.code@;
		@}
	;
