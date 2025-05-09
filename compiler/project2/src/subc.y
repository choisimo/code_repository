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

/* @TODO: parser code 작성 참고 주석
 * Parser 코드 작성 시 유의 사항 (전역)
 * - 단항 연산 관련 충돌 해결을 위해 %prec 활용
 * -- 단항 마이너스에 이항 마이너스보다 높은 우선순위 부여할 때 :
 * -- %left '+' '-'
 * -- %left '*'
 * -- %left UMINUS
 * -- exp:
 * --   | '-' exp %prec UMINUS
 * - IF-ELSE conflict 는 %precedence로 선언 가능
 * - 제공된 C 문법은 conflict 발생하도록 설계되어 있음 - @TODO: 수정 필요
 * -- bison -vd subc.y 컴파일 후 subc.output 파일 확인 가능 (conflict 확인 가능)
 * -- 또는 bison의 -Wcounterexamples 옵션을 사용하여 conflict 예시 확인 가능
 * - conflict 해결을 위해 우선순위와 결합법칙을 활용
 * - 문법의 Production 을 변경하는 것은 금지됨
 * 출력형식 : 문법 규칙이 인식될 때마다 해당 규칙을 출력해야 함. @TODO: 구현 필요
 * -- ext_def_list->epsilon
 * -- type_specifier->TYPE
 * -- pointers->epsilon
 * -- func_decl->type_specifier pointers ID '(' ')'
 * - 함정
 * - 어휘 분석기 문제
 * -- 1. 와일드카드 매칭 문제 : 정규 표현식 . 은 일반적으로 개행 문자와 매칭되지 않음 (여러 줄에 걸쳐 매칭하기 위해 특수 패턴이나 플래그 사용 필요)
 * -- 2. 주석 처리 복잡성 : 중첩 주석 등 복잡한 주석 처리를 위해 시작 조건을 사용
 * -- 3. 후행 컨텍스트 문제 : 일부 후행 컨텍스트 패턴은 경고 메시지를 생성함, 후행 컨텍스트와 특수 액션을 결합하면 성능이 저하될 수 있음
 * - 파서 생성 문제
 * -- 1. 우선순위 충돌 : 모호한 문법은 shift/reduce conflict를 발생시킴 : 우선순위 선언과 %prec 를 사용하여 해결
 * -- 2. 컨텍스트 종속 우선순위 : 연산자는 다른 컨텍스트에서 다른 우선순위를 가질 수 있음 : %prec 를 사용하여 해결
 * - 일반적인 문제 (@TODO: 해결 필요)
 * -- 1. shift/reduce 충돌 | reduce/reduce 충돌
 * -- 2. 우선순위 및 결합법칙 선언으로 많은 충돌 해결 가능
 * -- 3. 파서 문제를 해결하기 전에 저절한 어휘 토큰 인식을 확인
 * - 연산자 우선순위 및 결합법칙 처리 방법
 * -- 1. %left, %right, %nonassoc 선언을 사용하여 결합법칙을 지정
 * -- 2. 먼저 나온 연산자는 나중에 작성된 연산자보다 우선순위가 높음
 * ---- 예시) %left '+' '-' %left '*' 는 곱셈에 덧셈/뺄셈보다 더 높은 우선순위를 부여
*/

/* Bison declarations section */
/* @TODO: 연산자 우선순위 및 결합법칙 선언 필요 (높을수록 우선순위 높음)
 * - C 언어의 연산자 우선순위를 따라야 함
 * - 낮은 우선순위에서 높은 우선순위 순으로 선언
 * - 다음과 같은 우선순위로 구현 필요:
 *   1. 콤마 연산자 (,) - 가장 낮은 우선순위 - 왼쪽 결합
 *   2. 대입 연산자 (=) - 오른쪽 결합
 *   3. 논리 OR (||) - 왼쪽 결합
 *   4. 논리 AND (&&) - 왼쪽 결합
 *   5. 동등 연산자 (==, !=) - 왼쪽 결합
 *   6. 관계 연산자 (<, <=, >, >=) - 왼쪽 결합
 *   7. 덧셈/뺄셈 (+, -) - 왼쪽 결합
 *   8. 곱셈/나눗셈/나머지 (*, /, %) - 왼쪽 결합
 *   9. 전위 증감 연산자, 부정 연산자, 간접 참조, 주소 연산자 (-, !, &, ++, --) - %prec UMINUS 사용 필요 - 오른쪽 결합
 *   10. 후위 연산자 ([, ], (), ., ->) - 가장 높은 우선순위 - 왼쪽 결합
 * - IF-ELSE conflict 해결을 위한 precedence 선언 필요
 **/

%left ','
%right '='
%left LOGICAL_OR
%left LOGICAL_AND
%left EQUOP
%left RELOP
%left '+' '-'
%left '*' '/' '%'
%right UMINUS '!' '&' INCOP DECOP
%left STRUCTOP


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


%token TYPE STRUCT SYM_NULL RETURN
%token INTEGER_CONST CHAR_CONST STRING
%token IF ELSE WHILE FOR BREAK CONTINUE
%token LOGICAL_OR LOGICAL_AND RELOP EQUOP INCOP DECOP STRUCTOP
%token ID

%%

/* @TODO: 문법 규칙 추가
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
