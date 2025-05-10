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

/* 전역 변수 */
int comment_depth = 0; /* 중첩 주석 깊이 */

/* 문자 상수와 문자열 상수 처리를 위한 함수 */
static char get_escape_char(char c) {
    switch(c) {
        case 'n': return '\n';
        case 't': return '\t';
        case 'r': return '\r';
        case 'b': return '\b';
        case 'f': return '\f';
        case 'v': return '\v';
        case 'a': return '\a';
        case '0': return '\0';
        case '\'': return '\'';
        case '\"': return '\"';
        case '\\': return '\\';
        default: return c;
    }
}

static char get_char_value(const char* str) {
    if (str[1] == '\\') {
        return get_escape_char(str[2]);
    }
    return str[1];
}

static char* get_string_value(const char* str) {
    int len = strlen(str);
    char* result = (char*)malloc(len); /* 충분한 공간 확보 */
    int j = 0;
    
    /* 따옴표 제외하고 복사 (첫 문자와 마지막 문자 제외) */
    for (int i = 1; i < len - 1; i++) {
        if (str[i] == '\\') {
            if (i + 1 < len - 1 && str[i+1] == '\n') {
                i++; // skip line continuation
                continue;
            }
            if (i + 1 < len - 1) {
                result[j++] = get_escape_char(str[++i]);
                continue;
            }
        }
        result[j++] = str[i];
    }
    result[j] = '\0';
    return result;
}
%}

%option noyywrap
%option yylineno

/* Definitions Section */
/* This section contains simple name definitions and declarations of start conditions. */

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

"/*"                                                                             { comment_depth++; BEGIN(COMMENT); }
<COMMENT>"/*"                                                                    { comment_depth++; /* 중첩 주석 시작 */ }
<COMMENT>"*/"                                                                    { 
                                                                                    if (--comment_depth == 0) {
                                                                                        BEGIN(INITIAL);
                                                                                    }
                                                                                  }
<COMMENT>[^*/\n]+                                                                { /* skip */ }
<COMMENT>"*"+[^*/\n]*                                                            { /* skip */ }
<COMMENT>"/"[^*\n]*                                                              { /* skip */ }
<COMMENT>\n                                                                      { /* 줄 번호는 %option yylineno로 자동 증가 */ }
<COMMENT><<EOF>>                                                                 { 
                                                                                    fprintf(stderr, "에러: 파일 끝에 닫히지 않은 주석이 있습니다\n"); 
                                                                                    comment_depth = 0;
                                                                                    BEGIN(INITIAL); 
                                                                                  }

"int"|"char"                   { yylval.ival = (strcmp(yytext, "int") == 0) ? 0 : 1; return TYPE; }
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
"||"    { yylval.sval = strdup(yytext); return LOGICAL_OR; }
"&&"    { yylval.sval = strdup(yytext); return LOGICAL_AND; }
"<"|"<="|">"|">="    { yylval.sval = strdup(yytext); return RELOP; }
"=="|"!="    { yylval.sval = strdup(yytext); return EQUOP; }
"++"    { return INCOP; }
"--"    { return DECOP; }
"->"    { return STRUCTOP; }
"+"|"-"|"*"|"/"|"%"|"="|"&"|"!"|"^"|"~"|"."|"["|"]"|"("|")"|"{"|"}"|";"|":" { return yytext[0]; }
","    { return yytext[0]; }

  /* Constants
  * - @TODO escape sequence 처리 필요 구현
  * - "," "\" "\0" "\r" "\n" 등 처리 필요함
  */

{integer_const}   { 
                     yylval.ival = atoi(yytext);
                     return INTEGER_CONST; 
                   }
{char_const}      { 
                     yylval.cval = get_char_value(yytext);
                     return CHAR_CONST; 
                   }
{string_const}    { 
                     yylval.sval = get_string_value(yytext);
                     return STRING; 
                   }
{id}              { 
                     yylval.sval = strdup(yytext);
                     return ID; 
                   }

  /* Whitespace */
{whitespace}+    { /* Skip whitespace */ }
\n    { /* yylineno는 %option yylineno로 자동 증가 */ }

  /* Anything else */
.    {
    fprintf(stderr, "에러: %d행 - 잘못된 문자 '%c' (ASCII: %d)\n", yylineno, *yytext, *yytext); 
    return -1;  /* 오류 코드 반환 */
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
