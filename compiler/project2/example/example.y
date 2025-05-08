%{
#include <stdio.h>

int yylex();
int yyerror(const char* s);
%}

%token 	ENTER NUMBER

%left 	'+' '-'
%left 	'*' '/'

%%

expr: binary ENTER {
	printf("Result = %d\n",$1);
	return 0;
}

binary: binary '+' binary {
			$$ = $1 + $3;
			printf("%d = %d + %d\n",$$,$1,$3);
		}
	|	binary '-' binary {
		$$ = $1 - $3;
		printf("%d = %d - %d\n",$$,$1,$3);
	}
	| 	binary '*' binary {
		$$ = $1 * $3;
		printf("%d = %d * %d\n",$$,$1,$3);
	}
	| 	binary '/' binary {
		$$ = $1 / $3;
		printf("%d = %d / %d\n",$$,$1,$3);
	}
	| 	NUMBER 			{ $$ = $1; }

%%
int yyerror(const char *msg) {
	printf("Error encountered: %s \n", msg);
}
