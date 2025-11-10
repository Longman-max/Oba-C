# Oba-C-Compiler

A simple compiler for a mini-language, written entirely in C. This project is a learning exercise in compiler construction.

## Current Status

The **Lexer (Scanner)** is complete. It can read source code and successfully tokenize it, identifying keywords, identifiers, literals, operators, and handling illegal characters.

## How to Build and Run

You can build and run the current Lexer test driver using GCC:

```bash
# Compile the compiler
gcc -o oba_c_compiler src/main.c src/lexer/lexer.c src/lexer/token.c -Iinclude -std=c99

# Run the test driver
./oba_c_compiler
```