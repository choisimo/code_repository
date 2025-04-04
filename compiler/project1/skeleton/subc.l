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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/****************************************************/
typedef struct nstack {
    char* arr;
    int capacity;
    int top;
} nstack;

// create stack
nstack* nstack_creat(int capacity) {
    nstack* stack = (nstack *)malloc(sizeof(nstack));
    
    if (stack == NULL) {
        printf("failed to allocate memory");
        return NULL;
    }

    stack->capacity = capacity;
    stack->top = -1;
    stack->arr = (char *)malloc(capacity * sizeof(char));

    if (stack->arr == NULL){
        free(stack);
        return NULL;
    }

    return stack;
}

// delete stack
void nstack_delete(nstack* stack) {
    if (stack != NULL) {
        free(stack->arr);
        free(stack);
    }
}

// check if empty
bool empty(nstack* stack) {
    return stack->top == -1;
}

// check if full
bool full(nstack* stack) {
    return stack->top == stack->capacity -1;
}

// add element to the stack
bool push(nstack* stack, char item) {
    if (full(stack)) {
        int new_capacity = stack->capacity * 2;
        char* newArr = (char *)malloc(new_capacity * sizeof(char));

        if (newArr == NULL) {
            printf("failed to allocate memory\n");
            return false;
        }

        memcpy(newArr, stack->arr, stack->capacity * sizeof(char));
        free(stack->arr);
        stack->arr = newArr;
        stack->capacity = new_capacity;
    }
    stack->arr[++stack->top] = item;
    return true;
}

// delete element from the stack 
char pop(nstack* stack) {
    if (empty(stack)) {
        printf("stack underflow\n");
        exit(1);
    }
    return stack->arr[stack->top--];
}

// peak the stack
char peak(nstack* stack) {
    if (empty(stack)) {
        printf("stack underflow\n");
        exit(1);
    }
    return stack->arr[stack->top];
}
/****************************************************/

typedef struct PROCESSOR {
    int comment_depth;
    nstack* cstack;
    bool mode;          // comment mode (0: no, 1: yes)
} PROCESSOR;

// 프로세서 생성
PROCESSOR* processor_create(int capacity) {
    PROCESSOR* proc = 
    (PROCESSOR *)malloc(sizeof(PROCESSOR));

    if (proc == NULL) {
        return NULL;
    }

    proc->comment_depth = 0;
    proc->mode = false;
    proc->cstack = nstack_creat(capacity);

    if (proc->cstack == NULL) {
        free(proc);
        return NULL;
    }

    return proc;
}

// 프로세서 해제
void processor_delete(PROCESSOR* proc) {
    if (proc != NULL) {
        nstack_delete(proc->cstack);
        free(proc);
    }
}

// 주석 시작 처리
void process_comment_start(PROCESSOR* proc) {
    proc->comment_depth++;
    push(proc->cstack, '/');
    proc->mode = true;
}

// 주석 종료 처리
bool process_comment_end(PROCESSOR* proc) {
    if (!empty(proc->cstack)) {
        pop(proc->cstack);
        proc->comment_depth--;

        if (proc->comment_depth == 0) {
            proc->mode = false;
            return true;
        }
    }
    return false;
}

// 주석 상태 확인
bool is_in_comment(PROCESSOR* proc) {
    return proc->mode;
}

/****************************************************/

PROCESSOR *processor = NULL;
%}

/* You can put any meaningful regular definitions */
letter      [A-Za-z_]
digit       [0-9]
id  		{letter}({letter}|{digit})*

/*
op1         [+-=]
op2 		(\(|\)|\{|\}|\;)
operator	{op1}|{op2} 
*/

whitespace	[ \t\n]+
int			{digit}+
float		{digit}+\.{digit}*([eE][+-]?{digit}+)?

/* KEYWORDS */
keyword (auto|break|case|char|continue|default|do|double|else|extern|float|for|goto|if|int|long|register|return|short|sizeof|static|struct|switch|typedef|union|unsigned|void|while)

/* OPERATORS */
operator ("("|")"|"["|"]"|"{"|"}"|"->"|"."|","|"\\"|"!"|"~"|"++"|"--"|"*"|"/"|"%"|"+"|"-"|"<<"|">>"|"<"|"<="|">"|">="|"=="|"!="|"&"|"^"|"|"|"&&"|"||"|"?"|":"|";"|"="|"+="|"-="|"*="|"/="|"%="|"&="|"^="|"|="|"<<="|">>=")

/* inclusive start states */
%start AA BB 	
/* exclusive start state for comments */
%x COMMENT		

%%

<AA,BB>".."/[0-9]+	{ printf("OP\t%s\n", yytext); }
<AA,BB>{operator}   { printf("OP\t%s\n", yytext); }
<AA,BB>{whitespace} { /* ECHO 출력?? */}
<AA,BB>{id}     	{ printf("ID\t%s\n", yytext); }
<AA,BB>{keyword}	{ printf("KEYWORD\t%s\n", yytext); }
<AA,BB>{float}		{ printf("FLOAT\t%s\n", yytext); }
<AA,BB>{int}		{ printf("INTEGER\t%s\n", yytext); }

<AA>"/*"        { BEGIN(COMMENT); process_comment_start(processor); }
<BB>"/*"        { BEGIN(COMMENT); process_comment_start(processor); }

<COMMENT>"/*"	{ process_comment_start(processor); } 
<COMMENT>"*/"	{ if(process_comment_end(processor)){ BEGIN(INITIAL); } }
<COMMENT>.|\n	{}

%%

int main (int argc, char** argv)
{
	processor = processor_create(100);
	if (processor == NULL) {
		return 1;
	}

	struct id* temp;
	++argv, --argc;
	if (argc > 0) {
		yyin = fopen (argv[0], "r");
	} else {
		yyin = stdin;
	}

	BEGIN AA;
	yylex ();

	processor_delete(processor);
	
	return 0;
}
