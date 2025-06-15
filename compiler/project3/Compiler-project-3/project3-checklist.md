# Project 3 Requirements Checklist

| Requirement | Implemented (Y/N) | Related Source File & Line | Notes |
|---|---|---|---|
| **1. 핵심 의미 분석 (Semantic Checks)** | | | |
| --- | --- | --- | --- |
| 선언되지 않은 식별자 사용 | Y | error.c | `error_undeclared()` |
| - 선언되지 않은 변수 사용 시 `use of undeclared identifier` 에러 출력 | Y | error.c, symtab.c | `error_undeclared()`, `sym_lookup()` |
| - 정의되지 않은 함수 호출 시 `use of undeclared identifier` 에러 출력 | Y | error.c, symtab.c | `error_undeclared()`, `sym_lookup()` |
| - Scope(범위)를 벗어난 변수 사용 시 `use of undeclared identifier` 에러 출력 | Y | error.c, symtab.c | `error_undeclared()`, `sym_lookup()` |
| 중복 선언 (Re-declaration) | Y | error.c | `error_redeclaration()` |
| - 같은 Scope 내에 동일한 이름의 변수/구조체/함수 선언 시 `redeclaration` 에러 출력 | Y | error.c, symtab.c | `error_redeclaration()`, `sym_insert()` |
| 타입 검사 (Type Checking) | | | |
| - 할당 연산 (=) | | | |
| - LHS(좌변)가 L-value가 아닐 경우 `lvalue is not assignable` 에러 출력 | Y | error.c | `error_assignable()` |
| - 포인터가 아닌 타입에 NULL 할당 시 `cannot assign 'NULL' to non-pointer type` 에러 출력 | Y | error.c | `error_null()` |
| - 서로 호환되지 않는 타입 간 할당 시 `incompatible types for assignment operation` 에러 출력 | Y | error.c | `error_incompatible()` |
| - 산술/논리 연산 (+, -, *, /, &&, ||) | | | |
| - 피연산자가 int 타입이 아닐 경우 `invalid operands to binary expression` 에러 출력 | Y | error.c | `error_binary()` |
| - 관계 연산 (>, <, >=, <=) | | | |
| - 피연산자가 int 또는 char 타입이 아닐 경우 에러 출력 | Y | subc.y | Parser rules for relational ops, type checking logic |
| - 서로 다른 타입(e.g., int와 char) 비교 시 `types are not comparable in binary expression` 에러 출력 | Y | error.c | `error_comparable()` |
| - 동등 연산 (==, !=) | | | |
| - 피연산자가 int, char, pointer 타입이 아닐 경우 에러 출력 | Y | subc.y | Parser rules for equality ops, type checking logic |
| - 호환되지 않는 포인터 타입 간 비교 시 `types are not comparable in binary expression` 에러 출력 | Y | error.c | `error_comparable()` |
| - 단항 연산 (-, !) | | | |
| - 피연산자가 int 타입이 아닐 경우 `invalid argument type to unary expression` 에러 출력 | Y | error.c | `error_unary()` |
| - 증감 연산 (++, --) | | | |
| - 피연산자가 int 또는 char 타입이 아닐 경우 `invalid argument type to unary expression` 에러 출력 | Y | error.c | `error_unary()` |
| - 포인터 연산 (*, &) | | | |
| - * 연산의 피연산자가 포인터가 아닐 경우 `indirection requires pointer operand` 에러 출력 | Y | error.c | `error_indirection()` |
| - & 연산의 피연산자가 주소를 가질 수 없는 r-value일 경우 `cannot take the address of an rvalue` 에러 출력 | Y | error.c | `error_addressof()` |
| - 구조체 연산 (., ->) | | | |
| - . 연산의 LHS가 구조체가 아닐 경우 `member reference base type is not a struct` 에러 출력 | Y | error.c | `error_struct()` |
| - -> 연산의 LHS가 구조체 포인터가 아닐 경우 `member reference base type is not a struct pointer` 에러 출력 | Y | error.c | `error_strurctp()` (sic) |
| - 존재하지 않는 멤버 접근 시 `no such member in struct` 에러 출력 | Y | error.c | `error_member()` |
| - 배열 연산 ([]) | | | |
| - [] 앞의 변수가 배열이 아닐 경우 `subscripted value is not an array` 에러 출력 | Y | error.c | `error_array()` |
| - [] 안의 인덱스가 int 타입이 아닐 경우 `array subscript is not an integer` 에러 출력 | Y | error.c | `error_subscript()` |
| - 함수 선언 및 호출 | | | |
| - 인수의 개수나 타입이 일치하지 않을 때 `incompatible arguments in function call` 에러 출력 | Y | error.c | `error_arguments()` |
| - 리턴 값이 함수의 리턴 타입과 일치하지 않을 때 `incompatible return types` 에러 출력 | Y | error.c | `error_return()` |
| - 함수가 아닌 변수를 함수처럼 호출 시 `not a function` 에러 출력 | Y | error.c | `error_function()` |
| - 구조체 선언 | | | |
| - 정의되지 않은 구조체를 멤버나 포인터로 사용 시 `incomplete type` 에러 출력 | Y | error.c | `error_incomplete()` |
| **2. Symbol Table 및 타입 시스템** | | | |
| --- | --- | --- | --- |
| Scoped Symbol Table 구현 | Y | symtab.c | `enter_scope()`, `leave_scope()`, `sym_lookup()`, `sym_insert()` |
| - Scope({ }) 진입/탈출 시 Symbol Table 스택 관리 (push/pop) | Y | symtab.c | `enter_scope()`, `leave_scope()` |
| - 변수/함수 탐색 시 현재 Scope부터 바깥 Scope 순서로 탐색 | Y | symtab.c | `sym_lookup()` logic |
| - 연결 리스트(Linked list) 등 자유로운 방식으로 구현 | Y | symtab.c | Implemented using a hash table and linked lists for collision resolution |
| 타입 정보 관리 | Y | type.c | `create_pointer_type()`, `create_array_type()`, `create_struct_type()`, etc. |
| - int, char, pointer, array, struct 타입을 구분하여 저장 | Y | type.c | `TYPE_INT`, `TYPE_CHAR`, specific creation functions |
| - 포인터, 배열의 경우 참조하는 타입 정보까지 저장 (e.g., int*, char[]) | Y | type.c | `create_pointer_type()`, `create_array_type()` store base types |
| - 구조체의 경우 필드 정보를 별도 Symbol Table로 저장 | Y | type.c, symtab.c | `create_struct_type()` uses a symbol table for members |
| - int, char 등 기본 타입을 공유하는 전역 타입 리스트 관리 | Y | type.c, globals.c | `TYPE_INT`, `TYPE_CHAR` are global types |
| L-value 정보 관리: 할당문의 LHS(좌변)에 올 수 있는지 여부를 타입 정보와 함께 관리 | Y | type.c, subc.y | `create_extended_type_info()`, used in parser rules (e.g. ID) |
| 함수 정보 관리: 파라미터와 리턴 타입을 저장하는 별도 전역 리스트 관리 | Y | globals.c, type.c | `create_func_info()`, `global_function_definitions` |
| **3. 출력 형식 (Output Format)** | | | |
| --- | --- | --- | --- |
| `error_preamble()` 함수를 수정하여 `<filename>:<line_num>: error: <description>` 형식으로 출력 | Y | error.c, subc.l, env.c | `error_preamble()`, `get_lineno()`, `current_filename` |
| - 에러 발생 시 `get_lineno()` 함수를 사용해 현재 라인 번호 포함 | Y | subc.l, error.c | `get_lineno()` called by error reporting functions |
| - main 함수에서 받은 파일명을 전역 변수로 관리하여 에러 메시지에 사용 | Y | env.c, error.c | `current_filename` used in `error_preamble()` |
| - 한 줄에 여러 에러가 있는 경우, 파싱 순서상 먼저 발견되는 1개의 에러만 출력 | Y | error.c | `yyerror()` (Bison default behavior) - needs confirmation |
| **4. 프로젝트 제약 조건 및 단순화** | | | |
| --- | --- | --- | --- |
| 테스트 코드에는 문법 오류(Syntax Error)가 없음 | N/A | | Project constraint |
| 변수 선언과 동시 초기화 (int a = 0;) 없음 | N/A | | Project constraint |
| 익명 구조체 (struct { ... } w;) 없음 | N/A | | Project constraint |
| 모든 변수 선언은 블록의 시작 부분에 위치함 | N/A | | Project constraint |
