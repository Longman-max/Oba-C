#include <stdio.h>
#include "lexer.h"    // <-- Defines Lexer
#include "token.h"    // <-- Defines Token
#include "parser.h"   // <-- Defines Parser
#include "ast.h"      // <-- Defines ASTNode

// Test source code for Oba-C
const char *test_source = 
    "int my_var; \n"
    "int another_var; \n"
    "my_var = (10 + 2) * 5; \n"
    "if (my_var > 50) print(my_var); \n";

// --- New AST Printing Function (for testing) ---

void ast_print(ASTNode *node, int indent) {
    if (!node) return;

    // Print indentation
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


int main() {
    printf("--- Running Oba-C Parser ---\n");
    printf("Source Code:\n%s\n", test_source);

    // 1. Lexer
    Lexer *l = lexer_create(test_source);
    
    // 2. Parser
    Parser *p = parser_create(l);
    ASTNode *program = parse_program(p);

    // 3. Print AST
    printf("\n--- Abstract Syntax Tree (AST) ---\n");
    if (program) {
        ast_print(program, 0);
    } else {
        printf("Parsing failed.\n");
    }

    // 4. Cleanup
    ast_node_free(program);
    parser_destroy(p);
    lexer_destroy(l); 
   
    return 0;
}