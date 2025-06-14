/*
 * File Name    : subc.y
 * Description  : a skeleton bison input
 */

%{

/* Prologue section */

#include <stdio.h> // For fprintf and stderr
#include "subc.h"
#include "new_file/error.h"
#include "new_file/symtab.h"
#include "new_file/env.h"

extern char *current_filename; // Declared in new_file/env.h, defined in new_file/globals.c
extern char *yytext;

int   yylex ();
int   yyerror (char* s);
int   get_lineno();
void  error_condition_type(void); // New error function

%}

/* Bison declarations section */

/* yylval types */
%union {
  int   intVal;
  char  *stringVal;
  struct ExtendedTypeInfo* type_info_ptr; // For expressions
  struct TypeInfo* type_ptr;             // For type specifiers
  struct SymbolEntry* symbol_ptr;         // For lists like def_list, param_list
  struct FuncInfo* func_info_ptr;        // For function declarations
}

/* Precedences and Associativities */
%left   ','
%right  '='
%left   LOGICAL_OR
%left   LOGICAL_AND
%left   EQUOP
%left   RELOP
%left   '+' '-'
%left   '*' '/' '%'
%right  '!' '&' INCOP DECOP
%left   '[' ']' '(' ')' '.' STRUCTOP

%precedence IF
%precedence ELSE

/* Tokens and Types */
%token STRUCT RETURN WHILE FOR BREAK CONTINUE SYM_NULL
%token<intVal> INTEGER_CONST
%token<stringVal> ID TYPE CHAR_CONST STRING // identifier name

%type <type_ptr> type_specifier struct_specifier
%type <intVal> pointers
%type <symbol_ptr> param_decl param_list def def_list args // args now returns SymbolEntry* list
%type <func_info_ptr> func_decl
%type <type_info_ptr> expr expr_e binary unary // ExtendedTypeInfo* is type_info_ptr for these

/* Grammar rules */
%%
program
  : ext_def_list 
  ;

ext_def_list
  : ext_def_list ext_def 
  | %empty 
  ;

ext_def
  : type_specifier pointers ID ';' 
    { /* Global variable declaration. Already handled by def via sym_insert in current (global) scope. */ }
  | type_specifier pointers ID '[' INTEGER_CONST ']' ';' 
    { /* Global array declaration. Already handled by def via sym_insert. */ }
  | struct_specifier ';' 
    { /* Struct definition. struct_specifier rule handles adding to global_struct_definitions. */ }
  | func_decl compound_stmt /* $1 is FuncInfo* from func_decl */
    {
      FuncInfo* new_func = $1; // This is also current_processing_func_info set by func_decl
      if (new_func) {
        FuncInfo* existing_func = NULL;
        FuncInfo* iter = global_function_definitions;
        while(iter) {
          if(iter->name && strcmp(iter->name, new_func->name) == 0) {
            existing_func = iter;
            break;
          }
          iter = iter->next_global_func_link;
        }

        if (existing_func) {
          error_redeclaration(); // "Redeclaration of function"
          free_func_info(new_func); // Free the FuncInfo created if it's a redeclaration
        } else {
          // Add to global list
          new_func->next_global_func_link = global_function_definitions;
          global_function_definitions = new_func;

          // Also add function to symbol table for direct lookup via ID rule in unary expressions
          TypeInfo* f_type_for_symtab = create_function_type(new_func->return_type, new_func->parameters);
          if (!f_type_for_symtab) {
            // Error creating function type (e.g. out of memory in create_function_type or deep_copy_symbol_list)
            // This is a critical internal error. create_function_type itself should handle printing.
            // No easy recovery here, this indicates a deeper problem.
            fprintf(stderr, "Critical error: Failed to create TypeInfo for function %s for symbol table.\n", new_func->name);
          } else {
            if (!sym_insert(new_func->name, f_type_for_symtab)) {
              // This case should ideally not be hit if function redeclaration is caught by global_function_definitions check
              // and assuming function names don't clash with global vars in a way sym_insert disallows.
              // If sym_insert fails (e.g. truly out of memory, or a name clash not caught above),
              // it would have printed an error. We might need to free f_type_for_symtab if sym_insert failed
              // and didn't take ownership or if it wasn't a singleton.
              // For now, assume sym_insert handles its errors, or this path implies prior error.
              // The spec says: "변수, 구조체, 함수 등 서로 다른 종류끼리 이름이 겹치는 경우는 고려하지 않습니다."
              // This implies sym_insert for a function name into global scope should be fine if no other global func has same name.
              // If sym_insert has its own error for genuine redeclaration in same scope:
              // error_redeclaration(); // Or a more specific one if needed.
              // However, the primary redeclaration check for functions is done above using global_function_definitions.
            }
          }
        }
      }
      current_processing_func_info = NULL; // Clear after use in this ext_def
      current_function_return_type = NULL; // Also clear this, as it's set by compound_stmt based on current_processing_func_info
    }
  ;

type_specifier
  : TYPE /* TYPE is "int" or "char" from lexer */
    {
      // yytext is available for tokens. TYPE is a token.
      if (strcmp(yytext, "int") == 0) $$ = TYPE_INT;
      else if (strcmp(yytext, "char") == 0) $$ = TYPE_CHAR;
      else $$ = NULL; 
    }
  | struct_specifier /* struct_specifier will return TypeInfo* */
    { $$ = $1; } 
  ;

struct_specifier /* $$ is TypeInfo* (type_ptr) */
  : STRUCT ID /* $2 is ID token (name) */
    '{' 
      {
          /* Mark that we are now collecting struct fields */
          building_struct_fields = 1;
          enter_scope();
        } /* For struct fields */
      def_list /* $5 is SymbolEntry* list of fields */
    '}'
    {
      $$ = NULL; // Default to NULL in case of errors
      if (!$2) { // ID token's stringVal should not be NULL from lexer
          fprintf(stderr, "Compiler Error: Struct name token is NULL.\n");
          leave_scope(); // Balance enter_scope
          YYERROR;       // Trigger YACC error recovery
      }

      TypeInfo* existing_struct = NULL;
      TypeInfo* iter = global_struct_definitions;
      while(iter) {
        if(iter->info.struct_info.name && strcmp(iter->info.struct_info.name, $2) == 0) {
          existing_struct = iter;
          break;
        }
        iter = iter->next_global_struct_link;
      }

      if (existing_struct) {
        error_redeclaration(); // "Redeclaration of struct"
        // $$ remains NULL
      } else {
        // $5 is the SymbolEntry* list from def_list.
        TypeInfo* new_struct_type = create_struct_type($2, $5); 
        if (new_struct_type) {
             new_struct_type->next_global_struct_link = global_struct_definitions;
             global_struct_definitions = new_struct_type;
             $$ = new_struct_type; 
        } else {
            // create_struct_type failed (e.g. out of memory), error already printed by it.
            // $$ remains NULL
        }
      }
      leave_scope(); /* Pops the field scope. */
      building_struct_fields = 0; /* Done collecting struct fields */
      free($2); /* Free ID strdup'd by lexer */
    }
  | STRUCT ID /* $2 is ID token (name) */
    { /* Action for using a struct type */
      $$ = NULL; // Default to NULL
      if (!$2) {
          fprintf(stderr, "Compiler Error: Struct name token for lookup is NULL.\n");
          YYERROR;
      }
      TypeInfo* found_struct = NULL;
      TypeInfo* iter = global_struct_definitions;
      while(iter) {
        if(iter->info.struct_info.name && strcmp(iter->info.struct_info.name, $2) == 0) {
          found_struct = iter;
          break;
        }
        iter = iter->next_global_struct_link;
      }

      if (!found_struct) {
        error_undeclared(); // "Struct <name> not defined"
        // $$ remains NULL
      } else {
        $$ = found_struct;
      }
      free($2); /* Free ID strdup'd by lexer */
    }
  ;

func_decl /* $$ is FuncInfo* (func_info_ptr) */
  : type_specifier pointers ID '(' param_list ')'
    {
      TypeInfo* return_base_type = $1;
      TypeInfo* final_return_type = return_base_type;
      for (int i = 0; i < $2; ++i) { // $2 is pointer count
        if (final_return_type == NULL) break; // Error from type_specifier
        final_return_type = create_pointer_type(final_return_type);
      }
      
      $$ = NULL;
      current_processing_func_info = NULL; 
      if (final_return_type) {
        // $3 is ID (function name), $5 is SymbolEntry* list of params from param_list
        $$ = create_func_info($3, final_return_type, $5);
        if ($$) {
          current_processing_func_info = $$; // Set global for compound_stmt
        }
        // Free the temporary parameter list created by param_list and param_decl rules
        struct SymbolEntry *p_iter = $5;
        while(p_iter) {
            struct SymbolEntry *next_p = p_iter->next;
            free(p_iter->name); // Name was strdup'd in param_decl
            // p_iter->type is not freed (it's part of FuncInfo now or global)
            free(p_iter); // SymbolEntry was malloc'd in param_decl
            p_iter = next_p;
        }
      } else {
        // If return type was bad, still need to free param list if it was parsed
        struct SymbolEntry *p_iter = $5;
        while(p_iter) {
            struct SymbolEntry *next_p = p_iter->next;
            free(p_iter->name);
            free(p_iter);
            p_iter = next_p;
        }
      }
      free($3); // Free function name ID (char*) from lexer
    }
  | type_specifier pointers ID '(' ')' /* No params */
    {
      TypeInfo* return_base_type = $1;
      TypeInfo* final_return_type = return_base_type;
      for (int i = 0; i < $2; ++i) {
        if (final_return_type == NULL) break;
        final_return_type = create_pointer_type(final_return_type);
      }
      $$ = NULL;
      current_processing_func_info = NULL;
      if (final_return_type) {
        $$ = create_func_info($3, final_return_type, NULL);
        if ($$) {
          current_processing_func_info = $$; // Set global
        }
      }
      free($3); // Free function name ID (char*) from lexer
    }
  ;

pointers
  : '*' pointers { $$ = $2 + 1; }
  | %empty { $$ = 0; }
  ;

param_list /* $$ is SymbolEntry* list (symbol_ptr) */
  : param_decl
    { $$ = $1; } // $1 is a SymbolEntry* from param_decl
  | param_list ',' param_decl
    {
      // $1 is existing SymbolEntry* list, $3 is new SymbolEntry* from param_decl
      if ($1 && $3) {
        struct SymbolEntry* tail = $1;
        while(tail->next) tail = tail->next; // Find end of current list
        tail->next = $3; // Append new parameter
        $3->next = NULL; // Ensure new parameter is the tail
        $$ = $1; // Return head of the updated list
      } else if ($3) { 
        // $1 was NULL (e.g. error in previous param_decl in a longer list)
        // or this is the first valid item after errors.
        $$ = $3;
        $3->next = NULL;
      } else { 
        // $3 was NULL (error in current param_decl)
        $$ = $1; // Return previous list, error in $3 is lost unless handled in param_decl
      }
    }
  ;		

param_decl /* $$ is SymbolEntry* (symbol_ptr) */
  : type_specifier pointers ID
    {
      TypeInfo *base_type = $1;
      TypeInfo *final_type = base_type;
      for (int i = 0; i < $2; ++i) { // $2 is pointer count
        if (final_type == NULL) break; // Propagate error from type_specifier
        final_type = create_pointer_type(final_type);
      }
      $$ = NULL;
      if (final_type) {
        // Create a SymbolEntry, but don't add to any scope here.
        // This list is temporary for FuncInfo creation.
        $$ = (SymbolEntry*)malloc(sizeof(SymbolEntry));
        if (!$$) { fprintf(stderr, "Out of memory for param_decl SymbolEntry\n"); exit(1); }
        $$->name = strdup($3); // strdup ID
        if (!$$->name) { fprintf(stderr, "Out of memory for param_decl name\n"); free($$); exit(1); }
        $$->type = final_type;
        $$->level = 0; // Level for params, will be adjusted or used when inserting into function scope
        $$->next = NULL;
        free($3); // Free original ID from lexer ($3 is char*)
      } else {
        free($3); // Still need to free ID if type was bad
      }
    }
  | type_specifier pointers ID '[' INTEGER_CONST ']' /* Param array */
    {
      TypeInfo *base_type = $1;
      TypeInfo *element_type = base_type;
      for (int i = 0; i < $2; ++i) { // $2 is pointer count
        if (element_type == NULL) break;
        element_type = create_pointer_type(element_type);
      }
      $$ = NULL;
      if (element_type) {
        TypeInfo *array_type = create_array_type(element_type, $5); 
        $$ = (SymbolEntry*)malloc(sizeof(SymbolEntry));
        if (!$$) { fprintf(stderr, "Out of memory for param_decl array SymbolEntry\n"); exit(1); }
        $$->name = strdup($3);
        if (!$$->name) { fprintf(stderr, "Out of memory for param_decl array name\n"); free($$); exit(1); }
        $$->type = array_type;
        $$->level = 0; 
        $$->next = NULL;
        free($3); // Free original ID from lexer
      } else {
        free($3); // Still need to free ID if type was bad
      }
    }
  ;

def_list
  : def_list def
    {
      // Chain the symbols. If $1 (previous list) is NULL, $2 is the new head.
      // If $2 (current def) is NULL (due to error or it's not returning symbol), list doesn't change.
      if ($2 != NULL) {
        if ($1 == NULL) {
          $$ = $2;
        } else {
          // This assumes SymbolEntry has a 'next' field suitable for this list.
          // For symbol table structure, sym_insert handles the 'next' in scope.
          // This specific chaining is more for lists like struct fields or parameters.
          // For local variable declarations, sym_insert already adds to current scope.
          // So, for 'def_list' in 'compound_stmt', '$$' might not be strictly needed
          // if 'def' directly inserts into the current scope.
          // However, for struct fields, this list construction is crucial.
          // Let's assume 'SymbolEntry->next' is used for this ad-hoc list building.
          // It's critical that this 'next' usage doesn't conflict with 'next' pointer usage
          // within the symbol table's hash chain or scope list.
          // The SymbolEntry->next is for linking symbols in the same scope bucket (hash collision)
          // or for linking parameters in a function's SymbolEntry list.
          // For struct fields, this is appropriate.
          struct SymbolEntry* current = $1;
          while(current->next) current = current->next; // Find end of list $1
          current->next = $2; // Append $2
          $2->next = NULL; // Ensure the appended symbol is the new tail for this list
          $$ = $1;
        }
      } else {
        $$ = $1;
      }
    }
  | %empty
    { $$ = NULL; }
  ;

def
  : type_specifier pointers ID ';'
    {
      TypeInfo *base_type = $1;
      TypeInfo *final_type = base_type;
      for (int i = 0; i < $2; ++i) { // $2 is count from pointers
        if (final_type == NULL) break; // Propagate error if base_type was NULL
        final_type = create_pointer_type(final_type);
      }
      
      $$ = NULL; // Default to NULL
      if (final_type) {
        SymbolEntry* sym = sym_insert($3, final_type);
        if (!sym) {
          error_redeclaration(); // Uses error func from subc.y
        } else {
          /* Only build a field list when defining struct fields */
          if (building_struct_fields) {
            /* Create a shallow copy so that manipulating field list does not
             * disturb the symbol table's 'next' links. */
            SymbolEntry *copy = (SymbolEntry *)malloc(sizeof(SymbolEntry));
            copy->name  = strdup(sym->name);
            copy->type  = sym->type;
            copy->level = sym->level;
            copy->next  = NULL;
            $$ = copy;
          } else {
            $$ = NULL; /* For ordinary variable declarations def_list is unused */
          }
        }
      } else {
        // Error already signaled by type_specifier if base_type is NULL
      }
      free($3); // ID stringVal is strdup'd by lexer, free it after use
    }
  | type_specifier pointers ID '[' INTEGER_CONST ']' ';'
    {
      TypeInfo *base_type = $1;
      TypeInfo *element_type = base_type;
      for (int i = 0; i < $2; ++i) { // $2 is count from pointers
        if (element_type == NULL) break;
        element_type = create_pointer_type(element_type);
      }

      $$ = NULL; // Default to NULL
      if (element_type) {
        TypeInfo *array_type = create_array_type(element_type, $5);
        SymbolEntry* sym = sym_insert($3, array_type);
        if (!sym) {
          error_redeclaration();
        } else {
          if (building_struct_fields) {
            SymbolEntry *copy = (SymbolEntry *)malloc(sizeof(SymbolEntry));
            copy->name  = strdup(sym->name);
            copy->type  = sym->type;
            copy->level = sym->level;
            copy->next  = NULL;
            $$ = copy;
          } else {
            $$ = NULL;
          }
        }
      } else {
        // Error already signaled
      }
      free($3); // ID stringVal is strdup'd by lexer, free it after use
    }
  ;

compound_stmt
  : '{'
    {
      enter_scope();
      // If this compound_stmt is a function body, current_processing_func_info will be set.
      if (current_processing_func_info != NULL) {
        // Set current_function_return_type for 'return' statement checks within this block
        current_function_return_type = current_processing_func_info->return_type;

        // Add parameters to the new scope
        if (current_processing_func_info->parameters != NULL) {
          SymbolEntry* param_iter = current_processing_func_info->parameters; // These are copies from deep_copy_symbol_list
          while(param_iter) {
            // sym_insert will strdup name and use the type.
            // The SymbolEntry 'param_iter' itself is part of FuncInfo's list, not directly inserted.
            SymbolEntry* inserted_param = sym_insert(param_iter->name, param_iter->type);
            if (!inserted_param) {
              // This implies a re-declaration of param name if sym_insert checks current scope only,
              // or param name conflicts with another symbol. This should ideally not happen if parameters
              // are unique and scope is fresh. Could also be memory error in sym_insert.
              fprintf(stderr, "%s:%d: Compiler error: Failed to insert parameter '%s' into function scope.\n", current_filename, get_lineno(), param_iter->name);
              // error_redeclaration(); // Or a more specific error
            }
            param_iter = param_iter->next;
          }
        }
      } else {
        // Not a function body, ensure current_function_return_type is NULL (or inherited from outer func if nested funcs were allowed)
        // For SubC, no nested functions, so it should be NULL if not in a function body.
        // It's cleared by ext_def after a function, so this path is for non-function compound statements.
        // current_function_return_type = NULL; // This might override if it's a block within a function.
        // Let current_function_return_type persist if already set by an outer function scope.
        // It is reset to NULL only when exiting a function's ext_def.
      }
    }
    def_list stmt_list
    '}'
    {
      leave_scope();
      // current_function_return_type is reset by the ext_def rule for the function.
      // If this compound_stmt is NOT a function body but IS INSIDE a function, 
      // current_function_return_type should remain as it was.
      // If this compound_stmt IS a function body, current_processing_func_info is cleared by ext_def.
    }
  ;

stmt_list
  : stmt_list stmt 
  | %empty 
  ;

stmt
  : expr ';' /* $1 is ETI* from expr */
    { if ($1) free_extended_type_info($1); }
  | RETURN expr ';' /* $2 is ETI* from expr */
    {
      if (!current_function_return_type) {
        // This case implies a 'return' statement is somehow parsed outside a function,
        // or current_function_return_type was not set.
        // This might indicate a logic error in how current_function_return_type is managed
        // or a grammar that allows RETURN in an invalid place.
        // For now, we'll assume if this is hit, it's an issue.
        // A robust compiler might have a specific error like "return statement not in function context".
        // error_preamble(); fprintf(stderr, "Return statement outside function context or function return type unknown.\n");
      } else if (!$2 || !$2->type) {
        // Error propagated from expr, or expr is invalid. Error message already printed.
        // $$ of expr is an ETI with type NULL.
      } else if (!are_types_equal(current_function_return_type, $2->type)) {
        // Check also if $2->type is NULL (error from expr) before are_types_equal
        // The condition !$2->type above handles this.
        error_return(); // "incompatible return types"
      }
      // If checks pass, no specific action needed for $$ of stmt.
      if ($2) free_extended_type_info($2);
    }
  | BREAK ';' { /* No semantic action needed beyond parsing */ }
  | CONTINUE ';' { /* No semantic action needed beyond parsing */ }
  | ';' { /* Empty statement, no semantic action */ }
  | compound_stmt { /* Handled by its own rules, including scope and var defs */ }
  | IF '(' expr ')' stmt /* $3 is ETI* for expr */
    {
      if ($3 && $3->type && $3->type->kind != TYPE_KIND_INT) {
        error_condition_type();
      }
      if ($3) free_extended_type_info($3);
    }
  | IF '(' expr ')' stmt ELSE stmt /* $3 is ETI* for expr */
    {
      if ($3 && $3->type && $3->type->kind != TYPE_KIND_INT) {
        error_condition_type();
      }
      if ($3) free_extended_type_info($3);
    }
  | WHILE '(' expr ')' stmt /* $3 is ETI* for expr */
    {
      if ($3 && $3->type && $3->type->kind != TYPE_KIND_INT) {
        error_condition_type();
      }
      if ($3) free_extended_type_info($3);
    }
  | FOR '(' expr_e ';' expr_e ';' expr_e ')' stmt
    /* $3 is init ETI*, $5 is cond ETI*, $7 is incr ETI* */
    {
      if ($3) free_extended_type_info($3); // Init expr's ETI, type not constrained by FOR itself

      // Condition expr ($5) must be int type if it exists and is valid
      if ($5 && $5->type && $5->type->kind != TYPE_KIND_INT) {
        error_condition_type();
      }
      if ($5) free_extended_type_info($5); // Free cond expr's ETI

      if ($7) free_extended_type_info($7); // Increment expr's ETI, type not constrained by FOR
    }
  ;

expr_e /* $$ is ExtendedTypeInfo* (type_info_ptr) */
  : expr 
    { $$ = $1; } // Pass through from expr
  | %empty 
    { $$ = NULL; } // No expression yields NULL ETI pointer
  ;

expr /* $$ is ExtendedTypeInfo* (type_info_ptr) */
  : unary '=' expr /* $1 is LHS (ETI*), $3 is RHS (ETI*) */
    {
      // Initialize $$ to an error state (r-value with NULL type)
      // This will be the result if any checks fail.
      $$ = create_extended_type_info(NULL, 0);

      // Check for validity of operands first
      if (!$1 || !$3 ) { // If ETI pointers themselves are NULL (e.g. from parse error)
        // Error already printed by sub-rules or YYERROR was called.
        // Free $$ as it's an error.
        free_extended_type_info($$);
        $$ = NULL; // Result is NULL ETI pointer
        if ($1) free_extended_type_info($1);
        if ($3) free_extended_type_info($3);
        // No YYERROR, allow parsing to continue if possible with NULL ETI*
      } else if ($1->is_error || $3->is_error || !$1->type || !$3->type) { /* propagate existing error */
        // Error already printed by sub-rules. Propagate error type.
        // $$ is already create_extended_type_info(NULL, 0)
        free_extended_type_info($1);
        free_extended_type_info($3);
      } else {
        // Proceed with semantic checks for assignment
        // 1. LHS must be lvalue
        if (!$1->is_lvalue) {
          error_assignable();
          $$->is_error = 1; /* mark propagated error */
        }
        // 2. NULL assignment check (RHS is NULL literal)
        else if ($3->type->kind == TYPE_KIND_NULL) {
          if ($1->type->kind != TYPE_KIND_POINTER && $1->type->kind != TYPE_KIND_ARRAY) { // Allow NULL to array pointer decay? For now, only pointer.
            error_null();
            $$->is_error = 1;
          } else { // Valid NULL assignment to pointer
            $$->type = $1->type; // Type of expression is type of LHS
            $$->is_lvalue = 0;   // Result of assignment is rvalue
          }
        }
        // 3. Type equality check (excluding NULL case handled above)
        else if (!are_types_equal($1->type, $3->type)) {
          error_incompatible();
          $$->is_error = 1;
        }
        // 4. Valid assignment
        else {
          // Type of assignment expression is the type of the left operand
          $$->type = $1->type;
          // Result of an assignment expression is an r-value
          $$->is_lvalue = 0;
        }
        // Free the ETI wrappers for LHS and RHS operands
        free_extended_type_info($1);
        free_extended_type_info($3);
      }
    }
  | binary
    { $$ = $1; } // Pass through from binary expression
  ;

binary /* $$ is ExtendedTypeInfo* (type_info_ptr) */
    /* Arithmetic Operations */
  : binary '+' binary
    {
      $$ = create_extended_type_info(NULL, 0); // Default error ETI
      if (!$1 || !$1->type || !$3 || !$3->type) {
        // Error in operands, already reported. $$ remains error ETI.
      } else if ($1->type->kind == TYPE_KIND_INT && $3->type->kind == TYPE_KIND_INT) {
        $$->type = TYPE_INT;
        $$->is_lvalue = 0;
      } else {
        error_binary(); // Invalid operand types for +
      }
      if ($1) free_extended_type_info($1);
      if ($3) free_extended_type_info($3);
    }
  | binary '-' binary
    {
      $$ = create_extended_type_info(NULL, 0);
      if (!$1 || !$1->type || !$3 || !$3->type) { } // Propagate error
      else if ($1->type->kind == TYPE_KIND_INT && $3->type->kind == TYPE_KIND_INT) {
        $$->type = TYPE_INT; $$->is_lvalue = 0;
      } else { error_binary(); }
      if ($1) free_extended_type_info($1);
      if ($3) free_extended_type_info($3);
    }
  | binary '*' binary
    {
      $$ = create_extended_type_info(NULL, 0);
      if (!$1 || !$1->type || !$3 || !$3->type) { } // Propagate error
      else if ($1->type->kind == TYPE_KIND_INT && $3->type->kind == TYPE_KIND_INT) {
        $$->type = TYPE_INT; $$->is_lvalue = 0;
      } else { error_binary(); }
      if ($1) free_extended_type_info($1);
      if ($3) free_extended_type_info($3);
    }
  | binary '/' binary
    {
      $$ = create_extended_type_info(NULL, 0);
      if (!$1 || !$1->type || !$3 || !$3->type) { } // Propagate error
      else if ($1->type->kind == TYPE_KIND_INT && $3->type->kind == TYPE_KIND_INT) {
        $$->type = TYPE_INT; $$->is_lvalue = 0;
      } else { error_binary(); }
      if ($1) free_extended_type_info($1);
      if ($3) free_extended_type_info($3);
    }
  | binary '%' binary
    {
      $$ = create_extended_type_info(NULL, 0);
      if (!$1 || !$1->type || !$3 || !$3->type) { } // Propagate error
      else if ($1->type->kind == TYPE_KIND_INT && $3->type->kind == TYPE_KIND_INT) {
        $$->type = TYPE_INT; $$->is_lvalue = 0;
      } else { error_binary(); }
      if ($1) free_extended_type_info($1);
      if ($3) free_extended_type_info($3);
    }

    /* Logical Operations */
  | binary LOGICAL_AND binary
    {
      $$ = create_extended_type_info(NULL, 0);
      if (!$1 || !$1->type || !$3 || !$3->type) { } // Propagate error
      else if ($1->type->kind == TYPE_KIND_INT && $3->type->kind == TYPE_KIND_INT) {
        $$->type = TYPE_INT; $$->is_lvalue = 0; // Result is 0 or 1
      } else { error_binary(); /* Reusing error_binary as per typical C behavior (expects integers) */ }
      if ($1) free_extended_type_info($1);
      if ($3) free_extended_type_info($3);
    }
  | binary LOGICAL_OR binary
    {
      $$ = create_extended_type_info(NULL, 0);
      if (!$1 || !$1->type || !$3 || !$3->type) { } // Propagate error
      else if ($1->type->kind == TYPE_KIND_INT && $3->type->kind == TYPE_KIND_INT) {
        $$->type = TYPE_INT; $$->is_lvalue = 0; // Result is 0 or 1
      } else { error_binary(); }
      if ($1) free_extended_type_info($1);
      if ($3) free_extended_type_info($3);
    }

    /* Relational Operations */
  | binary RELOP binary // RELOP includes <, <=, >, >=
    {
      $$ = create_extended_type_info(NULL, 0);
      if (!$1 || !$1->type || !$3 || !$3->type) { } // Propagate error
      else {
        TypeInfo *t1 = $1->type;
        TypeInfo *t2 = $3->type;
        if ((t1->kind == TYPE_KIND_INT && t2->kind == TYPE_KIND_INT) ||
            (t1->kind == TYPE_KIND_CHAR && t2->kind == TYPE_KIND_CHAR)) {
          $$->type = TYPE_INT; // Result of relational op is int (0 or 1)
          $$->is_lvalue = 0;
        } else {
          error_comparable(); // Operands not comparable or not of allowed types
        }
      }
      if ($1) free_extended_type_info($1);
      if ($3) free_extended_type_info($3);
    }

    /* Equality Operations */
  | binary EQUOP binary // EQUOP includes ==, !=
    {
      $$ = create_extended_type_info(NULL, 0);
      if (!$1 || !$1->type || !$3 || !$3->type) { } // Propagate error
      else {
        TypeInfo *t1 = $1->type;
        TypeInfo *t2 = $3->type;
        int compatible = 0;
        if ((t1->kind == TYPE_KIND_INT && t2->kind == TYPE_KIND_INT) ||
            (t1->kind == TYPE_KIND_CHAR && t2->kind == TYPE_KIND_CHAR)) {
          compatible = 1;
        } else if (t1->kind == TYPE_KIND_POINTER && t2->kind == TYPE_KIND_POINTER) {
          compatible = 1; // Any two pointer types can be compared for equality
        } else if (t1->kind == TYPE_KIND_POINTER && t2->kind == TYPE_KIND_NULL) {
          compatible = 1;
        } else if (t1->kind == TYPE_KIND_NULL && t2->kind == TYPE_KIND_POINTER) {
          compatible = 1;
        }
        // Array types are not directly comparable with EQUOP, they decay to pointers implicitly
        // if passed to functions or in some expressions, but `array1 == array2` is not standard C.
        // The ID rule already marks array names as non-lvalue, but they can be rvalues.
        // For now, this doesn't explicitly block array types if they somehow reach here and aren't pointers.
        // However, sym_lookup provides their type, and they aren't INT/CHAR/POINTER/NULL.

        if (compatible) {
          $$->type = TYPE_INT; // Result of equality op is int (0 or 1)
          $$->is_lvalue = 0;
        } else {
          error_comparable();
        }
      }
      if ($1) free_extended_type_info($1);
      if ($3) free_extended_type_info($3);
    }
  | unary %prec '=' /* Base case: promote unary to binary with '=' precedence */
    { $$ = $1; } // Pass through the ExtendedTypeInfo* from unary
  ;

unary
  : '(' expr ')'
    { $$ = $2; } // Pass through ExtendedTypeInfo*
  // | '(' unary ')' // This rule is redundant if expr covers unary. Remove.
  | INTEGER_CONST
    { $$ = create_extended_type_info(TYPE_INT, 0); }
  | CHAR_CONST
    { $$ = create_extended_type_info(TYPE_CHAR, 0); }
  | STRING
    { $$ = create_extended_type_info(TYPE_CHAR_POINTER, 0); } // Using the new global type
  | ID  /* ID yylval is stringVal ($1) */
    {
      SymbolEntry* sym = sym_lookup($1);
      if (!sym) {
        error_undeclared();
        $$ = create_extended_type_info(NULL, 0);
        $$->is_error = 1;
      } else {
        int is_lvalue = 0;
        // Array names and function names are not lvalues.
        // Struct variables are lvalues. Pointers, int, char variables are lvalues.
        if (sym->type && (sym->type->kind == TYPE_KIND_ARRAY || sym->type->kind == TYPE_KIND_FUNCTION)) {
           is_lvalue = 0;
        } else {
           is_lvalue = 1; // Variables (int, char, struct instance, pointer) are lvalues
        }
        $$ = create_extended_type_info(sym->type, is_lvalue);
        $$->is_error = 0;
      }
      free($1); // Free the string from lexer's strdup
    }
  | '-' unary %prec '!' /* $2 is ETI* for unary */
    {
      $$ = create_extended_type_info(NULL, 0); // Default error
      if (!$2 || !$2->type) {
        // Error propagated from operand
      } else if ($2->type->kind == TYPE_KIND_INT) {
        $$->type = TYPE_INT;
        $$->is_lvalue = 0;
      } else {
        error_unary(); // Invalid type for unary minus
      }
      if ($2) free_extended_type_info($2);
    }
  | '!' unary /* $2 is ETI* for unary */
    {
      $$ = create_extended_type_info(NULL, 0); // Default error
      if (!$2 || !$2->type) { /* Propagate error from operand */ }
      else if ($2->type->kind == TYPE_KIND_INT) { // Logical not expects int (boolean)
        $$->type = TYPE_INT; // Result is int (0 or 1)
        $$->is_lvalue = 0;
      } else { error_unary(); } // Invalid type for logical not
      if ($2) free_extended_type_info($2);
    }
  | unary INCOP %prec STRUCTOP /* $1 is ETI* for unary (postfix) */
    {
      $$ = create_extended_type_info(NULL, 0); // Default error
      if (!$1 || !$1->type) { /* Propagate error */ }
      else if (!$1->is_lvalue) {
        error_assignable(); // Operand of ++ must be lvalue
      } else if ($1->type->kind == TYPE_KIND_INT || $1->type->kind == TYPE_KIND_CHAR) {
        $$->type = $1->type; // Type of expr is operand's type
        $$->is_lvalue = 0;   // Postfix ++ result is rvalue (value before increment)
      } else {
        error_unary(); // Invalid type for ++
      }
      if ($1) free_extended_type_info($1);
    }
  | unary DECOP %prec STRUCTOP /* $1 is ETI* for unary (postfix) */
    {
      $$ = create_extended_type_info(NULL, 0);
      if (!$1 || !$1->type) { /* Propagate error */ }
      else if (!$1->is_lvalue) {
        error_assignable(); // Operand of -- must be lvalue
      } else if ($1->type->kind == TYPE_KIND_INT || $1->type->kind == TYPE_KIND_CHAR) {
        $$->type = $1->type;
        $$->is_lvalue = 0;   // Postfix -- result is rvalue
      } else {
        error_unary(); // Invalid type for --
      }
      if ($1) free_extended_type_info($1);
    }
  | INCOP unary %prec '!' /* $2 is ETI* for unary (prefix) */
    {
      $$ = create_extended_type_info(NULL, 0);
      if (!$2 || !$2->type) { /* Propagate error */ }
      else if (!$2->is_lvalue) {
        error_assignable(); // Operand of ++ must be lvalue
      } else if ($2->type->kind == TYPE_KIND_INT || $2->type->kind == TYPE_KIND_CHAR) {
        $$->type = $2->type;
        $$->is_lvalue = 1;   // Prefix ++ result is lvalue
      } else {
        error_unary(); // Invalid type for ++
      }
      if ($2) free_extended_type_info($2);
    }
  | DECOP unary %prec '!' /* $2 is ETI* for unary (prefix) */
    {
      $$ = create_extended_type_info(NULL, 0);
      if (!$2 || !$2->type) { /* Propagate error */ }
      else if (!$2->is_lvalue) {
        error_assignable(); // Operand of -- must be lvalue
      } else if ($2->type->kind == TYPE_KIND_INT || $2->type->kind == TYPE_KIND_CHAR) {
        $$->type = $2->type;
        $$->is_lvalue = 1;   // Prefix -- result is lvalue
      } else {
        error_unary(); // Invalid type for --
      }
      if ($2) free_extended_type_info($2);
    }
  | '&' unary /* $2 is ETI* for unary (Address-of operator) */
    {
      $$ = create_extended_type_info(NULL, 0); // Default error
      if (!$2 || !$2->type) { /* Propagate error */ }
      else if (!$2->is_lvalue) { // Operand of & must be an lvalue
        error_addressof();
      } else {
        $$->type = create_pointer_type($2->type);
        $$->is_lvalue = 0; // Result of & operator is an rvalue
        // create_pointer_type exits on memory error, so no NULL check needed for $$->type here.
      }
      if ($2) free_extended_type_info($2);
    }
  | '*' unary %prec '!' /* $2 is ETI* for unary (Dereference operator) */
    {
      $$ = create_extended_type_info(NULL, 0); // Default error
      if (!$2 || !$2->type) { /* Propagate error */ }
      else if ($2->type->kind == TYPE_KIND_POINTER) {
        $$->type = $2->type->info.pointer_to;
        $$->is_lvalue = 1; // Result of * (dereference) is an lvalue
        if (!$$->type) { // Should not happen if pointer_to is valid
            error_indirection(); // Or some other internal error
            $$->type = NULL; // Ensure error state
            $$->is_lvalue = 0;
        }
      } else {
        error_indirection(); // Operand must be a pointer
      }
      if ($2) free_extended_type_info($2);
    }
  | unary '[' expr ']' /* $1 ETI* for unary (array), $3 ETI* for expr (index) */
    {
      $$ = create_extended_type_info(NULL, 0); // Default error state
      if (!$1 || !$1->type || !$3 || !$3->type) { 
        /* Propagated error from operands, $$ remains error ETI */
        if ($1 && !$1->type) { /* Error was in $1 */ }
        else if ($3 && !$3->type) { /* Error was in $3 */ }
        // If $1 or $3 ETIs themselves are NULL, it's a parse error or earlier issue.
      } else if ($1->type->kind != TYPE_KIND_ARRAY) {
        error_array(); // "subscripted value is not an array"
      } else if ($3->type->kind != TYPE_KIND_INT) {
        error_subscript(); // "array subscript is not an integer"
      } else {
        $$->type = $1->type->info.array_info.element_type;
        // Element of an array is an lvalue, unless the array itself is a const array (not in SubC)
        // or if the element type is function/array (which also become non-lvalue if assigned to directly)
        // For SubC, array elements are generally lvalues.
        if ($$->type && ($$->type->kind == TYPE_KIND_ARRAY || $$->type->kind == TYPE_KIND_FUNCTION)) {
            $$->is_lvalue = 0; // If element itself is an array/function, it's not assignable directly
        } else {
            $$->is_lvalue = 1; 
        }
      }
      if ($1) free_extended_type_info($1);
      if ($3) free_extended_type_info($3);
    }
  | unary '.' ID /* $1 ETI* for unary (struct), $3 char* for ID (field name) */
    {
      $$ = create_extended_type_info(NULL, 0); // Default error state
      if (!$1 || !$1->type) { 
        /* Propagated error from struct expression, $$ remains error ETI */
      } else if ($1->type->kind != TYPE_KIND_STRUCT) {
        error_struct(); // "member reference base type is not a struct"
      } else {
        SymbolEntry* field = NULL;
        // Ensure struct_info.fields is valid before iterating
        if ($1->type->info.struct_info.fields) {
            SymbolEntry* field_list = $1->type->info.struct_info.fields;
            while (field_list) {
                if (field_list->name && strcmp(field_list->name, $3) == 0) {
                    field = field_list;
                    break;
                }
                field_list = field_list->next;
            }
        }
        if (!field) {
          error_member(); // "no such member in struct"
        } else {
          $$->type = field->type;
          // If the struct itself ($1) is an lvalue, then its member is also an lvalue.
          // Exception: if the field type is an array or function.
          if ($1->is_lvalue && field->type && 
              !(field->type->kind == TYPE_KIND_ARRAY || field->type->kind == TYPE_KIND_FUNCTION)) {
            $$->is_lvalue = 1;
          } else {
            $$->is_lvalue = 0; // Member of rvalue struct, or field is array/function
          }
        }
      }
      if ($1) free_extended_type_info($1);
      free($3); // Free ID string from lexer ($3 is char*)
    }
  | unary STRUCTOP ID /* $1 ETI* for unary (struct ptr), $3 char* for ID (field name) */
    {
      $$ = create_extended_type_info(NULL, 0); // Default error state
      if (!$1 || !$1->type) { /* Propagated error */ }
      else if (!($1->type->kind == TYPE_KIND_POINTER && 
                 $1->type->info.pointer_to && 
                 $1->type->info.pointer_to->kind == TYPE_KIND_STRUCT)) {
        error_strurctp(); // "member reference base type is not a struct pointer"
      } else {
        SymbolEntry* field = NULL;
        // Ensure struct_info.fields is valid before iterating
        if ($1->type->info.pointer_to->info.struct_info.fields) {
            SymbolEntry* field_list = $1->type->info.pointer_to->info.struct_info.fields;
            while (field_list) {
                if (field_list->name && strcmp(field_list->name, $3) == 0) {
                    field = field_list;
                    break;
                }
                field_list = field_list->next;
            }
        }
        if (!field) {
          error_member(); // "no such member in struct"
        } else {
          $$->type = field->type;
          // Result of -> operator is always an lvalue, unless the field itself is an array/function.
          if (field->type && 
              !(field->type->kind == TYPE_KIND_ARRAY || field->type->kind == TYPE_KIND_FUNCTION)) {
            $$->is_lvalue = 1;
          } else {
            $$->is_lvalue = 0;
          }
        }
      }
      if ($1) free_extended_type_info($1);
      free($3); // Free ID string from lexer ($3 is char*)
    }
  | unary '(' args ')' /* $1 ETI* (function), $3 SymbolEntry* (actual_args list) */
    {
      $$ = create_extended_type_info(NULL, 0); // Default error state

      if (!$1 || !$1->type) { /* Propagated error from function expression part */ }
      else if ($1->type->kind != TYPE_KIND_FUNCTION) {
        error_function(); // "not a function"
      } else {
        TypeInfo* func_type = $1->type;
        SymbolEntry* formal_params = func_type->info.function_info.parameters;
        SymbolEntry* actual_args = $3; // This is the list from the 'args' rule
        int compatible = 1;

        while (formal_params && actual_args) {
          if (!actual_args->type) { // Error propagated in an argument expression
            compatible = 0; break;
          }
          if (!are_types_equal(formal_params->type, actual_args->type)) {
            compatible = 0; break;
          }
          formal_params = formal_params->next;
          actual_args = actual_args->next;
        }
        if (formal_params || actual_args) { // Mismatch in number of arguments
          compatible = 0;
        }

        if (!compatible) {
          error_arguments(); // "incompatible arguments in function call"
        } else {
          $$->type = func_type->info.function_info.return_type;
          $$->is_lvalue = 0; // Result of a function call is an rvalue
        }
      }
      // Free the ETI for the function expression
      if ($1) free_extended_type_info($1);
      // Free the temporary list of SymbolEntry used for actual_args
      SymbolEntry* arg_iter = $3;
      while(arg_iter) {
        SymbolEntry* next_arg = arg_iter->next;
        // arg_iter->type points to a type from an expression, not freed here.
        free(arg_iter); // Free the SymbolEntry wrapper for the arg type
        arg_iter = next_arg;
      }
    }
  | unary '(' ')' /* $1 ETI* (function), no actual_args */
    {
      $$ = create_extended_type_info(NULL, 0); // Default error state
      if (!$1 || !$1->type) { /* Propagated error */ }
      else if ($1->type->kind != TYPE_KIND_FUNCTION) {
        error_function(); // "not a function"
      } else {
        TypeInfo* func_type = $1->type;
        if (func_type->info.function_info.parameters != NULL) {
          // Function expected arguments, but none were provided.
          error_arguments();
        } else {
          // Function takes no arguments, and none were provided. Correct.
          $$->type = func_type->info.function_info.return_type;
          $$->is_lvalue = 0;
        }
      }
      if ($1) free_extended_type_info($1);
    }
  | SYM_NULL
    { $$ = create_extended_type_info(TYPE_NULLPTR, 0); }
  ;

args /* $$ is SymbolEntry* list, where name can be null, type holds arg type */
  : expr /* $1 is ExtendedTypeInfo* */
    {
      $$ = NULL;
      if ($1 && $1->type) {
        $$ = (SymbolEntry*)malloc(sizeof(SymbolEntry));
        if (!$$) {fprintf(stderr, "Out of memory for arg list node\n"); exit(1);}
        $$->name = NULL; // No name for evaluated argument expression
        $$->type = $1->type; // Store the type of the expression
        $$->level = 0; // Not strictly needed for this temporary list
        $$->next = NULL;
      } else if ($1 && !$1->type) { // Propagated error from expr
          // $$ remains NULL
      }
      if ($1) free_extended_type_info($1); // Free the ETI wrapper for the expression
    }
  | args ',' expr /* $1 is SymbolEntry* list, $3 is ExtendedTypeInfo* */
    {
      $$ = $1; // Start with the existing list (could be NULL if first arg had error)
      if ($3 && $3->type) {
        SymbolEntry* new_arg = (SymbolEntry*)malloc(sizeof(SymbolEntry));
        if (!new_arg) {fprintf(stderr, "Out of memory for arg list node\n"); exit(1);}
        new_arg->name = NULL;
        new_arg->type = $3->type;
        new_arg->level = 0;
        new_arg->next = NULL;

        if (!$$) { // If $1 (previous args) was NULL due to error or it's the first one
          $$ = new_arg;
        } else {
          SymbolEntry* tail = $$;
          while(tail->next) tail = tail->next;
          tail->next = new_arg;
        }
      } else if ($3 && !$3->type) { // Propagated error from expr
          // $$ remains $1 (previous list, possibly with errors or NULL)
      }
      if ($3) free_extended_type_info($3);
    }
  ;

%%

/* Epilogue section - semantic error helpers are implemented in new_file/error.c */
#if 0

// Print the preamble of error message.
void error_preamble(void) {
  // Ensure current_filename is not NULL to prevent crashes, though it should always be set.
  if (current_filename != NULL) {
    fprintf(stderr, "%s:%d: error: ", current_filename, get_lineno());
  } else {
    // Fallback if current_filename is somehow NULL
    fprintf(stderr, "unknown_file:%d: error: ", get_lineno());
  }
}

void error_undeclared(void) {
  error_preamble();
  fprintf(stderr, "use of undeclared identifier\n");
}

void error_redeclaration(void) {
  error_preamble();
  fprintf(stderr, "redeclaration\n");
}

void error_assignable(void) {
  error_preamble();
  fprintf(stderr, "lvalue is not assignable\n");
}

void error_incompatible(void) {
  error_preamble();
  fprintf(stderr, "incompatible types for assignment operation\n");
}

void error_null(void) {
  error_preamble();
  fprintf(stderr, "cannot assign 'NULL' to non-pointer type\n");
}

void error_binary(void) {
  error_preamble();
  fprintf(stderr, "invalid operands to binary expression\n");
}

void error_unary(void) {
  error_preamble();
  fprintf(stderr, "invalid argument type to unary expression\n");
}

void error_comparable(void) {
  error_preamble();
  fprintf(stderr, "types are not comparable in binary expression\n");
}

void error_indirection(void) {
  error_preamble();
  fprintf(stderr, "indirection requires pointer operand\n");
}

void error_addressof(void) {
  error_preamble();
  fprintf(stderr, "cannot take the address of an rvalue\n");
}

void error_struct(void) {
  error_preamble();
  fprintf(stderr, "member reference base type is not a struct\n");
}

void error_strurctp(void){
  error_preamble();
  fprintf(stderr, "member reference base type is not a struct pointer\n");
}

void error_member(void) {
  error_preamble();
  fprintf(stderr, "no such member in struct\n");
}

void error_array(void) {
  error_preamble();
  fprintf(stderr, "subscripted value is not an array\n");
}

void error_subscript(void) {
  error_preamble();
  fprintf(stderr, "array subscript is not an integer\n");
}

void error_incomplete(void) {
  error_preamble();
  fprintf(stderr, "incomplete type\n");
}

void error_return(void) {
  error_preamble();
  fprintf(stderr, "incompatible return types\n");
}

void error_function(void) {
  error_preamble();
  fprintf(stderr, "not a function\n");
}

void error_arguments(void) {
  error_preamble();
  fprintf(stderr, "incompatible arguments in function call\n");
}

void error_condition_type(void) {
  error_preamble();
  fprintf(stderr, "condition expression must be of integer type\n");
}
#endif /* disable duplicate error helpers */
