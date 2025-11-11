#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"
#include "ast.h"
#include "symtab.h"

// --- Private Function Prototypes ---
static int vm_evaluate_expression(VirtualMachine *vm, ASTNode *expr);
static void vm_execute_statement(VirtualMachine *vm, ASTNode *stmt);

// --- Core VM Management ---

VirtualMachine* vm_create(SymbolTable *st) {
    VirtualMachine *vm = (VirtualMachine*)calloc(1, sizeof(VirtualMachine));
    if (!vm) return NULL;
    
    vm->symtab = st;
    
    // Initialize all memory to zero
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        vm->memory[i] = 0;
    }
    return vm;
}

void vm_destroy(VirtualMachine *vm) {
    // Note: The symbol table is managed externally (and should be destroyed externally)
    free(vm);
}

// --- Execution Traversal Functions ---

// Walks the expression tree and returns the resulting integer value
static int vm_evaluate_expression(VirtualMachine *vm, ASTNode *expr) {
    if (!expr) return 0;

    switch (expr->type) {
        case EXPR_LITERAL:
            return expr->value;

        case EXPR_IDENTIFIER: {
            int index = symtab_lookup(vm->symtab, expr->name);
            if (index == -1) {
                fprintf(stderr, "Runtime Error: Undefined variable '%s'.\n", expr->name);
                exit(1);
            }
            return vm->memory[index];
        }

        case EXPR_BINARY: {
            int left_val = vm_evaluate_expression(vm, expr->left);
            int right_val = vm_evaluate_expression(vm, expr->right);
            
            // Handle arithmetic operations
            if (strcmp(expr->op->lexeme, "+") == 0) return left_val + right_val;
            if (strcmp(expr->op->lexeme, "-") == 0) return left_val - right_val;
            if (strcmp(expr->op->lexeme, "*") == 0) return left_val * right_val;
            if (strcmp(expr->op->lexeme, "/") == 0) {
                if (right_val == 0) {
                    fprintf(stderr, "Runtime Error: Division by zero.\n");
                    exit(1);
                }
                return left_val / right_val;
            }
            
            // Handle comparison operations (used in IF statements)
            if (strcmp(expr->op->lexeme, "==") == 0) return left_val == right_val;
            if (strcmp(expr->op->lexeme, "<") == 0) return left_val < right_val;
            if (strcmp(expr->op->lexeme, ">") == 0) return left_val > right_val;
            
            fprintf(stderr, "Runtime Error: Unknown operator '%s'.\n", expr->op->lexeme);
            return 0;
        }
        
        default:
            fprintf(stderr, "Runtime Error: Cannot evaluate node type %d in expression.\n", expr->type);
            return 0;
    }
}

// Executes a single statement, modifying the VM state
static void vm_execute_statement(VirtualMachine *vm, ASTNode *stmt) {
    if (!stmt) return;
    
    switch (stmt->type) {
        case STMT_VAR_DECL:
            // Symbol is added during the initial pass (in vm_execute_program)
            // No runtime action needed other than reserving memory (which we do implicitly)
            break;

        case STMT_ASSIGN: {
            int index = symtab_lookup(vm->symtab, stmt->name);
            if (index == -1) {
                fprintf(stderr, "Runtime Error: Cannot assign to undeclared variable '%s'.\n", stmt->name);
                exit(1);
            }
            int result = vm_evaluate_expression(vm, stmt->expression);
            vm->memory[index] = result;
            printf("[TRACE] Assigned '%s' = %d\n", stmt->name, result);
            break;
        }

        case STMT_PRINT: {
            int value = vm_evaluate_expression(vm, stmt->print_expr);
            // Assumes print_expr is an identifier, but we evaluate the expression result
            printf("Oba-C Output: %d\n", value);
            break;
        }

        case STMT_IF: {
            int condition_result = vm_evaluate_expression(vm, stmt->condition);
            if (condition_result) {
                // If condition is true (non-zero), execute the body statement
                vm_execute_statement(vm, stmt->body);
            }
            break;
        }
        
        default:
            fprintf(stderr, "Runtime Error: Unknown statement type %d.\n", stmt->type);
            break;
    }
}

// Main execution loop: Traverses the program's statements
void vm_execute_program(VirtualMachine *vm, ASTNode *program) {
    if (!program || program->type != NODE_PROGRAM) return;

    // --- EXECUTION PHASE ---
    printf("\n--- Running Oba-C Virtual Machine ---\n");
    
    for (int i = 0; i < program->statement_count; i++) {
        vm_execute_statement(vm, program->statements[i]);
    }
    
    printf("--- Execution Complete ---\n");
}