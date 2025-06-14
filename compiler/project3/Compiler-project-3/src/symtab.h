/*
 * File Name    : symtab.h
 * Description  : Symbol table abstractions for Project-3 semantic analysis
 */

#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#include "type.h"

typedef struct SymbolEntry {
    char                *name;     /* identifier */
    TypeInfo            *type;     /* semantic type */
    int                  level;    /* scope depth (0 = global) */
    struct SymbolEntry  *next;     /* next symbol in same scope */
} SymbolEntry;

/* Scope stack node */
typedef struct Scope {
    SymbolEntry *symbols;          /* head of linked symbols in this scope */
    struct Scope *next;            /* outer scope */
} Scope;

/* Global variables managed by symtab.c */
extern Scope *current_scope;   /* top of scope stack */

/* API */
void   enter_scope(void);
void   leave_scope(void);
SymbolEntry *sym_lookup(const char *name);           /* search up the stack */
SymbolEntry *sym_lookup_current(const char *name);   /* search only top */
SymbolEntry *sym_insert(const char *name, TypeInfo *type);

#endif /* __SYMTAB_H__ */
