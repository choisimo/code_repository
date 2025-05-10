/*                                                          */
/* File Name    : subc.l                                    */
/* Description  : a lexical analyzer for SubC language      */
/*                                                          */
/* Course       : Compiler                                  */

%{
/* C Code Section */
/* Any code placed inside the brackets will be literally copied into the generated C file. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "subc.tab.h"
%}

%option noyywrap

/* Definitions Section */
/* This section contains simple name definitions and declarations of start conditions. */

/* @TODO: lexical analyzer 정의 부분 확장 필요
 * - 필요한 정규 표현식 정의 추가:
 *   + letter, digit, id 등 기본 요소 정의 
 *   + 주석 처리를 위한 상태(start condition) 정의
 * - 와일드 카드 처리 부분 구현:
 *   + 줄바꿈 문자(\n)도 매칭 가능하도록 구현
 *   + [\s\S] 또는 유사한 패턴 사용 고려 
 */


letter        [A-Za-z_]
digit         [0-9]
integer_const ([1-9][0-9]*|0)
whitespace    [ \t]
special_char  \\[nt]
id            {letter}({letter}|{digit})*
escape         \\[abfnrtv0\\'\"]
string_const   \"({escape}|[^\\\"\n]|\\\n)*\"
char_const     '({escape}|[^\\'\n])' 

/* @TODO 주석 처리 구현
* - start condition 정의 - %x COMMENT
* 독립적인 상태를 가지는 start condition COMMENT 정의 
*/

%x COMMENT

%%

  /**
   * @TODO: Rules Section 확장 필요
   * - 주석 처리 규칙 구현:
   *   + C 스타일 주석(/* */) 처리 구현
   *   + 중첩 주석 고려 필요
   *   + // 스타일 주석은 금지되어 있음 (주석 파싱 x, 주석으로 사용될 때)
   * 
   * - 토큰 인식 규칙 추가:
   *   + 모든 C 언어 키워드: if, else, while, for, return 등
   *   + 연산자: ++, --, ||, &&, ==, !=, >=, <= 등
   *   + 구조체 연산자 ->
   *   + 모든 기본 문자 토큰(+, -, *, /, = 등)
   * 
   * - 토큰 반환 값 처리:
   *   + 키워드와 특수 연산자는 토큰 타입 반환
   *   + 기본 문자 토큰은 yytext[0] 반환
   *   + subc.tab.h에 정의된 토큰과 일치해야 함
   *
   * - 와일드카드 처리를 위한 특별 규칙 구현:
   *   + 미리 정의된 패턴에 매칭되지 않는 모든 문자 처리
   */

  /* Rules Section */
  /* This section contains series of rules for recognizing tokens. */
  /* @TODO : Rules Section 확장 필요 (토큰 정의하기)
  * - 모든 토큰 선언하기
  * - Bison 은 선언된 token 만 인식함. 모든 토큰 선언 필요
  */

  /*
  @TODO : Refactoring (additional implementation required)
  * - Comments - using start condition to handle
  **/

"/*"                                                                             { BEGIN(COMMENT); }
<COMMENT>"*/"                                                                    { BEGIN(INITIAL); }
<COMMENT>[^*\n]+                                                                 { /* skip */ }
<COMMENT>\*+[^*/\n]*                                                             { /* skip */ }
<COMMENT>\n                                                                      { yylineno++; }
<COMMENT><<EOF>>                                                                 { printf("Error: unterminated comment\n"); BEGIN(INITIAL); }

/* Order : Keyword - Operator - Constant - WhiteSpace - Anything */
/* Keywords */
"int"|"char"                   { return TYPE; }
"struct"                       { return STRUCT; }
"NULL"                         { return SYM_NULL; }
"return"                       { return RETURN; }
"if"                           { return IF; }
"else"                         { return ELSE; }
"while"                        { return WHILE; }
"for"                          { return FOR; }
"break"                        { return BREAK; }
"continue"                     { return CONTINUE; }

  /* Operators */
"||"                           { return LOGICAL_OR; }
"&&"                           { return LOGICAL_AND; }
"<"|"<="|">"|">="              { return RELOP; }
"=="|"!="                      { return EQUOP; }
"++"                           { return INCOP; }
"--"                           { return DECOP; }
"->"                           { return STRUCTOP; }
"+"|"-"|"*"|"/"|"%"|"="|"&"|"!"|"^"|"~"|"."|"["|"]"|"("|")"|"{"|"}"|";"|":" { return yytext[0]; }
","                            { return yytext[0]; }

  /* Constants
  * - @TODO escape sequence 처리 필요 구현
  * - "," "\" "\0" "\r" "\n" 등 처리 필요함
  */

{integer_const}   { return INTEGER_CONST; }
{char_const}      { return CHAR_CONST; }
{string_const}    { return STRING; }
{id}              { return ID; }

  /* Whitespace */
{whitespace}+                   { /* Skip whitespace */ }
\n                             { yylineno++;  /* Skip newline */ }

  /* Anything else */
.  { 
    fprintf(stderr, "에러: %d행 - 잘못된 문자 '%c'\n", yylineno, *yytext); 
    yyterminate();
}

%%

/* User Code Section */
/* The user code section is simply copied to lex.yy.c verbatim. */

int main(int argc, char* argv[]) {
  // I/O stream settings
  if (argc >= 2) {
    yyin = fopen(argv[1], "r");
  } else {
    yyin = stdin;
  }

  if(!yyin) {
    printf("Can't open input stream!\n");
    exit(1);
  }
  
  yyparse();
  fclose(yyin);
  
  return 0;
}
