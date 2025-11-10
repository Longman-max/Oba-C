#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
    const char *source; // this will point to the program source code string
    int position;     
    char current_char;
    int line;
    int column;
} Lexer;

// Core functions 
Lexer* lexer_create(const char *source_code);
void lexer_destroy(Lexer *l);
Token* lexer_next_token(Lexer *l);

#endif // LEXER_H