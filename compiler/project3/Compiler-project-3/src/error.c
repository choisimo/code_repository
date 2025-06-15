/*
 * File Name    : error.c
 * Description  : Implementation of semantic error reporting helpers.
 */

 #include "error.h"
 #include <stdio.h>
 #include "env.h"
#include "subc.h"
 

// Ensure we don't recompile duplicate definitions from subc.y epilogue.
// The parser's epilogue now defers to these implementations.

 
 /* Common prefix for every semantic error. */
 void error_preamble(void)
 {
     // Ensure current_filename is not NULL
     // extern char *current_filename; // Available via env.h
     // extern int get_lineno(); // Available via subc.h
     if (current_filename != NULL) {
         fprintf(stderr, "%s:%d: error: ", current_filename, get_lineno());
     } else {
         fprintf(stderr, "unknown_file:%d: error: ", get_lineno());
     }
 }
 
 void error_undeclared(void)
 {
     error_preamble();
     fprintf(stderr, "use of undeclared identifier\n");
 }
 
 void error_redeclaration(void)
 {
     error_preamble();
     fprintf(stderr, "redeclaration\n");
 }
 
 void error_assignable(void) { error_preamble(); fprintf(stderr, "lvalue is not assignable\n"); }
 void error_incompatible(void) { error_preamble(); fprintf(stderr, "incompatible types for assignment operation\n"); }
 void error_null(void) { error_preamble(); fprintf(stderr, "cannot assign 'NULL' to non-pointer type\n"); }
 void error_binary(void) { error_preamble(); fprintf(stderr, "invalid operands to binary expression\n"); }
 void error_unary(void) { error_preamble(); fprintf(stderr, "invalid argument type to unary expression\n"); }
 void error_comparable(void) { error_preamble(); fprintf(stderr, "types are not comparable in binary expression\n"); }
 void error_indirection(void) { error_preamble(); fprintf(stderr, "indirection requires pointer operand\n"); }
 void error_addressof(void) { error_preamble(); fprintf(stderr, "cannot take the address of an rvalue\n"); }
 void error_struct(void) { error_preamble(); fprintf(stderr, "member reference base type is not a struct\n"); }
 void error_strurctp(void) { error_preamble(); fprintf(stderr, "member reference base type is not a struct pointer\n"); }
 void error_member(void) { error_preamble(); fprintf(stderr, "no such member in struct\n"); }
 void error_array(void) { error_preamble(); fprintf(stderr, "subscripted value is not an array\n"); }
 void error_subscript(void) { error_preamble(); fprintf(stderr, "array subscript is not an integer\n"); }
 void error_incomplete(void) { error_preamble(); fprintf(stderr, "incomplete type\n"); }
 void error_return(void) { error_preamble(); fprintf(stderr, "incompatible return types\n"); }
 void error_function(void) { error_preamble(); fprintf(stderr, "not a function\n"); }
 void error_arguments(void) { error_preamble(); fprintf(stderr, "incompatible arguments in function call\n"); }
 void error_condition_type(void) { error_preamble(); fprintf(stderr, "condition expression must be of integer type\n"); }

/* Bison yyerror */
int yyerror(char *s) {
    fprintf(stderr, "%s at line %d\n", s, get_lineno());
    return 0;
}