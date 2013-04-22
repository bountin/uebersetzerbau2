%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

%%

program:
	  function program
	|
	;

function:
	  T_IDENTIFIER '(' parameters ')' stats T_END ';'
	;

parameters:
	  parameters_with_vardefs
	| 
	;
parameters_with_vardefs:
	  vardef ',' parameters_with_vardefs
	| vardef
	;
vardef:
	  T_IDENTIFIER ':' type
	;
type:
	  T_ARRAY T_OF type
	| T_INT
	;

stats:
	  stat ';' stats
	|
	;
stat:
	  T_RETURN expression
	| T_IF boolean T_THEN stats T_END
	| T_IF boolean T_THEN stats T_ELSE stats T_END
	| T_WHILE boolean T_DO stats T_END
	| T_VAR vardef T_ASSIGN expression
	| l_expression T_ASSIGN expression
	| term
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
	| term '[' expression ']'
	;

expression:
	  expression_sub
	| expression_add
	| expression_mult
	;
expression_sub:
	  term '-' expression_sub
	| term
	;
expression_add:
	  term '+' expression_add
	| term
	;
expression_mult:
	  term '*' expression_mult
	| term
	;

term:
	  '(' expression ')'
	| T_NUM
	| term '[' expression ']'
	| T_IDENTIFIER
	| T_IDENTIFIER '(' ')' ':' type
	| T_IDENTIFIER '(' call_parameters ')' ':' type
	;
call_parameters:
	  expression ',' call_parameters
	| expression
	;
