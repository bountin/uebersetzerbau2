ag: y.tab.c lex.yy.c table.c table.h checks.c checks.h
	gcc *.c -lfl -o ag

oxout.y oxout.l: grammar.y parser.flex
	ox grammar.y parser.flex

y.tab.c y.tab.h: grammar.y
	bison -vd grammar.y

lex.yy.c: parser.flex
	flex parser.flex

clean:
	rm -f ag *tab.? *.output lex.yy.c

test: ag
	/usr/ftp/pub/ubvl/test13/ag/test

