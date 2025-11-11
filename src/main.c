#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "token.h"
#include "parser.h" // <-- THIS LINE FIXES THE ERROR
#include "ast.h"
#include "symtab.h" 
#include "vm.h"     

// Test source code for Oba-C
const char *test_source = 
    "int my_score; \n"
    "int base_level_power; \n"
    "base_level_power = 100; \n"
    "my_score = (base_level_power + 20) * 3; \n"
    "if (my_score > 350) print(my_score); \n"
    "if (my_score < 350) print(base_level_power); \n"
    "my_score = my_score / 2; \n"
    "print(my_score); \n";

// --- AST Printing Function (for debugging) ---
void ast_print(ASTNode *node, int indent) {
    if (!node) return;

    for (int i = 0; i < indent; i++) printf("  ");

    switch (node->type) {
        case NODE_PROGRAM:
            printf("Program:\n");
            for (int i = 0; i < node->statement_count; i++) {
                ast_print(node->statements[i], indent + 1);
            }
            break;
        case STMT_VAR_DECL:
            printf("VarDecl: %s\n", node->name);
            break;
        case STMT_ASSIGN:
            printf("Assign: %s\n", node->name);
            ast_print(node->expression, indent + 1);
            break;
        case STMT_PRINT:
            printf("Print:\n");
            ast_print(node->print_expr, indent + 1);
            break;
        case STMT_IF:
            printf("If:\n");
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Condition:\n");
            ast_print(node->condition, indent + 2);
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Body:\n");
            ast_print(node->body, indent + 2);
            break;
        case EXPR_BINARY:
            printf("BinaryOp: %s\n", node->op->lexeme);
            ast_print(node->left, indent + 1);
            ast_print(node->right, indent + 1);
            break;
        case EXPR_LITERAL:
            printf("Literal: %d\n", node->value);
            break;
        case EXPR_IDENTIFIER:
            printf("Identifier: %s\n", node->name);
            break;
    }
}

// --- Semantic Analysis Pass (Symbol Registration) ---
// Walks the AST to register all declarations in the Symbol Table
static void register_symbols(ASTNode *program, SymbolTable *st) {
    if (program->type != NODE_PROGRAM) return;

    printf("\n--- Semantic Pass: Registering Symbols ---\n");

    for (int i = 0; i < program->statement_count; i++) {
        ASTNode *stmt = program->statements[i];
        if (stmt->type == STMT_VAR_DECL) {
            int index = symtab_insert(st, stmt->name);
            printf("[SYMBOL] Variable '%s' registered at memory index %d\n", stmt->name, index);
        }
    }
}


int main() {
    printf("--- Oba-C Compiler: Front-End ---\n");

    // 1. Lexer
    Lexer *l = lexer_create(test_source);
    
    // 2. Parser
    Parser *p = parser_create(l); // This line needs "parser.h"
    ASTNode *program = parse_program(p);

    if (!program) {
        fprintf(stderr, "Compilation failed during parsing.\n");
        return 1;
    }
    
    // 3. Print AST (for debugging)
    printf("\n--- Abstract Syntax Tree (AST) ---\n");
    ast_print(program, 0);

    // 4. Semantic Pass (Symbol Table creation)
    SymbolTable *st = symtab_create();
    register_symbols(program, st);
    
    // 5. Code Generation / Execution
    VirtualMachine *vm = vm_create(st);
    vm_execute_program(vm, program); // Run the program
    
    // 6. Cleanup
    ast_node_free(program);
    vm_destroy(vm);
    symtab_destroy(st);
    parser_destroy(p);
    lexer_destroy(l); 
   
    return 0;
}