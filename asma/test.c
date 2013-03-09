#include <stdio.h>
#include <assert.h>

extern int asma(char *);

int asma_ref(char *s) {
	int c = 0;

	for (int i=0; i<16; i++) {
		if (s[i] == ' ')
			c++;
	}

	return c;
}

int main(int argc, char* argv[]) {
	char* tests[] =  {
		"                ",
		"aaaaaaaaaaaaaaaa",
		"ab ab ab ab ab a",
		"abc abc abc abc ",
		"                    ",
		"aaaaaaaaaaaaaaaaaaaa"
		};
	int i;
	int elements = sizeof(tests) / sizeof (char*);

	for (i = 0; i < elements; i++) {
		printf("%d,", i);
		assert(asma(tests[i]) == asma_ref(tests[i]));
	}

	printf("\n***** EVERTHING OKAY (%d tests succeeded) *****\n\n", i);

	return 0;
}
