number: lex.yy.o
	gcc -o number lex.yy.o -ll
lex.yy.o: lex.yy.c
	gcc -c -g lex.yy.c
lex.yy.c: number.l
	flex number.l
