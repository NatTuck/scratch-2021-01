
# Homework Questions



# Challenge 1 Background

 - Calc example

## Basic plan for an interpreter

 - Read a line of text
   - (or maybe a whole file)
 - Split line into tokens
 - Parse the tokens into an Abstract Syntax Tree (AST)
 - Evaluate the AST
   - Calc: Produce a value
   - Shell: Produce behavior

## Creating Processes

On POSIX systems like Linux, all processes are:

 - created by the fork() system call
 - descended from pid=1 ("init")

We have two system calls to execute a new program:

 - fork - creates a new process
   - copies the current process, so now there's
     two of them
   - we call fork once, but it returns twice in
     two separate processes
   - the original process is called the parent
     here, fork returns the child's pid
   - the new process is called the child
     here, fork returns 0
 - exec - runs a program in the current process
   - you specify what program to run (e.g. a path)
   - the old program is replaced - its not running
     anymore after exec

## How Windows does it

Only one system call:

```
CreateProcess(const char* path)
```

 - Create a new process
 - Run a program in it

## Why use fork without exec?

 - For parallel speedup
 - For concurrency

But both of those can be done with threads.

 - For process isolation security


## Example ast

" 2*3 + 10 "

```
      +
     / \
    *   10
   / \
  2   3
```







