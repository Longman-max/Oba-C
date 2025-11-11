#include <stdlib.h>
#include <string.h>
#include "symtab.h"

// Creates and initializes a new Symbol Table
SymbolTable* symtab_create() {
    SymbolTable *st = (SymbolTable*)calloc(1, sizeof(SymbolTable));
    if (!st) return NULL;
    st->count = 0;
    return st;
}

// Cleans up the Symbol Table memory
void symtab_destroy(SymbolTable *st) {
    if (st) {
        // Free the dynamically allocated names
        for (int i = 0; i < st->count; i++) {
            free(st->symbols[i].name);
        }
        free(st);
    }
}

// Looks up a symbol by name and returns its index in the table
int symtab_lookup(SymbolTable *st, const char *name) {
    for (int i = 0; i < st->count; i++) {
        if (strcmp(st->symbols[i].name, name) == 0) {
            return i;
        }
    }
    return -1; // Not found
}

// Inserts a new symbol. Uses the next available stack index as its location.
int symtab_insert(SymbolTable *st, const char *name) {
    if (st->count >= MAX_SYMBOLS) {
        fprintf(stderr, "Error: Symbol table overflow. Max %d symbols supported.\n", MAX_SYMBOLS);
        return -1;
    }
    
    // Check if the symbol already exists
    if (symtab_lookup(st, name) != -1) {
        fprintf(stderr, "Error: Variable '%s' already declared.\n", name);
        return -1;
    }
    
    Symbol *s = &st->symbols[st->count];
    s->name = strdup(name);
    s->stack_index = st->count; // Assign the current count as the memory address
    
    st->count++;
    return s->stack_index;
}