### Directory Structure

```
project1/
├── README.md               
├── lex_example/            # Lex 예제 code
│   ├── lookahead/          # lookahead 예제
│   │   ├── lookahead.l     # lookahead Lex code
│   │   ├── makefile        
│   │   └── test.c          # 테스트용 C 파일
│   ├── number/             # 숫자 인식 예제
│   │   ├── makefile        
│   │   ├── number.l        # 숫자 인식 Lex 파일
│   │   └── test.c          # 테스트용 C 파일
│   ├── startstate/         # 시작 상태(start condition) 예제
│   │   ├── magic.l         # 시작 상태 구현 Lex 파일
│   │   ├── makefile        
│   │   └── test.c          # 테스트용 C 파일
│   └── wordcount/          # 단어 카운트 예제
│       ├── makefile        
│       ├── test.c          # 테스트용 C 파일
│       └── wordcount.l     # 단어 카운트 Lex 파일
├── result_example/         # 결과 예제
│   ├── shortex.c           # 샘플 C 프로그램
│   └── shortex.txt         # 예상 출력 결과
└── skeleton/               # 프로젝트 기본 뼈대 코드
    ├── makefile            # 빌드 스크립트
    └── subc.l              # 기본 Lex 파일
```

### Lex 개요 및 간단한 소개
- Lex는 Chomsky hierarchy의 3rd level인 정규 언어를 인식하는 도구
- Lex는 정규 표현식과 상태 기계(state machine)를 사용하여 입력 스트림을 처리
- Lex는 입력 스트림을 읽고, 정규 표현식과 일치하는 부분을 찾아 해당하는 동작을 수행
- Lex는 C 코드로 변환되어 컴파일됨

### Lex 기본 구조
```
{definitions} 
%%
{rules}
%%
{user subroutines}
```
- `{definitions}`: 정규 표현식, 변수, 함수 등을 정의하는 부분
- `%%`: Lex의 구분자
- `{rules}`: 정규 표현식과 해당하는 동작을 정의하는 부분 (패턴 매칭)
- `%%`: Lex의 구분자
- `{user subroutines}`: 사용자 정의 함수 및 서브루틴을 정의하는 부분

### Lex 기본 구조 - code example

#### Definitions
```c
%{
#include <stdio.h>
#include <stdlib.h>
letter              A-Z, a-z, _
digit               0-9
identifier          letter (letter | digit)*
number              digit+
integer-constant    -?{digit}+
%}
```

#### Rules
```c
%%
/* 키워드 인식 */
if                      { printf("Keyword: if\n"); }
else                    { printf("Keyword: else\n"); }

/* 연산자 인식 */
+                       { printf("Operator: +\n"); }
-                       { printf("Operator: -\n"); }
*                       { printf("Operator: *\n"); }

/* 구분자 인식 */
;                       { printf("Delimiter: ;\n"); }
,                       { printf("Delimiter: ,\n"); }
{                     { printf("Delimiter: {\n"); }
}                       { printf("Delimiter: }\n"); }

/* 주석 인식 */
//[^\n]*                 { printf("Comment: %s\n", yytext); }

/* 숫자 및 식별자 인식 */
[0-9]+                  { printf("Number: %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9]* { printf("Identifier: %s\n", yytext); }
%%
```
> 주의사항 : 먼저 호출 되는 규칙이 우선적으로 적용됨!!


#### User Subroutines
```c
int main(int argc, char **argv) {
    yylex(); // Lex 분석기 호출
    return 0;
}
```

#### START STATE
```c
/* exclusive state (%x) */
/* 상태 접두사가 없는 규칙은 제외 */
%x STATE_NAME
/* inclusive state (%s) */
/* 상태가 없는 규칙도 포함 */
%s STATE_NAME

/* STATE CHANGE */
BEGIN(STATE_NAME);
/* return to init */
BEGIN(INITIAL);

/* Multiple STATES example */
%x STATE1 STATE2 STATE3
%s STATE4 
```


### 참고 문헌들
- [flex & bison](https://web.iitd.ac.in/~sumeet/flex__bison.pdf)
- [oracle_docs](https://docs.oracle.com/cd/E19504-01/802-5880/lex-42143/index.html)
- [ibm_docs](https://www.ibm.com/docs/en/aix/7.2?topic=l-lex-command)
- [default-reserved.pdf](https://drop.nodove.com/-URZ7qnQHuC/Project1_Description.pdf)
- [regular-expression.pdf](https://drop.nodove.com/-frwTgqsmZB/davechild_regular-expressions.pdf)
