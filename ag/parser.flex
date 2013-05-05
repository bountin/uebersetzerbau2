/***** Definition section *****/

%{
	#include "oxout.tab.h"
%}"

id [a-zA-Z][0-9a-zA-Z]*
hexnum \$[0-9a-fA-F]+
decnum [0-9]+
whitespace [ \t\n]
comment --.*

%%	/***** Rules section *****/

"var"		return T_VAR;
"array" 	return T_ARRAY;
"of" 		return T_OF;
"int"		return T_INT;
"return" 	return T_RETURN;

"if"		return T_IF;
"then"		return T_THEN;
"else"		return T_ELSE;
"while"		return T_WHILE;
"do"		return T_DO;
"end"		return T_END;

"not"		return T_NOT;
"or"		return T_OR;

":="		return T_ASSIGN;

";"	return ';';
"("	return '(';
")"	return ')';
","	return ',';
":"	return ':';
"<"	return '<';
"#"	return '#';
"["	return '[';
"]"	return ']';
"-"	return '-';
"+"	return '+';
"*"	return '*';

{id}		return T_IDENTIFIER; @{ @T_IDENTIFIER.name@ = strdup(yytext); @}

{hexnum} 	return T_NUM;  @{ @T_NUM.val@ = strtol(yytext + sizeof(char),NULL,16); @}
{decnum} 	return T_NUM;  @{ @T_NUM.val@ = strtol(yytext               ,NULL,10); @}

{whitespace} |
{comment} { /** Do nothing **/ }

. { printf("Lexical error.\n"); exit(1); }

%% /***** Code section *****/
