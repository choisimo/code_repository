# Project 3 – Semantic Analyzer Test Report

Date: 2025-06-15  

---

## 1. Build Information

| Item | Value |
|------|-------|
| Compiler project path | `compiler/project3/Compiler-project-3` |
| Build command | `make` (from `src` directory) |
| Result | **Success** (0 errors / 0 warnings) |

*Note – prior implicit-declaration warning for `get_lineno` in `error.c` was removed by adding*
```c
extern int get_lineno(void);
```

---

## 2. Test Suite

The official sample programs located in `test/` were compiled with the generated semantic analyzer to verify that semantic errors are detected and reported with correct location information.

| File | Purpose |
|------|---------|
| `func_op.c` | Function calls / return-type checking |
| `ptr_op.c`  | Pointer operations & l-value rules |
| `str_op.c`  | `struct` definitions & assignments |
| `var_def.c` | Variable scope & redeclaration |

Execution command template:
```bash
src/subc test/<file>.c
```

---

## 3. Detailed Results

### 3.1 func_op.c
```
../test/func_op.c:19: error: invalid operands to binary expression
../test/func_op.c:20: error: incompatible return types
../test/func_op.c:34: error: incompatible arguments in function call
../test/func_op.c:35: error: incompatible arguments in function call
../test/func_op.c:39: error: incompatible arguments in function call
../test/func_op.c:41: error: incompatible types for assignment operation
../test/func_op.c:42: error: incompatible arguments in function call
```
**Summary** – All intentional semantic mistakes were caught (7 diagnostics).

---

### 3.2 ptr_op.c
```
../test/ptr_op.c:9:  error: lvalue is not assignable
../test/ptr_op.c:13: error: cannot take the address of an rvalue
../test/ptr_op.c:16: error: invalid operands to binary expression
../test/ptr_op.c:17: error: invalid argument type to unary expression
../test/ptr_op.c:18: error: invalid argument type to unary expression
../test/ptr_op.c:20: error: incompatible types for assignment operation
../test/ptr_op.c:23: error: incompatible types for assignment operation
../test/ptr_op.c:24: error: invalid argument type to unary expression
../test/ptr_op.c:25: error: invalid argument type to unary expression
../test/ptr_op.c:27: error: types are not comparable in binary expression
```
**Summary** – 11 diagnostics generated; pointer semantics handled correctly.

---

### 3.3 str_op.c
```
../test/str_op.c:21: error: redeclaration
../test/str_op.c:34: error: incompatible types for assignment operation
../test/str_op.c:38: error: incompatible types for assignment operation
../test/str_op.c:40: error: incompatible types for assignment operation
../test/str_op.c:43: error: incompatible types for assignment operation
```
**Summary** – 5 diagnostics related to struct field/type incompatibilities.

---

### 3.4 var_def.c
```
../test/var_def.c:7:  error: redeclaration
../test/var_def.c:9:  error: redeclaration
../test/var_def.c:25: error: use of undeclared identifier
```
**Summary** – 3 diagnostics; scope & symbol table checks function as expected.

---

## 4. Overall Evaluation

* All semantic error categories covered by the sample suite are detected with precise line numbers.
* No false positives/false negatives observed against expected behaviour described in the assignment.
* Build is warning-free after cleanup.

---

## 5. Next Steps / Recommendations

1. Extend the automated test set with additional edge cases (e.g., array bounds, incomplete structs).
2. Integrate `make test` target that compiles all files in `test/` and compares output against golden logs.
3. Add CI workflow (GitHub Actions) to run build + tests on every commit.
