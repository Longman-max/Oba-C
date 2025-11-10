#ifndef TOKEN_H
#define TOKEN_H

// 🚀 Start with a core set of token types
typedef enum {
    // Keywords
    TOKEN_INT,
    TOKEN_IF,
    TOKEN_PRINT,

    // Operators and Delimiters
    TOKEN_ASSIGN,       // =
    TOKEN_PLUS,         // +
    TOKEN_MINUS,        // -
    TOKEN_STAR,         // *
    TOKEN_SLASH,        // /
    TOKEN_SEMICOLON,    // ;
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_EQUAL,        // ==
    TOKEN_LT,           // <
    TOKEN_GT,           // >

    // Literals and Identifiers
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER_LITERAL,
    
    // End of File (EOF)
    TOKEN_EOF,
    
    // Error
    TOKEN_ILLEGAL

} TokenType;

// The Token structure itself
typedef struct {
    TokenType type;
    char *lexeme; // The actual text (e.g., "x", "123", "int")
    int line;     // For better error reporting
    int column;
} Token;

// Utility function to create a new token
// --- FIXED: Added 'const' to match the .c file ---
Token* token_create(TokenType type, const char *lexeme, int line, int column);

void token_free(Token *t);
const char* token_type_to_string(TokenType type); // For debugging

#endif // TOKEN_H