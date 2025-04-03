magic: lex.yy.o
	gcc -o magic lex.yy.o -ll
lex.yy.o: lex.yy.c
	gcc -c -g lex.yy.c
lex.yy.c: magic.l
	flex magic.l
