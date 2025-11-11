# Oba-C

**Oba-C** is a simple compiler for a mini-language, implemented entirely in C. This project serves as a hands-on learning exercise in compiler construction.

---

## Overview

Oba-C is a complete, working compiler and interpreter that demonstrates the fundamental stages of compilation:

1.  **Lexical Analysis (Lexer):** Converts raw source code into tokens.
2.  **Syntax Analysis (Parser):** Constructs an Abstract Syntax Tree (AST) from tokens.
3.  **Execution (Interpreter):** An AST-walking Virtual Machine (VM) that executes the program.

For a detailed breakdown of Oba-C's internal architecture and design, see the full documentation: [Oba-C Documentation](docs/documentation.md)

---

## Current Status

The **Oba-C** compiler is complete and fully functional.

- **Front-End:** The Lexer and Parser build a valid Abstract Syntax Tree.
- **Back-End:** A Symbol Table manages variables, and an AST-walking Interpreter (VM) successfully executes the program's logic.

---

## Guide for setting up C/C++

Before building Oba-C, ensure you have a working C compiler (like `gcc`) and the `make` utility.

### Windows

1.  Install **MSYS2** from [msys2.org](https://www.msys2.org/) (this is the easiest way to get `gcc` and `make`).
2.  After installing, open the **MSYS2 MINGW64** terminal and run:
    ```bash
    pacman -S base-devel mingw-w64-x86_64-gcc
    ```
3.  Verify installation:
    ```bash
    gcc --version
    make --version
    ```

### macOS

1.  Install Xcode Command Line Tools:
    ```bash
    xcode-select --install
    ```
2.  Verify installation (this includes `gcc`/`clang` and `make`):
    ```bash
    gcc --version
    make --version
    ```

### Linux (Ubuntu/Debian)

1.  Install GCC and build tools:
    ```bash
    sudo apt update
    sudo apt install build-essential
    ```
2.  Verify installation:
    ```bash
    gcc --version
    make --version
    ```

---

## Cloning the Repository

Clone Oba-C locally:

```bash
git clone https://github.com/Longman-max/Oba-C.git
cd Oba-C
````

-----

## Building and Running

You can compile and run the entire program using the `Makefile`:

```bash
# Compile the compiler
make
```

```bash
# Run the compiler (which executes the test program in main.c)
make run
```

This will compile **Oba-C**, then run it. The output will show the AST, the symbol table registration, and the final output from your executed Oba-C code.

-----

## Contributing

I vibe coded this tool to learn how compilers work and decided to share it. I don’t plan on maintaining it, but feel free to explore, fork, or modify it however you like.

-----

## References

  * [Writing a Compiler in C](https://norasandler.com/2017/11/29/Write-a-Compiler.html)
  * [Creating a Compiler — YouTube Playlist](https://youtube.com/playlist?list=PLUDlas_Zy_qC7c5tCgTMYq2idyyT241qs&si=cofq5wZ0kBDhI0Sq)

-----

© 2025 Obasi Agbai — Oba-C Project
