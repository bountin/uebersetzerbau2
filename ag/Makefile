ag: y.tab.c lex.yy.c table.c table.h checks.c checks.h
	gcc *.c -lfl -o ag

oxout.y oxout.l: grammar.y parser.flex
	ox grammar.y parser.flex

y.tab.c y.tab.h: oxout.y
	bison -vd oxout.y

lex.yy.c: oxout.l
	flex oxout.l

clean:
	rm -f ag *tab.? *.output lex.yy.c oxout.?

test: ag
	/usr/ftp/pub/ubvl/test13/ag/test

