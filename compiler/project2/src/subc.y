/*
 * File Name    : subc.y
 * Description  : a skeleton bison input
 * 
 * Course       : Compiler
 */

%{
/* Prologue section */
#include <stdio.h>
#include <strings.h>

int   yylex ();
int   yyerror (char* s);
void  reduce(char* s);
%}

/* Bison declarations section */
/* Precedences and Associativities */
%left ','
%left STRUCTOP

/* Tokens */
%token            TYPE STRUCT

%%

/* Grammar rules section*/
program
  : ext_def_list
  {
    reduce("program->ext_def_list");
  }
  ;

ext_def_list
  : ext_def_list ext_def
  {
    reduce("ext_def_list->ext_def_list ext_def");
  }
  | %empty 
  {
    reduce("ext_def_list->epsilon");
  }
  ;

unary
  : INTEGER_CONST
  {
    reduce("unary->INTEGER_CONST");
  }
  | CHAR_CONST
  {
    reduce("unary->CHAR_CONST");
  }
  | STRING
  {
    reduce("unary->STRING");
  }
  | ID
  {
    reduce("unary->ID");
  }
  ;

%%

/* Epilogue section */

int yyerror (char* s) {
  fprintf (stderr, "yyerror: %s\n", s);
}

void reduce(char* s) {
  printf("%s\n", s);
}
