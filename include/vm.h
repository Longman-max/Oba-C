#ifndef VM_H
#define VM_H

#include "ast.h"
#include "symtab.h"

// The Virtual Machine/Execution Environment
typedef struct {
    SymbolTable *symtab;
    int memory[MAX_SYMBOLS]; // Simple array to hold variable values
} VirtualMachine;

// Function Prototypes
VirtualMachine* vm_create(SymbolTable *st);
void vm_destroy(VirtualMachine *vm);

// Main execution function
void vm_execute_program(VirtualMachine *vm, ASTNode *program);

#endif // VM_H