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
// We'll need a new structure for FuncInfo as described in the issue,
// let's define a placeholder for now.
// We'll define FuncInfo in type.h later or a new header e.g. funcinfo.h
typedef struct FuncInfo {
    char *name;
    TypeInfo *return_type;
    SymbolEntry *parameters; // Using SymbolEntry to list parameters (name and type)
    struct FuncInfo *next;
} FuncInfo;

FuncInfo *global_function_definitions = NULL;
