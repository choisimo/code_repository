%%
[\n\t ]     ;
-?(([0-9]+)|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?) { printf("number %s\n", yytext); }
.           ;
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
	yylex();
	return 0;
}
