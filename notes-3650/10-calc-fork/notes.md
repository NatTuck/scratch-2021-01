# Computer Systems

## First Thing

 - Questions on the Homework?

# Next Goal: Challenge 1

This is really an assignment in two parts:

 - HW05: Tokenizer
 - CH01: Complete Shell

## Part 1: Calculator

General flow:

 - Read a line of text: fgets (*never* "gets")
 - Split the line into tokens.
- Parse the tokens into an abstract syntax tree
 - Evaluate the AST

To demo this, we're going to copy the code from the scratch repo
and, delete tokens.c, and then rewrite that module.

Then we're going to do a quick scan through the rest of the program.

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

# Part 2: Shell Concepts

## File I/O

Progress from fgets /  to pure syscall I/O.

## Fork & Exec

Cover simple example.

 - A running instance of a program is a process.
 - Fork *copies* a process.
 - Exec loads a binary and replaces the contents of a 
   running process, like some sort of horror movie.

## Shell Operators

Demonstrate each of these in a shell:

 - Redirect input: sort < foo.txt
 - Redirect output: sort foo.txt > output.txt
 - Pipe: sort foo.txt | uniq
   - man 2 pipe
 - Background: sleep 10 &
 - And: true && echo one
   - Return value from main
   - Success (rv = 0) is true
   - Return val of last command in \$?
 - Or: true || echo one
 - Semicolon: echo one; echo two

## Redirect Example

 - redir.c

## Pipe Examples

 - pipe0, pipe1
 - sort-pipe

## Shell Evaluation Plan

 - Base case: "command arg1 arg2"
   - fork
   - in child: exec(command, arg1, arg2)
   - in parent: wait on child
 - Semicolon: "command1; command2"
   - split token list on semicolon
   - execute command1 (recursively)
   - execute command2 (recursively)
 - And / Or: "command1 OP command2"
   - split token list on operator
   - execute command1 (recursively)
   - wait for exit code
   - if correct exit code: execute command2 (recursively)
 - Background
   - fork
   - in child: execute command (recursively)
   - in parent:
     - don't wait right away
     - if you wait in the future, keep in mind it may be the
       background job
     - you should wait at some point to avoid zombies.
 - Redirect: "command OP file" 
   - fork
   - in child: change the file descriptor table to accomplish the redirect
   - in child: execute command (recursively)
   - in parent: wait on child
 - Pipe: "command1 | command 2"
   - fork
   - in child:
     - pipe syscall
     - fork
     - in child/child: hook pipe to stdout, close other side
     - in child/child: execute command1 (r)
     - in child/parent: hook pipe to stdin, close other side
     - in child/parent: execute command2 (r)
     - in child/parent: wait on child/child
   - in parent: wait on child

