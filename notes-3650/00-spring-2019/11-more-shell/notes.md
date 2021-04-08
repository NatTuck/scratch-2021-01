---
layout: default
---

# Computer Systems

## First: Challenge Questions

## Sort Pipe Example

 - Show example.

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

## Accessing Files and Directories

 - ls.c
 - Make sure to show manpages for readdir and stat.
 - Write a single program that, based on its argv[0], will do
   any of: mv, rm, cp, or chmod.
 - (rename, unlink, fread/fwrite, chmod)

## How does fgets / scanf work?

 - What if you mix read and fgets?
 - getc / ungetc
 - How do you actually build a buffer?

