/***** Definition section *****/

id [a-zA-Z][0-9a-zA-Z]*
hexnum \$[0-9a-fA-F]+
decnum [0-9]+
whitespace [ \t\n]
special [;(),:<#\[\]\-+*]|:=
comment --.*

%%	/***** Rules section *****/

"end"	|
"array" |
"of" 	|
"int"	|
"return" |
"if"	|
"then"	|
"else"	|
"while"	|
"do"	|
"var"	|
"not"	|
"or"	|
{special} { printf("%s\n", yytext); }

{whitespace} |
{comment} { /** Do nothing **/ }

{id} { printf("id %s\n", yytext); }

{hexnum} { printf("num %x\n", strtol(yytext + sizeof(char),NULL,16) ); }
{decnum} { printf("num %x\n", strtol(yytext               ,NULL,10) ); }

. { printf("Lexical error.\n"); exit(1); }

%% /***** Code section *****/
