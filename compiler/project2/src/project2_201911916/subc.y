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
%union {
  int ival;
  char cval;
  char *sval;
}

/* 토큰 타입 지정 */
%token <ival> INTEGER_CONST
%token <cval> CHAR_CONST
%token <sval> STRING ID LOGICAL_OR LOGICAL_AND RELOP EQUOP
%token TYPE STRUCT SYM_NULL RETURN
%token IF ELSE WHILE FOR BREAK CONTINUE
%token INCOP DECOP STRUCTOP

/* Bison declarations section */
/* @TODO: 연산자 우선순위 및 결합법칙 선언 필요 (높을수록 우선순위 높음)
연산자 우선순위 (낮음 → 높음)
1. 콤마(,) 
2. 대입(=) 
3. 논리 OR(||)
4. 논리 AND(&&) 
5. 동등(==, !=)
6. 관계(<, <=, >, >=)
7. 덧셈/뺄셈(+, -)
8. 곱셈/나눗셈(*, /, %)
9. 단항(-, !, &)
10. 전위 증감(++, --)
11. 후위([], (), ., ->)
 **/

%nonassoc LOWER_THAN_ELSE                             /* if-else 문법 */
%nonassoc ELSE                                                            /* else 문법 */
%left ','                                                                                /* 콤마 연산자 */
%right '='                                                                             /* 대입 연산자 */
%left LOGICAL_OR                                                          /* 논리 OR (||) */
%left LOGICAL_AND                                                        /* 논리 AND (&&) */
%left EQUOP                                                                      /* 동등 연산자 (==, !=) */
%left RELOP                                                                        /* 관계 연산자 (<, <=, >, >=)*/
%left '+' '-'                                                                           /* 덧셈/뺄셈 */
%left '*' '/' '%'                                                                    /* 곱셈/나눗셈/나머지 */
%right UMINUS '!' '&' INCOP DECOP                       /* 단항 연산자 */
%left '[' '(' '.' STRUCTOP                                                /* 후위 연산자 */

/* @TODO: 필요한 토큰 선언 추가
 * - C 언어 키워드: TYPE, STRUCT, SYM_NULL, RETURN, IF, ELSE, WHILE, FOR, BREAK, CONTINUE
 * - 상수: INTEGER_CONST, CHAR_CONST, STRING
 * - 연산자: LOGICAL_OR, LOGICAL_AND, RELOP, EQUOP, INCOP, DECOP, STRUCTOP
 * - 식별자: ID
 *
 * @TODO : Rules Section 확장 필요 (토큰 정의하기)
 * - 모든 토큰 선언하기
 * - Bison 은 선언된 token 만 인식함. 모든 토큰 선언 필요
 **/


%%

/*
* @TODO: 문법 규칙 추가
* - 제공된 C 언어 문법 규칙을 그대로 복사해야 함
* - 다음 구조를 포함해야 함:
* - 프로그램 구조, 외부 정의, 타입 지정자, 함수 선언, 매개변수 리스트, 정의 리스트, 복합 문장, 문장 리스트, 표현식 등을 포함
*   1. 프로그램 구조 (program, ext_def_list, ext_def)
*   2. 타입 지정자 (type_specifier, struct_specifier) - 구조체 정의 포함
*   3. 선언자 (declarator, param, ext_decl, func_decl) - 함수 선언
*   4. 초기화 (initializer, initializer_list) - 초기화
*   5. 문장 (stmt, expr_stmt, compound_stmt, selection_stmt, iteration_stmt, jump_stmt) - 문장 리스트, 복합 문장
*   6. 정의 (def, def_list, decl, decl_list) - 정의 리스트
*   7. 표현식 (expr, assignment_expr, logical_or_expr, logical_and_expr 등) - 표현식 리스트
*   8. 단항/후위 표현식 (unary_expr, postfix_expr, primary_expr) -
* - 단항 연산자에 대해 %prec UMINUS 사용 필요
**/


/* Grammar rules section*/

program
  : ext_def_list
    { reduce("program->ext_def_list"); }
  ;

ext_def_list
  : ext_def_list ext_def
    { reduce("ext_def_list->ext_def_list ext_def"); }
  | %empty
    { reduce("ext_def_list->epsilon"); }
  ;

ext_def
  : type_specifier pointers ID ';'
    { reduce("ext_def->type_specifier pointers ID ';'"); }
  | type_specifier pointers ID '[' INTEGER_CONST ']' ';'
    { reduce("ext_def->type_specifier pointers ID '[' INTEGER_CONST ']' ';'"); }
  | STRUCT ID '{' def_list '}' ';'
    { reduce("ext_def->STRUCT ID '{' def_list '}' ';'"); }
  | func_decl compound_stmt
    { reduce("ext_def->func_decl compound_stmt"); }
  ;

type_specifier
  : TYPE
    { reduce("type_specifier->TYPE"); }
  | struct_specifier
    { reduce("type_specifier->struct_specifier"); }
  ;

struct_specifier
  : STRUCT ID '{' def_list '}'
    { reduce("struct_specifier->STRUCT ID '{' def_list '}'"); }
  | STRUCT ID
    { reduce("struct_specifier->STRUCT ID"); }
  ;

func_decl
  : type_specifier pointers ID '(' ')'
    { reduce("func_decl->type_specifier pointers ID '(' ')'"); }
  | type_specifier pointers ID '(' param_list ')'
    { reduce("func_decl->type_specifier pointers ID '(' param_list ')'"); }
  ;

pointers
  : '*'
    { reduce("pointers->'*'"); }
  | %empty
    { reduce("pointers->epsilon"); }
  ;

param_list
  : param_decl
    { reduce("param_list->param_decl"); }
  | param_list ',' param_decl
    { reduce("param_list->param_list ',' param_decl"); }
  ;

param_decl
  : type_specifier pointers ID
    { reduce("param_decl->type_specifier pointers ID"); }
  | type_specifier pointers ID '[' INTEGER_CONST ']'  
    { reduce("param_decl->type_specifier pointers ID '[' INTEGER_CONST ']' "); }
  ;

def_list
  : def_list def
    { reduce("def_list->def_list def"); }
  | %empty
    { reduce("def_list->epsilon"); }
  ;

def
  : type_specifier pointers ID ';'
    { reduce("def->type_specifier pointers ID ';'"); }
  | type_specifier pointers ID '[' INTEGER_CONST ']' ';'
    { reduce("def->type_specifier pointers ID '[' INTEGER_CONST ']' ';'"); }
  | type_specifier pointers ID ',' ID ';'
    { reduce("def->type_specifier pointers ID ',' ID ';'"); }
  ;

compound_stmt
  : '{' def_list stmt_list '}'
    { reduce("compound_stmt->'{' def_list stmt_list '}'"); }
  ;

stmt_list
  : stmt_list stmt
    { reduce("stmt_list->stmt_list stmt"); }
  | %empty
    { reduce("stmt_list->epsilon"); }
  ;

stmt
  : expr ';'
    { reduce("stmt->expr ';'"); }
  | compound_stmt
    { reduce("stmt->compound_stmt"); }
  | RETURN expr ';'
    { reduce("stmt->RETURN expr ';'"); }
  | ';'
    { reduce("stmt->;"); }
  | IF '(' expr ')' stmt %prec LOWER_THAN_ELSE
    { reduce("stmt->IF '(' expr ')' stmt"); }
  | IF '(' expr ')' stmt ELSE stmt
    { reduce("stmt->IF '(' expr ')' stmt ELSE stmt"); }
  | WHILE '(' expr ')' stmt
    { reduce("stmt->WHILE '(' expr ')' stmt"); }
  | FOR '(' expr_e ';' expr_e ';' expr_e ')' stmt
    { reduce("stmt->FOR '(' expr_e ';' expr_e ';' expr_e ')' stmt"); }
  | BREAK ';'
    { reduce("stmt->BREAK ';'"); }
  | CONTINUE ';'
    { reduce("stmt->CONTINUE ';'"); }
  ;

expr_e
  : expr
    { reduce("expr_e->expr"); }
  | %empty
    { reduce("expr_e->epsilon"); }
  ;

expr
  : unary '=' expr
    { reduce("expr->unary '=' expr"); }
  | binary
    { reduce("expr->binary"); }
  ;

binary
  : binary RELOP binary
    { reduce("binary->binary RELOP binary"); }
  | binary EQUOP binary
    { reduce("binary->binary EQUOP binary"); }
  | binary '+' binary
    { reduce("binary->binary '+' binary"); }
  | binary '-' binary
    { reduce("binary->binary '-' binary"); }
  | binary '*' binary
    { reduce("binary->binary '*' binary"); }
  | binary '/' binary
    { reduce("binary->binary '/' binary"); }
  | binary '%' binary
    { reduce("binary->binary '%' binary"); }
  | unary
    { reduce("binary->unary"); }
  | binary LOGICAL_AND binary
    { reduce("binary->binary LOGICAL_AND binary"); }
  | binary LOGICAL_OR binary
    { reduce("binary->binary LOGICAL_OR binary"); }
  ;

unary
  : '(' expr ')'
    { reduce("unary->'(' expr ')'"); }
  | INTEGER_CONST
    { reduce("unary->INTEGER_CONST"); }
  | CHAR_CONST
    { reduce("unary->CHAR_CONST"); }
  | STRING
    { reduce("unary->STRING"); }
  | ID
    { reduce("unary->ID"); }
  | '-' unary %prec UMINUS
    { reduce("unary->'-' unary"); }
  | '!' unary
    { reduce("unary->'!' unary"); }
  | unary INCOP %prec STRUCTOP
    { reduce("unary->unary INCOP"); }
  | unary DECOP %prec STRUCTOP
    { reduce("unary->unary DECOP"); }
  | INCOP unary %prec UMINUS
    { reduce("unary->INCOP unary"); }
  | DECOP unary %prec UMINUS
    { reduce("unary->DECOP unary"); }
  | '&' unary
    { reduce("unary->'&' unary"); }
  | '*' unary %prec UMINUS
    { reduce("unary->'*' unary"); }
  | unary '[' expr ']'  
    { reduce("unary->unary '[' expr ']'"); }
  | unary '.' ID
    { reduce("unary->unary '.' ID"); }
  | unary STRUCTOP ID
    { reduce("unary->unary STRUCTOP ID"); }
  | unary '(' args ')'  
    { reduce("unary->unary '(' args ')' "); }
  | unary '(' ')'
    { reduce("unary->unary '(' ')' "); }
  | SYM_NULL
    { reduce("unary->SYM_NULL"); }
  ;

args
  : expr
    { reduce("args->expr"); }
  | args ',' expr
    { reduce("args->args ',' expr"); }
  ;

%%

/* Epilogue section */

int yyerror (char* s) {
  fprintf (stderr, "yyerror: %s\n", s);
}

void reduce(char* s) {
  printf("%s\n", s);
}
