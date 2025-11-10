#include <stdlib.h>
#include <string.h>
#include "token.h"

// Helper array for debugging token types
const char *TokenType_names[] = {
    "INT", "IF", "PRINT", 
    "ASSIGN", "PLUS", "MINUS", "STAR", "SLASH", "SEMICOLON", 
    "LPAREN", "RPAREN", "EQUAL", "LT", "GT",
    "IDENTIFIER", "INTEGER_LITERAL", 
    "EOF", "ILLEGAL"
};

// Function to create a new Token
Token* token_create(TokenType type, const char *lexeme, int line, int column) {
    Token *t = (Token *)malloc(sizeof(Token));
    if (!t) return NULL; 

    t->type = type;
    // Lexeme needs to be copied as the Lexer might overwrite the input buffer
    t->lexeme = strdup(lexeme); 
    t->line = line;
    t->column = column;

    return t;
}

// Function to free the memory allocated for a Token
void token_free(Token *t) {
    if (t) {
        free(t->lexeme);
        free(t);
    }
}

// Function to get the string representation of the token type (for debugging)
const char* token_type_to_string(TokenType type) {
    if (type >= TOKEN_INT && type <= TOKEN_ILLEGAL) {
        return TokenType_names[type];
    }
    return "UNKNOWN";
}