#ifndef SYMTAB_H
#define SYMTAB_H

#include <stdio.h>
#include <stdbool.h>

// Max number of variables we can support
#define MAX_SYMBOLS 100

// Structure to hold a symbol (variable name and its location)
typedef struct {
    char *name;      // The variable identifier (e.g., "my_var")
    int stack_index; // Where the variable is stored in the VM's memory/stack
} Symbol;

// The Symbol Table structure
typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    int count; // Current number of defined symbols
} SymbolTable;

// Function Prototypes
SymbolTable* symtab_create();
void symtab_destroy(SymbolTable *st);

// Inserts a new symbol and returns its index. Returns -1 if table is full.
int symtab_insert(SymbolTable *st, const char *name);

// Looks up a symbol and returns its index. Returns -1 if not found.
int symtab_lookup(SymbolTable *st, const char *name);

#endif // SYMTAB_H
