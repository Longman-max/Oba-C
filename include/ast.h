#ifndef AST_H
#define AST_H

#include "token.h" // <-- This is needed for the Token struct

// Define the different types of nodes our AST can have
typedef enum {
    NODE_PROGRAM,
    
    // Statements
    STMT_VAR_DECL,
    STMT_ASSIGN,
    STMT_PRINT,
    STMT_IF,
    
    // Expressions
    EXPR_BINARY,
    EXPR_LITERAL,
    EXPR_IDENTIFIER,
    
    // Conditions (can reuse EXPR_BINARY for ==, <, >)

} ASTNodeType;

// The core AST Node structure
typedef struct ASTNode {
    ASTNodeType type;
    
    // For NODE_PROGRAM
    struct ASTNode **statements; // A dynamic array of statement nodes
    int statement_count;

    // For STMT_VAR_DECL, STMT_ASSIGN, EXPR_IDENTIFIER
    char *name; // e.g., the variable name 'x'
    
    // For STMT_ASSIGN
    struct ASTNode *expression; // The right-hand side of '='
    
    // For STMT_PRINT
    struct ASTNode *print_expr;

    // For STMT_IF
    struct ASTNode *condition; // The (x == 10) part
    struct ASTNode *body;      // The statement to execute
    
    // For EXPR_BINARY (e.g., 10 + 2 or x < 5)
    struct ASTNode *left;
    struct ASTNode *right;
    Token *op; // The operator token (+, -, *, /, ==, <, >)

    // For EXPR_LITERAL
    int value; // The integer value

} ASTNode; // <-- This typedef creates the 'ASTNode' type

// --- AST Utility Functions ---
ASTNode* ast_node_create(ASTNodeType type);
void ast_node_free(ASTNode *node);
void ast_program_add_statement(ASTNode *program, ASTNode *statement);

#endif // AST_H