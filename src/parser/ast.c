#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"

// Creates a new, blank AST Node
ASTNode* ast_node_create(ASTNodeType type) {
    ASTNode *node = (ASTNode*)calloc(1, sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Error: Could not allocate memory for AST node.\n");
        exit(1);
    }
    node->type = type;
    return node;
}

// Helper to add a statement to a program node's dynamic array
void ast_program_add_statement(ASTNode *program, ASTNode *statement) {
    if (program->type != NODE_PROGRAM) {
        fprintf(stderr, "Error: Attempted to add statement to non-program node.\n");
        return;
    }

    // Increase the size of the statements array
    program->statement_count++;
    program->statements = (ASTNode**)realloc(program->statements, 
                                             program->statement_count * sizeof(ASTNode*));
    if (!program->statements) {
        fprintf(stderr, "Error: Could not reallocate memory for statements.\n");
        exit(1);
    }
    
    // Add the new statement
    program->statements[program->statement_count - 1] = statement;
}

// Recursively frees an AST node and its children
void ast_node_free(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAM:
            for (int i = 0; i < node->statement_count; i++) {
                ast_node_free(node->statements[i]);
            }
            free(node->statements);
            break;

        case STMT_VAR_DECL:
            free(node->name);
            break;

        case STMT_ASSIGN:
            free(node->name);
            ast_node_free(node->expression);
            break;

        case STMT_PRINT:
            ast_node_free(node->print_expr);
            break;
            
        case STMT_IF:
            ast_node_free(node->condition);
            ast_node_free(node->body);
            break;

        case EXPR_BINARY:
            ast_node_free(node->left);
            ast_node_free(node->right);
            // We don't free the token here, as the parser might reuse it
            // or the lexer will free it. Let's assume tokens are managed elsewhere for now.
            break;

        case EXPR_LITERAL:
            // No dynamic memory to free (value is inline)
            break;
            
        case EXPR_IDENTIFIER:
            free(node->name);
            break;
    }
    
    // Finally, free the node itself
    free(node);
}