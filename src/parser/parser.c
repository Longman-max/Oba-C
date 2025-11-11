#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h" // <-- This MUST be here to define Parser
#include "ast.h"    // <-- This MUST be here to define ASTNode

// --- Private Function Prototypes (for our grammar) ---
static ASTNode* parse_statement(Parser *p);
static ASTNode* parse_var_decl_statement(Parser *p);
static ASTNode* parse_assign_statement(Parser *p, Token* identifier_token);
static ASTNode* parse_print_statement(Parser *p);
static ASTNode* parse_if_statement(Parser *p);

static ASTNode* parse_expression(Parser *p);
static ASTNode* parse_term(Parser *p);
static ASTNode* parse_factor(Parser *p);

// --- Core Parser Functions ---

Parser* parser_create(Lexer *l) {
    Parser *p = (Parser*)calloc(1, sizeof(Parser));
    p->lexer = l;

    // Initialize by loading two tokens (current and peek)
    parser_next_token(p);
    parser_next_token(p); 
    
    return p;
}

void parser_destroy(Parser *p) {
    // Note: We don't free the lexer here, as it was created externally.
    free(p);
}

// Advances the token stream
void parser_next_token(Parser *p) {
    p->current_token = p->peek_token;
    p->peek_token = lexer_next_token(p->lexer);
}

// Helper to check and consume the next token if it matches
static int expect_peek(Parser *p, TokenType type) {
    if (p->peek_token->type == type) {
        parser_next_token(p);
        return 1;
    } else {
        // Simple error handling
        fprintf(stderr, "Parser Error (Line %d): Expected token %s, got %s\n",
                p->peek_token->line, 
                token_type_to_string(type),
                token_type_to_string(p->peek_token->type));
        return 0; 
    }
}

// --- Parsing Functions (Top-Down) ---

// Program -> Statement*
ASTNode* parse_program(Parser *p) {
    ASTNode *program = ast_node_create(NODE_PROGRAM);

    while (p->current_token->type != TOKEN_EOF) {
        ASTNode *stmt = parse_statement(p);
        if (stmt) {
            ast_program_add_statement(program, stmt);
        }
        parser_next_token(p);
    }
    return program;
}

// Statement -> Declaration | Assignment | PrintStatement | IfStatement
static ASTNode* parse_statement(Parser *p) {
    switch (p->current_token->type) {
        case TOKEN_INT:
            return parse_var_decl_statement(p);
        case TOKEN_PRINT:
            return parse_print_statement(p);
        case TOKEN_IF:
            return parse_if_statement(p);
        case TOKEN_IDENTIFIER:
            if (p->peek_token->type == TOKEN_ASSIGN) {
                return parse_assign_statement(p, p->current_token);
            }
            break;
        default:
            return NULL;
    }
    return NULL;
}

// Declaration -> 'int' Identifier ';'
static ASTNode* parse_var_decl_statement(Parser *p) {
    ASTNode *node = ast_node_create(STMT_VAR_DECL);

    if (!expect_peek(p, TOKEN_IDENTIFIER)) {
        ast_node_free(node);
        return NULL;
    }
    
    node->name = strdup(p->current_token->lexeme);

    if (!expect_peek(p, TOKEN_SEMICOLON)) {
        ast_node_free(node);
        return NULL;
    }
    
    return node;
}

// Assignment -> Identifier '=' Expression ';'
static ASTNode* parse_assign_statement(Parser *p, Token* identifier_token) {
    ASTNode *node = ast_node_create(STMT_ASSIGN);
    node->name = strdup(identifier_token->lexeme);

    // Consume the '='
    parser_next_token(p); // current_token is now '='
    parser_next_token(p); // current_token is now the start of the expression
    
    node->expression = parse_expression(p);

    if (!expect_peek(p, TOKEN_SEMICOLON)) {
        ast_node_free(node);
        return NULL;
    }
    
    return node;
}

// PrintStatement -> 'print' '(' Identifier ')' ';'
static ASTNode* parse_print_statement(Parser *p) {
    ASTNode *node = ast_node_create(STMT_PRINT);

    if (!expect_peek(p, TOKEN_LPAREN)) {
        ast_node_free(node);
        return NULL;
    }
    
    if (!expect_peek(p, TOKEN_IDENTIFIER)) {
         fprintf(stderr, "Parser Error (Line %d): Expected IDENTIFIER inside print()\n", p->current_token->line);
         ast_node_free(node);
         return NULL;
    }

    node->print_expr = ast_node_create(EXPR_IDENTIFIER);
    node->print_expr->name = strdup(p->current_token->lexeme);


    if (!expect_peek(p, TOKEN_RPAREN)) {
        ast_node_free(node);
        return NULL;
    }

    if (!expect_peek(p, TOKEN_SEMICOLON)) {
        ast_node_free(node);
        return NULL;
    }
    return node;
}

// IfStatement -> 'if' '(' Condition ')' Statement
static ASTNode* parse_if_statement(Parser *p) {
    ASTNode *node = ast_node_create(STMT_IF);

    if (!expect_peek(p, TOKEN_LPAREN)) {
        ast_node_free(node);
        return NULL;
    }
    
    parser_next_token(p); // Consume '('
    
    node->condition = parse_expression(p);
    
    if (!expect_peek(p, TOKEN_RPAREN)) {
        ast_node_free(node);
        return NULL;
    }
    
    parser_next_token(p); // Consume ')'
    
    node->body = parse_statement(p);
    
    return node;
}


// --- Expression Parsing (with precedence) ---

// Expression -> Term { ('+' | '-') Term }*
static ASTNode* parse_expression(Parser *p) {
    ASTNode *left = parse_term(p);

    while (p->peek_token->type == TOKEN_PLUS || p->peek_token->type == TOKEN_MINUS ||
           p->peek_token->type == TOKEN_EQUAL || p->peek_token->type == TOKEN_LT ||
           p->peek_token->type == TOKEN_GT) 
    {
        parser_next_token(p); // Consume the operator
        ASTNode *node = ast_node_create(EXPR_BINARY);
        node->op = p->current_token;
        node->left = left;
        
        parser_next_token(p); // Consume operator, move to right-hand side
        node->right = parse_term(p);
        
        left = node; 
    }
    
    return left;
}

// Term -> Factor { ('*' | '/') Factor }*
static ASTNode* parse_term(Parser *p) {
    ASTNode *left = parse_factor(p);

    while (p->peek_token->type == TOKEN_STAR || p->peek_token->type == TOKEN_SLASH) {
        parser_next_token(p); // Consume the operator
        ASTNode *node = ast_node_create(EXPR_BINARY);
        node->op = p->current_token;
        node->left = left;
        
        parser_next_token(p); // Consume operator, move to right-hand side
        node->right = parse_factor(p);
        
        left = node;
    }
    
    return left;
}

// Factor -> Number | Identifier | '(' Expression ')'
static ASTNode* parse_factor(Parser *p) {
    ASTNode *node = NULL;

    if (p->current_token->type == TOKEN_INTEGER_LITERAL) {
        node = ast_node_create(EXPR_LITERAL);
        node->value = atoi(p->current_token->lexeme);
    } 
    else if (p->current_token->type == TOKEN_IDENTIFIER) {
        node = ast_node_create(EXPR_IDENTIFIER);
        node->name = strdup(p->current_token->lexeme);
    }
    else if (p->current_token->type == TOKEN_LPAREN) {
        parser_next_token(p); // Consume '('
        node = parse_expression(p); // Parse the sub-expression
        
        if (!expect_peek(p, TOKEN_RPAREN)) {
            ast_node_free(node);
            return NULL;
        }
    } else {
        fprintf(stderr, "Parser Error (Line %d): Expected literal, identifier, or '(', got %s\n",
                p->current_token->line,
                token_type_to_string(p->current_token->type));
    }

    return node;
}