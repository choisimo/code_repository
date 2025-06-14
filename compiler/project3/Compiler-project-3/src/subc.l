/*                                                                          */
/* File Name    : subc.l                                                    */
/* Description  : a skeleton flex input                                     */
/*                                                                          */

%{

/* C Code Section */
/* Any code placed inside the brackets will be literally copied into the generated C file. */

#include "subc.h"
#include "subc.tab.h" // For yylval, token definitions
#include "new_file/env.h"
#include "new_file/type.h" // For ExtendedTypeInfo, TYPE_CHAR, TYPE_NULLPTR
#include <stdlib.h>    // For malloc, atoi
#include <string.h>    // For strdup

static int lineno = 1;
/* line counter already present; filename comes from env.c */

%}

/* Definitions Section */
/* This section contains simple name definitions and declarations of start conditions. */

letter        [A-Za-z_]
digit         [0-9]
id            {letter}({letter}|{digit})*
integer_const ([1-9]{digit}*)|"0"
whitespace    [ \t]
special_char  \\[nt]

%x COMMENT

%%

  /* Rules Section */
  /* This section contains series of rules for recognizing tokens. */

  /* Keywords */
"int"|"char"                    { return TYPE; }
"struct"                        { return STRUCT; } 
"NULL"                          {
                                  ExtendedTypeInfo* et_info = (ExtendedTypeInfo*)malloc(sizeof(ExtendedTypeInfo));
                                  et_info->type = TYPE_NULLPTR; // TYPE_NULLPTR is a global TypeInfo* from type.h
                                  et_info->is_lvalue = 0;
                                  yylval.type_info_ptr = et_info;
                                  return SYM_NULL;
                                }
"return"                        { return RETURN; }
"if"                            { return IF; }
"else"                          { return ELSE; }
"while"                         { return WHILE; }
"for"                           { return FOR; }
"break"                         { return BREAK; }
"continue"                      { return CONTINUE; }

  /* Identifiers */
{id}                            {
                                  yylval.stringVal = strdup(yytext);
                                  return ID;
                                }

  /* Integer constants */
{integer_const}                 {
                                  yylval.intVal = atoi(yytext);
                                  return INTEGER_CONST;
                                }

  /* String variables */
\"({special_char}|[^\\\"\n])*\" {
                                  // TODO: Implement proper unescaping and quote removal for yytext
                                  yylval.stringVal = strdup(yytext);
                                  return STRING;
                                }

  /* Character variables */
'({special_char}|[^\\'\n])'     {    
                                  ExtendedTypeInfo* et_info = (ExtendedTypeInfo*)malloc(sizeof(ExtendedTypeInfo));
                                  et_info->type = TYPE_CHAR; // TYPE_CHAR is a global TypeInfo* from type.h
                                  et_info->is_lvalue = 0;
                                  // The actual char value yytext[1] could be stored if ExtendedTypeInfo had a field for it.
                                  // For now, the type itself is sufficient for type checking.
                                  yylval.type_info_ptr = et_info;
                                  return CHAR_CONST;
                                }

  /* Operators */
","|";"|"("|")"|"["|"]"|"{"|"}" { return yytext[0]; }
"!"|"*"|"/"|"%"|"+"|"-"|"&"     { return yytext[0]; }
"="|"."                         { return yytext[0]; }

"&&"                            { return LOGICAL_AND; }
"||"                            { return LOGICAL_OR; }
"<"|"<="|">"|">="               { return RELOP; }
"=="|"!="                       { return EQUOP; }
"++"                            { return INCOP; }
"--"                            { return DECOP; }
"->"                            { return STRUCTOP; }

  /* Whitespaces */
{whitespace}


  /* Comment blocks */
"/*"                            {
                                  BEGIN (COMMENT);
                                }
<COMMENT>"*/"                   { 
                                  BEGIN (INITIAL);
                                }
<COMMENT>.

  /* Line number tracking */
<*>\n       { ++lineno; }

%%
   
/* User Code Section */
/* The user code section is simply copied to lex.yy.c verbatim. */

int get_lineno() {
   return lineno;
}

int main(int argc, char* argv[]) {
  if(argc >= 2) {
    yyin = fopen(argv[1], "r");
    current_filename = strdup(argv[1]);
  } else {
    yyin = stdin;
    current_filename = strdup("<stdin>");
  }

  if(!yyin) {
    printf("Can't open input stream!\n");
    exit(1);
  }
  
  init_environment();
  yyparse();
  fclose(yyin);

  return 0;
}
