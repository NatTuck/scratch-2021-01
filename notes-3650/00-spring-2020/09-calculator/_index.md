+++
title = "./09-calculator/notes.md"
+++

# Computer Systems

## First Thing

 - Questions on the Homework?

## Calculator

Geneneral flow:

 - Read a line of text: fgets (*never* "gets")
 - Split the line into tokens.
- Parse the tokens into an abstract syntax tree
 - Evaluate the AST

Draw a diagram on the board, with an example.

### Tokenizer

 - We have a line of text.
 - We need a list of tokens.
 - Work character by character and build up current token.
 - When the current character doesn't match the previous token,
   push the previous token and start a new one.
 - This is a state machine.

### Parser and AST

 - An Abstract Syntax tree encodes the expression as it will be evaluated.
 - For an arithemtic expression:
 - Numbers are leaves.
 - Internal nodes are operators.
 - Order of operations is encoded in the structure; the root note is evaluated
   last.
 - In general, this can be done efficiently with a stack.
 - Our code uses a less efficient method: scan the whole input for the root,
   split there, and recurse on the subtrees.

### Evaluating an AST

 - AST can be evaluated.
 - This is a simple recursive function.

### Clang Check and Valgrind

 - Clang is a compiler; clang-check compiles and spits out the warnings.
 - Valgrind is a tool for finding memory errors.
 - Warnings from either of these may or may not indicate a bug in your
   program, but fixing them tends to be a good idea.

