# Oba-C Documentation

This document explains the architecture of the **Oba-C** compiler and provides a user guide for the language itself.

---

## Oba-C Language: A User Guide

Welcome to **Oba-C**! It's a simple, C-like language for integer-based calculations.

### 1. Data Types
* You have **one** data type: `int` (integers).
* All math will result in integers (e.g., `5 / 2` will result in `2`).

### 2. Variable Declarations
* You **must** declare all variables before you use them.
* Declarations must be at the top level (you can't declare a variable inside an `if` statement).
* **Syntax:** `int <variable_name>;`
* **Example:**
    ```c
    int x;
    int my_score;
    ```

### 3. Assignment
* You assign values using the single equals sign (`=`).
* **Syntax:** `<variable_name> = <expression>;`
* **Example:**
    ```c
    x = 100;
    my_score = x * 2;
    ```

### 4. Expressions
* You can use `+`, `-`, `*`, `/` for math.
* The parser correctly handles **operator precedence**, so `(10 + 2) * 5` is calculated correctly (as 60).
* You can use parentheses `()` to group expressions.

### 5. Control Flow (The `if` statement)
* You have a simple `if` statement.
* **There is no `else`!**
* The `if` statement **only executes the *single* next statement** if the condition is true. It does not use `{}` curly braces.
* **Syntax:** `if (<condition>) <statement_to_run>;`
* **Example:**
    ```c
    if (my_score > 90) print(my_score);
    ```

### 6. Conditions
* Your conditions can use three operators: `==` (equal to), `>` (greater than), and `<` (less than).
* **Example:** `if (x == 10) ...`

### 7. Output (Printing to Console)
* You have one built-in function: `print()`.
* It takes a **single identifier** (variable name) as its argument.
* **Syntax:** `print(<variable_name>);`
* **Example:**
    ```c
    print(my_score);
    ```

---

## Compiler Architecture

### Compiler Pipeline

The compiler operates in distinct stages, passing data from one stage to the next.

````

Source Code -\> [ Lexer ] -\> Tokens -\> [ Parser ] -\> AST -\> [ Semantic Pass ] -\> [ VM Executor ] -\> Output

````

---

### 1. Lexical Analysis (Lexer)

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

-----

### 2\. Syntax Analysis (Parser)

**Files:**
`src/parser/parser.c`, `include/parser.h`

**Data Structure:**
`ASTNode` (defined in `include/ast.h`)

**Job:**
Takes the stream of tokens from the Lexer and builds a hierarchical tree structure called an **Abstract Syntax Tree (AST)**. This tree represents the program's grammatical structure and enforces the order of operations.

**Technique:**
Uses a **Recursive Descent** strategy — each part of the grammar (e.g., `parse_statement()`, `parse_expression()`) is a separate C function.

-----

### 3\. Semantic Analysis (Symbol Table)

**Files:**
`src/codegen/symtab.c`, `include/symtab.h`

**Data Structure:**
`SymbolTable` (defined in `include/symtab.h`)

**Job:**
Before execution, the compiler does a "Semantic Pass" over the AST. It finds all variable declarations (`int x;`) and registers them in the **Symbol Table**. This table maps the variable name (`"x"`) to a memory location (e.g., `index 0`).

-----

### 4\. Execution (Virtual Machine)

**Files:**
`src/vm/vm.c`, `include/vm.h`

**Job:**
This is the final stage. Instead of generating assembly code, Oba-C uses an **AST-walking interpreter**.

The `vm_execute_program` function traverses the AST, executing statements one by one:

  * **`STMT_ASSIGN`:** It evaluates the expression on the right-hand side (recursively calling `vm_evaluate_expression`) and stores the result in its memory array at the location provided by the Symbol Table.
  * **`STMT_PRINT`:** It evaluates the expression (variable) inside the `print()` call and prints the value to the console.
  * **`STMT_IF`:** It evaluates the condition. If the result is true (non-zero), it recursively executes the body statement.

-----

*© 2025 Obasi Agbai — Oba-C Project*
