all:
	gcc -c asma.s -o asma.o

test: all
	gcc -std=c99 test.c asma.o -o test
	./test

clean:
	rm -f *.o
	rm -f test
