#include <stdio.h>
#include "lexer.h"
#include "token.h"

// Test source code for Oba-C-Compiler
const char *test_source = 
    "int my_var; \n"
    "my_var = (10 + 2) * 5; \n"
    "if (my_var == 60) print(my_var); \n"
    "illegal@char";

void test_lexer(const char *source) {
    printf("--- Running Lexer Test ---\n");
    printf("Source Code:\n%s\n", source);

    Lexer *l = lexer_create(source);
    Token *t;
    int count = 0;

    do {
        t = lexer_next_token(l);
        
        // Print the token details
        printf("[%3d:%-2d] Type: %-16s Lexeme: \"%s\"\n", 
               t->line, t->column, 
               token_type_to_string(t->type), 
               t->lexeme);
        
        if (t->type == TOKEN_ILLEGAL) {
            printf("!!! ERROR: Found illegal token on line %d, column %d\n", t->line, t->column);
        }

        TokenType current_type = t->type;
        token_free(t); // Free the token after use
        
        if (current_type == TOKEN_EOF) break;
        count++;

    } while (1);

    lexer_destroy(l);
    printf("Successfully tokenized %d meaningful units.\n", count);
}

int main() {
    test_lexer(test_source);
    return 0;
}