/*
 * File Name    : type.c
 * Description  : Implementation of type helpers for Project-3 semantic analyser
 */

#include "type.h"
#include <assert.h>

/* Built-in singleton type instances */
TypeInfo *TYPE_INT      = NULL;
TypeInfo *TYPE_CHAR     = NULL;
TypeInfo *TYPE_NULLPTR  = NULL;

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
    return t;
}

void init_builtin_types(void)
{
    if (TYPE_INT) return; /* already initialised */
    TYPE_INT     = alloc_type(TYPE_KIND_INT);
    TYPE_CHAR    = alloc_type(TYPE_KIND_CHAR);
    TYPE_NULLPTR = alloc_type(TYPE_KIND_NULL);
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

TypeInfo *create_struct_type(const char *name, struct SymbolEntry *fields)
{
    TypeInfo *t = alloc_type(TYPE_KIND_STRUCT);
    t->info.struct_info.name   = strdup(name);
    t->info.struct_info.fields = fields;
    return t;
}

TypeInfo *create_function_type(TypeInfo *ret, struct SymbolEntry *params)
{
    TypeInfo *t = alloc_type(TYPE_KIND_FUNCTION);
    t->info.function_info.return_type = ret;
    t->info.function_info.parameters  = params;
    return t;
}

static int compare_param_lists(const struct SymbolEntry *a,
                               const struct SymbolEntry *b);

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
static int compare_param_lists(const struct SymbolEntry *a,
                               const struct SymbolEntry *b)
{
    while (a && b) {
        if (!are_types_equal(a->type, b->type)) return 0;
        a = a->next;
        b = b->next;
    }
    return (a == NULL && b == NULL);
}
