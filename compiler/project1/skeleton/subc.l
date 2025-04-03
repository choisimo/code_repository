%{
/**************************************************************
* File Name   : subc.l
*
* Description
*	This is a skeleton input file to lex.
*
* Which tool to use
*	The GNU lex-clone, flex/flex++ is recommended to use.
*	It supports a C++ mode.
*	For any information about flex, refer to flex.man.
*
*	Good Luck!
*
***************************************************************/

/**************************************************************/
/* This section will appear at the head of lex.yy.c/lex.yy.cc */
/**************************************************************/

/***************************************************************
{Other useful code segments can be here.}
***************************************************************/
int commentdepth=0;
%}

/* You can put any meaningful regular definitions */
letter      [A-Za-z_]
digit       [0-9]
id  		{letter}({letter}|{digit})*
op1         [+-=]
op2 		(\(|\)|\{|\}|\;)
operator	{op1}|{op2}
whitespace	[ \t\n]+

%start AA BB
%%

<AA>{operator}  {}

<AA>{id}        {}
<AA>"/*"        {}
<BB>"/*"        {}

%%


int main (int argc, char** argv)
{
	struct id* temp;
	++argv, --argc;
	if (argc > 0) {
		yyin = fopen (argv[0], "r");
	} else {
		yyin = stdin;
	}
	BEGIN AA;
	yylex ();
	return 0;
}
