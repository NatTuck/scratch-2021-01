+++
title = "./10-fork/notes.md"
+++

# Computer Systems

## First: Homework Questions

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
   - Return val of last command in $?
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

