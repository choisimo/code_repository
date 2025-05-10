lookahead: lex.yy.o
	gcc -o lookahead lex.yy.o -ll
lex.yy.o: lex.yy.c
	gcc -c -g lex.yy.c
lex.yy.c: lookahead.l
	flex lookahead.l
