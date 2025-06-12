/*
 * File Name    : type.h
 * Description  : Type system definitions and helper routines for Project 3
 */

 #ifndef __TYPE_H__
 #define __TYPE_H__
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 /* Forward declaration to avoid circular dependency with symtab.h */
 struct SymbolEntry;
 
 /* Enumeration that represents every distinct kind of type handled by the
  * simplified C compiler used in Project-3. Keep the order stable because some
  * code may rely on the ordinal values being constant across invocations. */
 typedef enum {
     TYPE_KIND_INT,
     TYPE_KIND_CHAR,
     TYPE_KIND_NULL,   /* special sentinel representing the literal NULL */
     TYPE_KIND_POINTER,
     TYPE_KIND_ARRAY,
     TYPE_KIND_STRUCT,
     TYPE_KIND_FUNCTION
 } TypeKind;
 
 /* Forward declaration so we can reference pointers inside the structure. */
 struct TypeInfo;
 
 typedef struct TypeInfo TypeInfo;
 
 /* Primary data-structure that fully describes a type.  Depending on the
  * `kind` field different members of the anonymous union become valid. */
 struct TypeInfo {
     TypeKind kind;
 
     union {
         /* Pointer: the type being pointed to (e.g. int* -> int). */
         TypeInfo *pointer_to;
 
         /* Array: the element type and the declared array size.  A size of 0
          * means that the size is unknown/unspecified (e.g. in parameter
          * declarations). */
         struct {
             TypeInfo *element_type;
             int       array_size;
         } array_info;
 
         /* Struct: name of the struct and a linked list with its fields. The
          * field list reuses SymbolEntry to make name → type mappings. */
         struct {
             char           *name;   /* struct tag, never NULL */
             struct SymbolEntry *fields; /* field symbol table (flat list) */
         } struct_info;
 
         /* Function: return type + parameter list (a flat list of SymbolEntry).
          * The parameter nodes use their `type` field to store parameter type. */
         struct {
             TypeInfo       *return_type;
             struct SymbolEntry *parameters;
         } function_info;
     } info;
 };
 
 /* ExtendedTypeInfo augments TypeInfo with the “l-value” property required by
  * the semantic checker to enforce assignment rules. */
 typedef struct ExtendedTypeInfo {
     TypeInfo *type;      /* can be NULL to signal previous error */
     int       is_lvalue; /* 1 if l-value, 0 if r-value */
 } ExtendedTypeInfo;
 
 /****************************************************************************
  *                             Public API                                   *
  ****************************************************************************/
 
 /* Basic built-in types are exposed as singletons to avoid creating the same
  * immutable objects over and over again.  They are initialised by calling
  * `init_builtin_types()` exactly once at program start-up. */
 extern TypeInfo *TYPE_INT;
 extern TypeInfo *TYPE_CHAR;
 extern TypeInfo *TYPE_NULLPTR; /* special stand-in for NULL literal */
 
 /* Must be called once before any other type helper to populate the three
  * global built-in type objects above. */
 void init_builtin_types(void);
 
 /* Constructors *************************************************************/
 TypeInfo *create_type(TypeKind kind);
 TypeInfo *create_pointer_type(TypeInfo *base);
 TypeInfo *create_array_type(TypeInfo *element, int size);
 TypeInfo *create_struct_type(const char *name, struct SymbolEntry *fields);
 TypeInfo *create_function_type(TypeInfo *ret, struct SymbolEntry *params);
 
 /* Comparison ***************************************************************/
 /* Deep structural equality check compatible with Project-3 requirements.
  * – For struct types we compare by *name* only as requested by the spec.
  * – For function types we compare return type + number/types of parameters. */
 int are_types_equal(const TypeInfo *a, const TypeInfo *b);
 
 #endif /* __TYPE_H__ */
 