# Oba-C Documentation

This document briefly explains the architecture of the **Oba-C** compiler.

---

## Compiler Pipeline

The compiler operates in distinct stages, passing data from one stage to the next.

```

Source Code -> [ Lexer ] -> Tokens -> [ Parser ] -> AST -> [ ...TODO... ]

````

---

## 1. Lexical Analysis (Lexer)

**Files:**  
`src/lexer/lexer.c`, `include/lexer.h`  

**Data Structure:**  
`Token` (defined in `include/token.h`)  

**Job:**  
Reads the raw source code (a string of characters) and groups it into meaningful units called **Tokens**.

**Example:**  
The code:
```c
x = 10;
````

Becomes a stream of tokens:

```
[ IDENTIFIER, "x" ]
[ ASSIGN, "=" ]
[ INTEGER_LITERAL, "10" ]
[ SEMICOLON, ";" ]
```

---

## 2. Syntax Analysis (Parser)

**Files:**
`src/parser/parser.c`, `include/parser.h`

**Data Structure:**
`ASTNode` (defined in `include/ast.h`)

**Job:**
Takes the stream of tokens from the Lexer and builds a hierarchical tree structure called an **Abstract Syntax Tree (AST)**.
This tree represents the program's grammatical structure and enforces the order of operations.

**Technique:**
Uses a **Recursive Descent** strategy — each part of the grammar (e.g., `parse_statement()`, `parse_expression()`) is a separate C function.

---

## Abstract Syntax Tree (AST)

The **AST** is the central data structure for the rest of the compiler.
It is a tree of `ASTNode` structs. Each `ASTNode` has a type (e.g., `STMT_ASSIGN`, `EXPR_BINARY`) and pointers to its children (e.g., `left` and `right` for an operator).

**Example:**
An assignment like:

```c
x = (10 + 2);
```

Would be represented as:

```
Assign: x
  BinaryOp: +
    Literal: 10
    Literal: 2
```

---

## 3. Code Generation (TODO)

This stage will **walk the AST** and generate target code (like **x86 Assembly** or **C**) for each node.

---

*© 2025 Obasi Agbai — Oba-C Project*
