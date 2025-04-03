%s AA BB

%%
<AA>magic { printf("first\n"); BEGIN BB; }
<BB>magic { printf("second\n"); BEGIN AA; }
%%


#include <stdio.h>
int main(int argc, char ** argv)
{
	if (argc > 1) {
		FILE * file;
		file = fopen(argv[1], "r");
		if (!file) {
			fprintf(stderr, "could not open %s\n", argv[1]);
			exit(1);	
		} 
		yyin = file;
	}

    BEGIN AA;
	yylex();
	return 0;
}
