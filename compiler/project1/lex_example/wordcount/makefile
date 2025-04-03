wordcount: lex.yy.o
	gcc -o wordcount lex.yy.o -ll
lex.yy.o: lex.yy.c
	gcc -c -g lex.yy.c
lex.yy.c: wordcount.l
	flex wordcount.l
