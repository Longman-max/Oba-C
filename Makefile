# Makefile for the Oba-C Compiler Project

# Compiler and Flags

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LDFLAGS =

Target executable name

TARGET = oba_c

Source Files

SRC_DIR_LEXER = src/lexer
SRC_DIR_PARSER = src/parser
SRC_DIR_CODEGEN = src/codegen
SRC_DIR_VM = src/vm

List all source files (.c)

SRCS = 

src/main.c 

$(SRC_DIR_LEXER)/lexer.c 

$(SRC_DIR_LEXER)/token.c 

$(SRC_DIR_PARSER)/parser.c 

$(SRC_DIR_PARSER)/ast.c 

$(SRC_DIR_CODEGEN)/symtab.c 

$(SRC_DIR_VM)/vm.c

Object files are generated from source files

OBJS = $(SRCS:.c=.o)

# Default target: builds the executable

all: $(TARGET)

# Rule to link the final executable

$(TARGET): $(OBJS)
$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to compile each .c file into a .o file

%.o: %.c
$(CC) $(CFLAGS) -c $< -o $@

# Run the compiler test driver

run: $(TARGET)
./$(TARGET)

# Clean up all generated files

clean:
rm -f $(OBJS) $(TARGET)