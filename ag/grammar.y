
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

@attributes {struct symbol *params, *vars; } 				expression_add expression_mult expression_sub term_boolean boolean call_parameters
@attributes {struct symbol *params, *vars; struct type *type; }		term expression l_expression

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

			@t check_depth (@expression.type@, @vardef.type@->depth);
		@}
	| l_expression T_ASSIGN expression
		@{	@i @stat.vars_out@ = @stat.vars@;

			@t check_depth (@expression.type@, @l_expression.type@->depth);
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
		@{	@t check_depth (@expression.0.type@, 0);
			@t check_depth (@expression.1.type@, 0);
		@}
	| expression '#' expression
		@{	@t check_depth (@expression.0.type@, 0);
			@t check_depth (@expression.1.type@, 0);
		@}
	;

l_expression:
	  T_IDENTIFIER
		@{	@i @l_expression.type@ = create_type ("", get_type (@l_expression.vars@, @l_expression.params@, @T_IDENTIFIER.name@)->depth);

			@t check_variable (@T_IDENTIFIER.name@, @l_expression.params@, @l_expression.vars@);
		@}
	| term '[' expression ']'
		@{	@i @l_expression.type@ = create_type ("", @term.type@->depth - 1);

			@t check_depth_not_zero (@term.type@);
		@}
	;

expression:
	  expression_sub
		@{	@i @expression.type@ = create_type ("", 0);
		@}
	| expression_add
		@{	@i @expression.type@ = create_type ("", 0);
		@}
	| expression_mult
		@{	@i @expression.type@ = create_type ("", 0);
		@}
	| term
		@{	@i @expression.type@ = @term.type@;
		@}
	;
expression_sub:
	  term '-' expression_sub
		@{	@t check_depth (@term.type@, 0);
		@}
	| term '-' term
		@{	@t check_depth (@term.0.type@, 0); check_depth (@term.1.type@, 0);
		@}
	;
expression_add:
	  term '+' expression_add
		@{	@t check_depth (@term.type@, 0);
		@}
	| term '+' term
		@{	@t check_depth (@term.0.type@, 0); check_depth (@term.1.type@, 0);
		@}
	;
expression_mult:
	  term '*' expression_mult
		@{	@t check_depth (@term.type@, 0);
		@}
	| term '*' term
		@{	@t check_depth (@term.0.type@, 0); check_depth (@term.1.type@, 0);
		@}
	;

term:
	  '(' expression ')'
		@{	@i @term.type@ = @expression.type@;
		@}
	| T_NUM
		@{	@i @term.type@ = create_type ("", 0);
		@}
	| term '[' expression ']'
		@{	@i @term.0.type@ = create_type ("", @term.1.type@->depth - 1);
		@}
	| T_IDENTIFIER
		@{	@i @term.type@ = create_type ("", get_type (@term.vars@, @term.params@, @T_IDENTIFIER.name@)->depth);

			@t check_variable (@T_IDENTIFIER.name@, @term.params@, @term.vars@);
		@}
	| T_IDENTIFIER '(' ')' ':' type
		@{	@i @term.type@ = create_type ("", @type.depth@);
		@}
	| T_IDENTIFIER '(' call_parameters ')' ':' type
		@{	@i @term.type@ = create_type ("", @type.depth@);
		@}
	;
call_parameters:
	  expression ',' call_parameters
	| expression
	;
