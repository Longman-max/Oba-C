#include <stdlib.h>
#include <string.h> // <-- Added for strnlen, memcpy
#include <ctype.h>
#include "lexer.h"

// --- ADDED: Portable strndup implementation ---
// This is needed because strndup is not standard C (it's POSIX).
static char *strndup_portable(const char *s, size_t n) {
    size_t len = strnlen(s, n);
    char *new_s = (char *)malloc(len + 1);
    if (new_s == NULL) return NULL;
    memcpy(new_s, s, len);
    new_s[len] = '\0';
    return new_s;
}

// Helper map for keywords (for easy lookup)
static struct {
    char *key;
    TokenType value;
} keywords[] = {
    {"int", TOKEN_INT},
    {"if", TOKEN_IF},
    {"print", TOKEN_PRINT},
    {NULL, TOKEN_ILLEGAL} // Sentinel
};

// --- Lexer State Management Functions ---

Lexer* lexer_create(const char *source_code) {
    Lexer *l = (Lexer *)malloc(sizeof(Lexer));
    if (!l) return NULL;

    l->source = source_code;
    l->position = 0;
    l->line = 1;
    l->column = 1;
    l->current_char = (l->source[0] != '\0') ? l->source[0] : 0;

    return l;
}

void lexer_destroy(Lexer *l) {
    free(l);
}

// Advances the lexer's position by one character
static void advance(Lexer *l) {
    if (l->current_char == '\n') {
        l->line++;
        l->column = 1;
    } else {
        l->column++;
    }
    l->position++;
    l->current_char = (l->source[l->position] != '\0') ? l->source[l->position] : 0;
}

// Skips whitespace characters
static void skip_whitespace(Lexer *l) {
    while (l->current_char != 0 && isspace(l->current_char)) {
        advance(l);
    }
}

// Looks up if an identifier is a keyword
static TokenType lookup_identifier(const char *identifier) {
    for (int i = 0; keywords[i].key != NULL; i++) {
        if (strcmp(keywords[i].key, identifier) == 0) {
            return keywords[i].value;
        }
    }
    return TOKEN_IDENTIFIER;
}

// --- Tokenizing Functions ---

// Handles identifiers and keywords (e.g., 'x', 'int')
static Token* read_identifier_or_keyword(Lexer *l) {
    int start_pos = l->position;
    int start_col = l->column;

    // Identifiers start with a letter and can contain letters or digits
    while (isalnum(l->current_char) || l->current_char == '_') {
        advance(l);
    }

    int len = l->position - start_pos;
    // --- FIXED: Use portable strndup ---
    char *lexeme = strndup_portable(l->source + start_pos, len);
    
    TokenType type = lookup_identifier(lexeme);
    Token *t = token_create(type, lexeme, l->line, start_col);
    free(lexeme); // Free the temp copy

    return t;
}

// Handles integer literals (e.g., '123', '0', '42')
static Token* read_number(Lexer *l) {
    int start_pos = l->position;
    int start_col = l->column;

    while (isdigit(l->current_char)) {
        advance(l);
    }

    int len = l->position - start_pos;
    // --- FIXED: Use portable strndup ---
    char *lexeme = strndup_portable(l->source + start_pos, len);
    Token *t = token_create(TOKEN_INTEGER_LITERAL, lexeme, l->line, start_col);
    free(lexeme);

    return t;
}

// Core function to get the next token
Token* lexer_next_token(Lexer *l) {
    skip_whitespace(l);

    int start_col = l->column;
    char current_char = l->current_char;

    if (current_char == 0) {
        return token_create(TOKEN_EOF, "", l->line, start_col);
    }

    if (isalpha(current_char) || current_char == '_') {
        return read_identifier_or_keyword(l);
    }

    if (isdigit(current_char)) {
        return read_number(l);
    }

    // Handle single-character tokens and multi-character operators
    switch (current_char) {
        case '+': advance(l); return token_create(TOKEN_PLUS, "+", l->line, start_col);
        case '-': advance(l); return token_create(TOKEN_MINUS, "-", l->line, start_col);
        case '*': advance(l); return token_create(TOKEN_STAR, "*", l->line, start_col);
        case '/': advance(l); return token_create(TOKEN_SLASH, "/", l->line, start_col);
        case '(': advance(l); return token_create(TOKEN_LPAREN, "(", l->line, start_col);
        case ')': advance(l); return token_create(TOKEN_RPAREN, ")", l->line, start_col);
        case ';': advance(l); return token_create(TOKEN_SEMICOLON, ";", l->line, start_col);

        case '=':
            // Check for '==' (EQUAL) or '=' (ASSIGN)
            advance(l);
            if (l->current_char == '=') {
                advance(l);
                return token_create(TOKEN_EQUAL, "==", l->line, start_col);
            }
            return token_create(TOKEN_ASSIGN, "=", l->line, start_col);
        
        case '<':
            advance(l);
            return token_create(TOKEN_LT, "<", l->line, start_col);

        case '>':
            advance(l);
            return token_create(TOKEN_GT, ">", l->line, start_col);

        default:
            advance(l);
            char illegal_str[2] = {current_char, '\0'};
            return token_create(TOKEN_ILLEGAL, illegal_str, l->line, start_col);
    }
}