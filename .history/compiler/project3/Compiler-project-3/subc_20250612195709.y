/*
 * File Name    : subc.y
 * Description  : a skeleton bison input
 */

%{

/* Prologue section */

#include <stdio.h> // For fprintf and stderr
#include "subc.h"

extern char *current_filename; // Declared in new_file/env.h, defined in new_file/globals.c

int   yylex ();
int   yyerror (char* s);
int   get_lineno();

%}

/* Bison declarations section */

/* yylval types */
%union {
  int   intVal;
  char  *stringVal;
}

/* Precedences and Associativities */
%left   ','
%right  '='
%left   LOGICAL_OR
%left   LOGICAL_AND
%left   EQUOP
%left   RELOP
%left   '+' '-'
%left   '*' '/' '%'
%right  '!' '&' INCOP DECOP
%left   '[' ']' '(' ')' '.' STRUCTOP

%precedence IF
%precedence ELSE

/* Tokens and Types */
%token STRUCT RETURN WHILE FOR BREAK CONTINUE SYM_NULL
%token<intVal> INTEGER_CONST
%token<stringVal> ID // identifier name

/* Grammar rules */
%%
program
  : ext_def_list 
  ;

ext_def_list
  : ext_def_list ext_def 
  | %empty 
  ;

ext_def
  : type_specifier pointers ID ';' 
  | type_specifier pointers ID '[' INTEGER_CONST ']' ';' 
  | struct_specifier ';' 
  | func_decl compound_stmt
  ;

type_specifier
  : TYPE 
  | struct_specifier 
  ;

struct_specifier
  : STRUCT ID '{' def_list '}' 
  | STRUCT ID 
  ;

func_decl
  : type_specifier pointers ID '(' ')' 
  | type_specifier pointers ID '(' param_list ')' 
  ;

pointers
  : '*' 
  | %empty 
  ;

param_list  
  : param_decl 
  | param_list ',' param_decl 
  ;		

param_decl 
  : type_specifier pointers ID 
  | type_specifier pointers ID '[' INTEGER_CONST ']' 
  ;

def_list    
  : def_list def 
  | %empty 
  ;

def
  : type_specifier pointers ID ';' 
  | type_specifier pointers ID '[' INTEGER_CONST ']' ';' 
  ;

compound_stmt
  : '{' def_list stmt_list '}' 
  ;

stmt_list
  : stmt_list stmt 
  | %empty 
  ;

stmt
  : expr ';' 
  | RETURN expr ';' 
  | BREAK ';' 
  | CONTINUE ';' 
  | ';' 
  | compound_stmt 
  | IF '(' expr ')' stmt 
  | IF '(' expr ')' stmt ELSE stmt 
  | WHILE '(' expr ')' stmt 
  | FOR '(' expr_e ';' expr_e ';' expr_e ')' stmt 
  ;

expr_e
  : expr 
  | %empty 
  ;

expr
  : unary '=' expr 
  | binary 
  ;

binary
  : binary RELOP binary 
  | binary EQUOP binary 
  | binary '+' binary 
  | binary '-' binary 
  | binary '*' binary 
  | binary '/' binary 
  | binary '%' binary 
  | unary %prec '=' 
  | binary LOGICAL_AND binary 
  | binary LOGICAL_OR binary 
  ;

unary
  : '(' expr ')' 
  | '(' unary ')' 
  | INTEGER_CONST 
  | CHAR_CONST 
  | STRING 
  | ID 
  | '-' unary %prec '!' 
  | '!' unary 
  | unary INCOP %prec STRUCTOP 
  | unary DECOP %prec STRUCTOP 
  | INCOP unary %prec '!' 
  | DECOP unary %prec '!' 
  | '&' unary 
  | '*' unary %prec '!' 
  | unary '[' expr ']' 
  | unary '.' ID 
  | unary STRUCTOP ID 
  | unary '(' args ')' 
  | unary '(' ')' 
  | SYM_NULL 
  ;

args
  : expr 
  | args ',' expr 
  ;

%%

/* Epilogue section */

// Print the preamble of error message.
void error_preamble(void) {
  // Ensure current_filename is not NULL to prevent crashes, though it should always be set.
  if (current_filename != NULL) {
    fprintf(stderr, "%s:%d: error: ", current_filename, get_lineno());
  } else {
    // Fallback if current_filename is somehow NULL
    fprintf(stderr, "unknown_file:%d: error: ", get_lineno());
  }
}

void error_undeclared(void) {
  error_preamble();
  fprintf(stderr, "use of undeclared identifier\n");
}

void error_redeclaration(void) {
  error_preamble();
  fprintf(stderr, "redeclaration\n");
}

void error_assignable(void) {
  error_preamble();
  fprintf(stderr, "lvalue is not assignable\n");
}

void error_incompatible(void) {
  error_preamble();
  fprintf(stderr, "incompatible types for assignment operation\n");
}

void error_null(void) {
  error_preamble();
  fprintf(stderr, "cannot assign 'NULL' to non-pointer type\n");
}

void error_binary(void) {
  error_preamble();
  fprintf(stderr, "invalid operands to binary expression\n");
}

void error_unary(void) {
  error_preamble();
  fprintf(stderr, "invalid argument type to unary expression\n");
}

void error_comparable(void) {
  error_preamble();
  fprintf(stderr, "types are not comparable in binary expression\n");
}

void error_indirection(void) {
  error_preamble();
  fprintf(stderr, "indirection requires pointer operand\n");
}

void error_addressof(void) {
  error_preamble();
  fprintf(stderr, "cannot take the address of an rvalue\n");
}

void error_struct(void) {
  error_preamble();
  fprintf(stderr, "member reference base type is not a struct\n");
}

void error_strurctp(void){
  error_preamble();
  fprintf(stderr, "member reference base type is not a struct pointer\n");
}

void error_member(void) {
  error_preamble();
  fprintf(stderr, "no such member in struct\n");
}

void error_array(void) {
  error_preamble();
  fprintf(stderr, "subscripted value is not an array\n");
}

void error_subscript(void) {
  error_preamble();
  fprintf(stderr, "array subscript is not an integer\n");
}

void error_incomplete(void) {
  error_preamble();
  fprintf(stderr, "incomplete type\n");
}

void error_return(void) {
  error_preamble();
  fprintf(stderr, "incompatible return types\n");
}

void error_function(void) {
  error_preamble();
  fprintf(stderr, "not a function\n");
}

void error_arguments(void) {
  error_preamble();
  fprintf(stderr, "incompatible arguments in function call\n");
}
