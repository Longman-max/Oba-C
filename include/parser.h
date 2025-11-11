#ifndef PARSER_H
#define PARSER_H

#include "lexer.h" // <-- Defines Lexer and Token
#include "ast.h"   // <-- Defines ASTNode

// The Parser structure holds the state of our parsing
typedef struct {
    Lexer *lexer;
    Token *current_token;
    Token *peek_token; // Lookahead token
    
} Parser; // <-- THIS IS THE DEFINITION

// --- Parser Core Functions ---

Parser* parser_create(Lexer *l);
void parser_destroy(Parser *p);

// The main function to start parsing
ASTNode* parse_program(Parser *p);

// Helper function to advance tokens
void parser_next_token(Parser *p);

#endif // PARSER_H