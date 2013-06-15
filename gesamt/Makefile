gesamt: y.tab.c lex.yy.c table.c table.h checks.c checks.h code.c
	gcc *.c -lfl -o gesamt

oxout.y oxout.l: grammar.y parser.flex
	ox grammar.y parser.flex

y.tab.c y.tab.h: oxout.y
	bison -vd oxout.y

lex.yy.c: oxout.l
	flex oxout.l

code.c: code.bfe
	bfe < code.bfe | iburg > code.c

clean:
	rm -f gesamt *tab.? *.output lex.yy.c oxout.? code.c

check:
	/usr/ftp/pub/ubvl/test13/gesamt/test 2>&1 | egrep "(===|Das Ergebnis|Statistics|total|passed|failed|Error|Errot)" | egrep "failed|total|Statistics|.Error|Errot" -B1
test:
	/usr/ftp/pub/ubvl/test13/gesamt/test 
