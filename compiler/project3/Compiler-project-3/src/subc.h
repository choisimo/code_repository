/*
 * File Name    : subc.h
 * Description  : A header file for the subc program.
 */

 #ifndef __SUBC_H__
 #define __SUBC_H__
 
 #include <stdio.h>
 #include <string.h>
#include "type.h"
#include "symtab.h"
#include "error.h"
 
 
 
 
 // Global variable for storing defined struct types
 extern struct TypeInfo *global_struct_definitions;
 
 // Global variable for storing defined function information
 extern struct FuncInfo *global_function_definitions;
 
 // Global variable to store the current function's return type for 'return' statement checking
 extern struct TypeInfo *current_function_return_type;
 
 // Global variable to pass FuncInfo to compound_stmt for parameter setup
 extern struct FuncInfo *current_processing_func_info;
 
 // declare functions used in other source code file here
 
 #endif
 