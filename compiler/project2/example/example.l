%{
#include "example.tab.h"
#include <stdio.h>
%}

ws 		[ \t]
num 	[0-9]
OP 		"+"|"-"|"*"|"/"

%%
\n 		{ return ENTER; }
{ws} 	{}
{num}+ {
	yylval = atoi(yytext);
	return NUMBER;
	}
{OP} 	{ return yytext[0]; }

%%
int main(int argc, char* argv[]){
	yyparse();
	return 0;
}

