#include "symtab.h" // For Scope and SymbolEntry
#include "type.h"   // For TypeInfo

// From env.h
char *current_filename = NULL;

// From symtab.h
Scope *current_scope = NULL;

// Global list for struct type definitions
// We'll use a simple linked list of TypeInfo objects where kind is TYPE_KIND_STRUCT
TypeInfo *global_struct_definitions = NULL;

// Global list for function definitions
// FuncInfo is now defined in type.h
FuncInfo *global_function_definitions = NULL;

// For checking 'return' statements and setting up parameters in function scope
TypeInfo *current_function_return_type = NULL;
struct FuncInfo *current_processing_func_info = NULL;
