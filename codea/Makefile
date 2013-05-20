codea: y.tab.c lex.yy.c table.c table.h checks.c checks.h code.c
	gcc *.c -lfl -o codea

oxout.y oxout.l: grammar.y parser.flex
	ox grammar.y parser.flex

y.tab.c y.tab.h: oxout.y
	bison -vd oxout.y

lex.yy.c: oxout.l
	flex oxout.l

code.c: code.bfe
	bfe < code.bfe | iburg > code.c

clean:
	rm -f codea *tab.? *.output lex.yy.c oxout.?

check: codea
	/usr/ftp/pub/ubvl/test13/codea/test | sed '1,/Test file:/!{ /\(Correct\)\|\(.Error\)/,/Test file:/!d; }' | sed '/^$$/d'
test: codea
	/usr/ftp/pub/ubvl/test13/codea/test 
