<p align="center">
  <img src="docs/header.png" alt="Oba-C Header" width="400">
</p>

**Oba-C** is a simple compiler for a mini-language, implemented entirely in C.  
This project serves as a hands-on learning exercise in compiler construction.

---

## Overview

Oba-C demonstrates the fundamental stages of compilation:

1. **Lexical Analysis (Lexer):** Converts raw source code into tokens.  
2. **Syntax Analysis (Parser):** Constructs an Abstract Syntax Tree (AST) from tokens.  
3. **Code Generation (Planned):** Will translate the AST into target code.

For a detailed breakdown of Oba-C's internal architecture and design, see the full documentation:  
[Oba-C Documentation](docs/documentation.md)

## Current Status

The front-end of Oba-C is fully implemented:

- **Lexer:** Reads source code and generates tokens.  
- **Parser:** Consumes tokens to build a complete AST according to the mini-language grammar.

The code generation phase is planned for future development.

## Prerequisites

Before building Oba-C, ensure you have a working C compiler installed.

### Windows

1. Install **MinGW-w64** or **TDM-GCC**:
    - [MinGW-w64 Downloads](https://www.mingw-w64.org/downloads/)
    - Add the `bin` directory to your **PATH** environment variable.
2. Verify installation:
    ```bash
    gcc --version
    ```

### macOS

1. Install Xcode Command Line Tools:

    ```bash
    xcode-select --install
    ```
2. Verify installation:

    ```bash
    gcc --version
    ```

### Linux (Ubuntu/Debian)

1. Install GCC and build tools:

    ```bash
    sudo apt update
    sudo apt install build-essential
    ```
2. Verify installation:

    ```bash
    gcc --version
    ```

## Cloning the Repository

Clone Oba-C locally:

```bash
git clone [https://github.com/Longman-max/Oba-C.git](https://github.com/Longman-max/Oba-C.git)
cd Oba-C
````

## Building and Running

Compile and run the current parser test driver:

```bash
# Compile the compiler
gcc -o oba_c \
    src/main.c \
    src/lexer/lexer.c \
    src/lexer/token.c \
    src/parser/parser.c \
    src/parser/ast.c \
    -Iinclude \
    -std=c99

# Run the compiler test driver
./oba_c
```

This will parse sample code and display the resulting AST structure.

## Contributing

I vibe coded this tool to learn how compilers work and decided to share it.
I don’t plan on maintaining it, but feel free to explore, fork, or modify it however you like.

## References

  * [Writing a Compiler in C](https://norasandler.com/2017/11/29/Write-a-Compiler.html)
  * [Creating a Compiler — YouTube Playlist](https://youtube.com/playlist?list=PLUDlas_Zy_qC7c5tCgTMYq2idyyT241qs&si=cofq5wZ0kBDhI0Sq)

-----

© 2025 Obasi Agbai — Oba-C Project
