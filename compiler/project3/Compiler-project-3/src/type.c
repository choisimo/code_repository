/*
 * File Name    : type.c
 * Description  : Implementation of type helpers for Project-3 semantic analyser
 */

 #include "type.h"
 #include "symtab.h"   // For SymbolEntry definitions used in deep_copy
 #include <assert.h>
 #include "env.h"     // For current_filename maybe in deeper functions
 
 /* Ensure globals.c content is accessible for global lists */
 extern TypeInfo *global_struct_definitions;
 extern FuncInfo *global_function_definitions;
 
 /* Built-in singleton type instances */
 TypeInfo *TYPE_INT      = NULL;
 TypeInfo *TYPE_CHAR     = NULL;
 TypeInfo *TYPE_NULLPTR  = NULL;
 TypeInfo *TYPE_CHAR_POINTER = NULL;
 
 static TypeInfo *alloc_type(TypeKind kind)
 {
     TypeInfo *t = (TypeInfo *)malloc(sizeof(TypeInfo));
     if (!t) {
         fprintf(stderr, "Out of memory while allocating TypeInfo\n");
         exit(1);
     }
     t->kind = kind;
     /* zero the union to prevent uninitialised reads */
     memset(&t->info, 0, sizeof(t->info));
     t->next_global_struct_link = NULL; // Initialize the new field
     return t;
 }
 
 // Helper function to perform a deep copy of a SymbolEntry list (e.g., for struct fields)
 static SymbolEntry* deep_copy_symbol_list(SymbolEntry *src_list) {
     if (!src_list) return NULL;
     SymbolEntry *new_head = NULL, *new_tail = NULL;
     SymbolEntry* current_src_node = src_list;
     while (current_src_node) {
         SymbolEntry* new_sym = (SymbolEntry*)malloc(sizeof(SymbolEntry));
         if (!new_sym) {
             fprintf(stderr, "Out of memory in deep_copy_symbol_list for SymbolEntry\n");
             // Simplification: exit on critical memory failure. A real compiler might try to free partial list.
             exit(1); 
         }
         if (current_src_node->name) {
             new_sym->name = strdup(current_src_node->name);
             if (!new_sym->name) {
                 fprintf(stderr, "Out of memory in deep_copy_symbol_list for name strdup\n");
                 free(new_sym); 
                 exit(1); 
             }
         } else {
             new_sym->name = NULL; // Should not happen for valid struct fields
         }
         // Deep copy type if necessary? For now, TypeInfo is shared (often singletons or other shared refs)
         // If TypeInfo itself could be a complex, modifiable type specific to this symbol instance,
         // then a deep_copy_typeinfo would be needed. For SubC, existing type system reuses type instances.
         new_sym->type = current_src_node->type; 
         new_sym->level = current_src_node->level; // Level might need adjustment if used across scopes, but for fields it's fine.
         new_sym->next = NULL; // This 'next' is for the new list being built
 
         if (!new_head) {
             new_head = new_tail = new_sym;
         } else {
             new_tail->next = new_sym;
             new_tail = new_sym;
         }
         current_src_node = current_src_node->next;
     }
     return new_head;
 }
 
 void init_builtin_types(void)
 {
     if (TYPE_INT) return; /* already initialised */
     TYPE_INT     = alloc_type(TYPE_KIND_INT);
     TYPE_CHAR    = alloc_type(TYPE_KIND_CHAR);
     TYPE_NULLPTR = alloc_type(TYPE_KIND_NULL);
     TYPE_CHAR_POINTER = create_pointer_type(TYPE_CHAR); // Initialize TYPE_CHAR_POINTER
 }
 
 TypeInfo *create_type(TypeKind kind)
 {
     return alloc_type(kind);
 }
 
 TypeInfo *create_pointer_type(TypeInfo *base)
 {
     TypeInfo *t = alloc_type(TYPE_KIND_POINTER);
     t->info.pointer_to = base;
     return t;
 }
 
 TypeInfo *create_array_type(TypeInfo *element, int size)
 {
     TypeInfo *t = alloc_type(TYPE_KIND_ARRAY);
     t->info.array_info.element_type = element;
     t->info.array_info.array_size   = size;
     return t;
 }
 
 TypeInfo *create_struct_type(const char *name, SymbolEntry *fields) {
     if (!name) { 
         fprintf(stderr, "Error: struct name is NULL in create_struct_type\n");
         return NULL;
     }
     TypeInfo *t = alloc_type(TYPE_KIND_STRUCT); 
     // alloc_type initializes next_global_struct_link
 
     t->info.struct_info.name = strdup(name);
     if (!t->info.struct_info.name) {
         fprintf(stderr, "Out of memory for struct name strdup in create_struct_type\n");
         free(t); 
         return NULL;
     }
     t->info.struct_info.fields = deep_copy_symbol_list(fields);
     // If deep_copy_symbol_list failed (e.g. due to memory), it would have exited.
     // If it could return NULL without exiting, we'd need to check:
     // if (!t->info.struct_info.fields && fields != NULL) { free(t->info.struct_info.name); free(t); return NULL; }
     return t;
 }
 
 TypeInfo *create_function_type(TypeInfo *ret, SymbolEntry *params)
 {
     TypeInfo *t = alloc_type(TYPE_KIND_FUNCTION);
     t->info.function_info.return_type = ret;
     t->info.function_info.parameters  = params;
     return t;
 }
 
 static int compare_param_lists(const SymbolEntry *a,
                                const SymbolEntry *b);
 
 int are_types_equal(const TypeInfo *a, const TypeInfo *b)
 {
     if (a == b) return 1;              /* pointer equality fast-path */
     if (!a || !b) return 0;
     if (a->kind != b->kind) return 0;
 
     switch (a->kind) {
     case TYPE_KIND_INT:
     case TYPE_KIND_CHAR:
     case TYPE_KIND_NULL:
         return 1; /* built-ins are singletons or equivalent */
 
     case TYPE_KIND_POINTER:
         return are_types_equal(a->info.pointer_to, b->info.pointer_to);
 
     case TYPE_KIND_ARRAY:
         return a->info.array_info.array_size == b->info.array_info.array_size &&
                are_types_equal(a->info.array_info.element_type,
                                b->info.array_info.element_type);
 
     case TYPE_KIND_STRUCT:
         /* Project spec: even identical layout but different tag ⇒ different */
         return strcmp(a->info.struct_info.name, b->info.struct_info.name) == 0;
 
     case TYPE_KIND_FUNCTION:
         return are_types_equal(a->info.function_info.return_type,
                                b->info.function_info.return_type) &&
                compare_param_lists(a->info.function_info.parameters,
                                    b->info.function_info.parameters);
     default:
         return 0;
     }
 }
 
 /* Helper to compare two linked parameter lists. We treat (void) function() as
  * zero parameters (NULL list). */
 static int compare_param_lists(const SymbolEntry *a,
                                const SymbolEntry *b)
 {
     while (a && b) {
         if (!are_types_equal(a->type, b->type)) return 0;
         a = a->next;
         b = b->next;
     }
     return (a == NULL && b == NULL);
 }
 
 FuncInfo *create_func_info(const char *name, TypeInfo *return_type, SymbolEntry *params) {
     if (!name) {
         fprintf(stderr, "Error: function name is NULL in create_func_info\n");
         return NULL;
     }
     FuncInfo *fi = (FuncInfo *)malloc(sizeof(FuncInfo));
     if (!fi) {
         fprintf(stderr, "Out of memory while allocating FuncInfo\n");
         exit(1); 
     }
     fi->name = strdup(name);
     if (!fi->name) {
         fprintf(stderr, "Out of memory for func name strdup in create_func_info\n");
         free(fi);
         exit(1);
     }
     fi->return_type = return_type; 
     fi->parameters = deep_copy_symbol_list(params); 
     fi->next_global_func_link = NULL;
     return fi;
 }
 
 void free_func_info(FuncInfo *fi) {
     if (!fi) return;
     free(fi->name);
     
     SymbolEntry *p = fi->parameters;
     while(p) {
         SymbolEntry *next_p = p->next;
         free(p->name); // Name was strdup'd by deep_copy_symbol_list
         // p->type is not freed here as types are typically shared
         free(p);
         p = next_p;
     }
     // fi->return_type is not freed (shared)
     free(fi);
 }
 
 ExtendedTypeInfo *create_extended_type_info(TypeInfo *type, int is_lvalue) {
     ExtendedTypeInfo *eti = (ExtendedTypeInfo *)malloc(sizeof(ExtendedTypeInfo));
     if (!eti) {
         fprintf(stderr, "Out of memory for ExtendedTypeInfo\n");
         exit(1); // Critical error
     }
     eti->type = type;
     eti->is_lvalue = is_lvalue;
     eti->is_error = 0;
     return eti;
 }
 
 void free_extended_type_info(ExtendedTypeInfo *eti) {
     if (!eti) return;
     // eti->type is not typically freed here because it's usually a reference to a
     // shared TypeInfo (e.g., TYPE_INT, a type from symbol table, or TYPE_CHAR_POINTER).
     // If a TypeInfo was dynamically created *specifically and solely* for this ETI,
     // (e.g., if string literals didn't use TYPE_CHAR_POINTER and created a new TypeInfo each time),
     // then it would need to be freed here. With TYPE_CHAR_POINTER, this is not an issue.
     free(eti);
 }
 
/* Helper to free a list of SymbolEntry, including their names, but not their TypeInfo* type */
void free_symbol_list_contents(SymbolEntry *head) {
    SymbolEntry *current = head;
    while (current) {
        SymbolEntry *next = current->next;
        if (current->name) {
            free(current->name);
        }
        // Do NOT free current->type, as types are shared or managed globally
        free(current);
        current = next;
    }
}

void free_global_types_and_functions(void) {
    // Free global struct definitions
    TypeInfo *current_struct = global_struct_definitions;
    while (current_struct) {
        TypeInfo *next_struct = current_struct->next_global_struct_link;
        if (current_struct->info.struct_info.name) {
            free(current_struct->info.struct_info.name);
        }
        // The fields list was created by deep_copy_symbol_list, so its SymbolEntry nodes and their names need freeing.
        free_symbol_list_contents(current_struct->info.struct_info.fields);
        free(current_struct);
        current_struct = next_struct;
    }
    global_struct_definitions = NULL;

    // Free global function definitions
    FuncInfo *current_func = global_function_definitions;
    while (current_func) {
        FuncInfo *next_func = current_func->next_global_func_link;
        free_func_info(current_func); // free_func_info handles name and its parameter list (deep copied)
        current_func = next_func;
    }
    global_function_definitions = NULL;

    // Note: This does not free TYPE_INT, TYPE_CHAR, TYPE_NULLPTR, TYPE_CHAR_POINTER
    // as they are often considered permanent. If they also need freeing, add here.
    // It also doesn't free dynamically created pointer/array TypeInfo objects not in these global lists.
}
 