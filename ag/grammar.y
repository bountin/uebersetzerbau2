
%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "table.h"
#include "checks.h"
#include "type.h"

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
@autoinh params vars

@attributes {int val;}		T_NUM
@attributes {char *name;} 	T_IDENTIFIER

@attributes {struct symbol *params, *vars;}				stats
@attributes {struct symbol *params, *vars, *vars_out;}			stat
@attributes {struct symbol *params_out, *params_in;} 			parameters parameters_with_vardefs

@attributes {struct symbol *params, *vars; } 				expression expression_add expression_mult expression_sub l_expression term term_boolean boolean call_parameters

@attributes {int depth; }						type
@attributes {struct type *type; }					vardef

/** Checking symbol tables **/
@traversal @preorder t

%%

program:
	  function program
	|
	;

function:
	  T_IDENTIFIER '(' parameters ')' stats T_END ';'
		@{	@i @parameters.params_in@ = NULL;
			@i @stats.params@ = @parameters.params_out@;

			@i @stats.vars@ = NULL;

			@t check_uniqueness (@parameters.params_out@);
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
			@i @parameters_with_vardefs.0.params_out@ = tbl_add_symbol (@parameters_with_vardefs.1.params_out@, @vardef.type@);
		@}
	| vardef
		@{	@i @parameters_with_vardefs.params_out@ = tbl_add_symbol (@parameters_with_vardefs.params_in@, @vardef.type@);
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

stats:
	  stat ';' stats
		@{	@i @stats.1.vars@ = @stat.vars_out@;
		@}
	|
	;
stat:
	  T_RETURN expression
		@{	@i @stat.vars_out@ = @stat.vars@;
		@}
	| T_IF boolean T_THEN stats T_END
		@{	@i @stat.vars_out@ = @stat.vars@;
		@}
	| T_IF boolean T_THEN stats T_ELSE stats T_END
		@{	@i @stat.vars_out@ = @stat.vars@;
		@}
	| T_WHILE boolean T_DO stats T_END
		@{	@i @stat.vars_out@ = @stat.vars@;
		@}
	| T_VAR vardef T_ASSIGN expression
		@{	@i @stat.vars_out@ = tbl_add_symbol (@stat.vars@, @vardef.type@);
		@}
	| l_expression T_ASSIGN expression
		@{	@i @stat.vars_out@ = @stat.vars@;
		@}
	| term
		@{	@i @stat.vars_out@ = @stat.vars@;
		@}
	;

boolean:
	  term_boolean T_OR boolean
	| term_boolean
	;
term_boolean:
	  '(' boolean ')'
	| T_NOT term_boolean
	| expression '<' expression
	| expression '#' expression
	;

l_expression:
	  T_IDENTIFIER
		@{	@t check_variable (@T_IDENTIFIER.name@, @l_expression.params@, @l_expression.vars@);
		@}
	| term '[' expression ']'
	;

expression:
	  expression_sub
	| expression_add
	| expression_mult
	| term
	;
expression_sub:
	  term '-' expression_sub
	| term '-' term
	;
expression_add:
	  term '+' expression_add
	| term '+' term
	;
expression_mult:
	  term '*' expression_mult
	| term '*' term
	;

term:
	  '(' expression ')'
	| T_NUM
	| term '[' expression ']'
	| T_IDENTIFIER
		@{	@t check_variable (@T_IDENTIFIER.name@, @term.params@, @term.vars@);
		@}
	| T_IDENTIFIER '(' ')' ':' type
	| T_IDENTIFIER '(' call_parameters ')' ':' type
	;
call_parameters:
	  expression ',' call_parameters
	| expression
	;
