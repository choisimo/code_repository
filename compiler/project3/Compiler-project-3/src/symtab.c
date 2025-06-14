/*
 * File Name    : symtab.c
 * Description  : Basic scoped symbol table implementation for Project-3
 */

#include "symtab.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Scope *current_scope = NULL;

static SymbolEntry *create_symbol(const char *name, TypeInfo *type, int level)
{
    SymbolEntry *sym = (SymbolEntry *)malloc(sizeof(SymbolEntry));
    if (!sym) {
        fprintf(stderr, "Out of memory while allocating SymbolEntry\n");
        exit(1);
    }
    sym->name  = strdup(name);
    sym->type  = type;
    sym->level = level;
    sym->next  = NULL;
    return sym;
}

void enter_scope(void)
{
    Scope *s = (Scope *)malloc(sizeof(Scope));
    if (!s) {
        fprintf(stderr, "Out of memory while allocating Scope\n");
        exit(1);
    }
    s->symbols = NULL;
    s->next    = current_scope;
    current_scope = s;
}

void leave_scope(void)
{
    if (!current_scope) return;
    /* free symbols */
    SymbolEntry *sym = current_scope->symbols;
    while (sym) {
        SymbolEntry *next = sym->next;
        free(sym->name);
        free(sym);
        sym = next;
    }
    /* pop scope */
    Scope *outer = current_scope->next;
    free(current_scope);
    current_scope = outer;
}

SymbolEntry *sym_lookup_current(const char *name)
{
    if (!current_scope) return NULL;
    for (SymbolEntry *it = current_scope->symbols; it; it = it->next) {
        if (strcmp(it->name, name) == 0) return it;
    }
    return NULL;
}

SymbolEntry *sym_lookup(const char *name)
{
    for (Scope *scope = current_scope; scope; scope = scope->next) {
        for (SymbolEntry *it = scope->symbols; it; it = it->next) {
            if (strcmp(it->name, name) == 0) return it;
        }
    }
    return NULL;
}

SymbolEntry *sym_insert(const char *name, TypeInfo *type)
{
    /* ensure not redeclared in current scope */
    if (sym_lookup_current(name)) return NULL; /* caller handles error */

    SymbolEntry *sym = create_symbol(name, type, current_scope ? 0 : 0);
    sym->next = current_scope ? current_scope->symbols : NULL;
    if (current_scope)
        current_scope->symbols = sym;
    else {
        /* Should never happen because we always push at least global scope */
        fprintf(stderr, "sym_insert called with no current scope\n");
        exit(1);
    }
    return sym;
}
