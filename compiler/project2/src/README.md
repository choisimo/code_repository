# SubC 컴파일러 프로젝트

이 프로젝트는 C 언어의 부분집합인 SubC 언어를 위한 컴파일러를 구현합니다.

## 컴파일러 관련 용어 정의

### 기본 용어
- **컴파일러(Compiler)**: 고급 프로그래밍 언어로 작성된 소스 코드를 기계어 또는 다른 목적 코드로 변환하는 프로그램
- **어휘 분석(Lexical Analysis)**: 소스 코드를 토큰(token)이라는 의미 있는 단위로 분리하는 과정
- **구문 분석(Syntax Analysis)**: 토큰화된 소스 코드가 언어의 문법 규칙에 맞는지 확인하고 파스 트리를 구성하는 과정
- **의미 분석(Semantic Analysis)**: 프로그램의 의미적 오류(예: 타입 검사)를 확인하는 과정
- **중간 코드 생성(Intermediate Code Generation)**: 소스 코드의 중간 표현을 생성하는 과정
- **코드 최적화(Code Optimization)**: 생성된 코드를 더 효율적으로 만드는 과정
- **코드 생성(Code Generation)**: 최종 목적 코드(어셈블리, 기계어 등)를 생성하는 과정

### 파서 관련 용어
- **토큰(Token)**: 어휘 분석 단계에서 식별된 최소 의미 단위 (예: 식별자, 키워드, 기호 등)
- **문법(Grammar)**: 언어의 구문 규칙을 정의하는 형식적 규칙의 집합
- **생성 규칙(Production Rule)**: 비터미널 심볼을 다른 심볼들로 대체하는 규칙
- **터미널 심볼(Terminal Symbol)**: 문법에서 더 이상 대체되지 않는 기본 심볼 (토큰에 해당)
- **비터미널 심볼(Non-terminal Symbol)**: 문법 규칙에 의해 다른 심볼로 대체될 수 있는 심볼
- **파스 트리(Parse Tree)**: 소스 코드의 구문 구조를 시각화한 트리 구조
- **추상 구문 트리(AST, Abstract Syntax Tree)**: 파스 트리에서 구문적 세부 사항을 제거한 트리 구조

### Bison/Flex 관련 용어
- **Flex(Fast Lexical Analyzer Generator)**: 어휘 분석기(lexer)를 생성하는 도구
- **Bison**: GNU 파서 생성기, YACC(Yet Another Compiler Compiler)의 GNU 버전
- **규칙 섹션(Rules Section)**: Bison에서 문법 규칙을 정의하는 부분
- **액션(Action)**: 각 규칙이 매칭될 때 실행되는 코드
- **토큰 선언(%token)**: Bison에서 터미널 심볼(토큰)을 선언하는 지시자
- **우선순위 선언(%left, %right, %nonassoc)**: 연산자의 우선순위와 결합성을 지정하는 지시자

## 컴파일러 작동 메커니즘

### 1. 컴파일 과정
1. **전처리(Preprocessing)**: `#include`, `#define` 등의 전처리 지시자 처리
2. **어휘 분석(Lexical Analysis)**: Flex를 사용하여 소스 코드를 토큰으로 분리
3. **구문 분석(Syntax Analysis)**: Bison을 사용하여 토큰들이 문법에 맞는지 검사하고 파스 트리 생성
4. **의미 분석(Semantic Analysis)**: 타입 검사, 변수 선언 확인 등
5. **중간 코드 생성(Intermediate Code Generation)**: 추상 구문 트리에서 중간 코드 생성
6. **코드 최적화(Code Optimization)**: 중간 코드를 최적화
7. **목적 코드 생성(Target Code Generation)**: 최종 어셈블리 코드 또는 기계어 생성

### 2. Flex와 Bison의 상호작용
- **Flex**: 정규 표현식 패턴을 사용하여 입력 텍스트를 토큰으로 변환
- **Bison**: Flex에서 제공하는 토큰 스트림을 입력으로 받아 구문 분석 수행
- 두 도구는 `yylex()`와 같은 함수를 통해 통신

### 3. 오류 처리
- **어휘 오류(Lexical Error)**: 유효하지 않은 문자나 토큰
- **구문 오류(Syntax Error)**: 문법 규칙을 위반하는 토큰 순서
- **의미 오류(Semantic Error)**: 프로그램의 의미적 오류 (타입 불일치, 미선언 변수 사용 등)

## Bison 및 Flex 사용 방법

### Flex 파일 구조 (.l)
