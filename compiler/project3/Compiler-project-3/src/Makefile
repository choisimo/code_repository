#
# File Name		: Makefile
# Description	: Makefile for project #3.
#

CC = gcc
LEX = flex
YACC = bison

CFLAGS = -g
#CFLAGS = -fsanitize=address -static-libasan -g
LDFLAGS = -lfl 
YACCFLAGCS = -vd

OBJECTS = lex.yy.o subc.tab.o type.o symtab.o error.o env.o globals.o

all : subc

clean :
	rm -f *.o lex.yy.c subc.tab.c subc.tab.h subc.output subc

subc : ${OBJECTS}
	${CC} -o subc ${OBJECTS} ${CFLAGS} ${LDFLAGS}

subc.tab.o : subc.tab.c subc.h
	${CC} -c ${CFLAGS} subc.tab.c

lex.yy.o : lex.yy.c subc.tab.h subc.h
	${CC} -c ${CFLAGS} lex.yy.c

lex.yy.c : subc.l
	${LEX} subc.l

subc.tab.h subc.tab.c : subc.y
	${YACC} ${YACCFLAGCS} subc.y

# Build helper sources located in src directory

type.o : type.c type.h
	${CC} -c ${CFLAGS} type.c -o $@

symtab.o : symtab.c symtab.h type.h
	${CC} -c ${CFLAGS} symtab.c -o $@

error.o : error.c error.h
	${CC} -c ${CFLAGS} error.c -o $@

env.o : env.c env.h type.h symtab.h
	${CC} -c ${CFLAGS} env.c -o $@

globals.o : globals.c type.h symtab.h
	${CC} -c ${CFLAGS} globals.c -o $@
