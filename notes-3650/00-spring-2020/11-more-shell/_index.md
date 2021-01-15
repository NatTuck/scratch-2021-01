+++
title = "./11-more-shell/notes.md"
+++

# Computer Systems

## First: Challenge Questions

## Sort Pipe Example

 - Show example.

## Assignment Strategy

Grading on most assignments in this class follow a consistent structure:

 - Half the points are automated tests.
   - You gain points for passing tests.
   - If you pass half the tests, you get half the points.
 - The other half is manual grading.
   - You lose points for mistakes
   - On the shell, you lose points for not attepting features

It's very difficult to predict in advance how long it will take to complete an
assignment. To minimize the potential points lost due to not finishing on time,
the best strategy is to submit early and often, and to work one automated test
at a time.

 - Submit the starter code. It might pass tests.
 - Implement enough logic to pass the first test. Submit that.
 - Repeat for each test, or each couple of tests.
 
It can be *faster* to start with a simple design and then rewrite to handle more
complex cases than worrying about the best design up front and getting stuck
thinking about design. For example, it may be worth writing a non-recursive
evaluation function that doesn't handle deep nesting even though that can't
possibly pass some of the later tests. This strategy allows you to get
everything else working.

And finally, my #1 recommendation for writing software: Never write more than 20
lines of code without running your program and testing that your existing code
works - even if your program just prints out some intermediate value.

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

## Looking at the Shell Tests

01-echo:

 - Simplest case: A command with an argument.

02-echo-twice:
 
 - Verify that we can handle multiple commands; read and execute
   them one line at a time.

03-cd:

 - "cd" is a shell builtin.
 - To get this to work, you need to handle it as a special case. 
 - Specifically, you can't fork before calling the chdir system call, because
   the current working directory is part of your process state.
 - The final line depends on the top level process state having been changed on the
   previous line.

05-sort-rin:

 - This tests one of the operators.

12-pipeline:

 - What does our AST look like here?
 - Does order of operations actually matter?

The last two tests are purely extra challenges. Until you have everything else
working, I suggest mostly ignoring them unless you see how handling them would
impact your code structure and feel comfortable that you can make those design
choices early without slowing yourself down.

14-horror:

 - This test adds one major feature: parens.
 - Quotes are just an extra case for the tokenizer.
 - Parens aren't just a grouping operator: they create a subshell.
   - Fork for the parens.
   - Recursively evaluate the portion of the command in the parens.
   - The subshell process exits with the value of that command.
 - Once you have subshells, this becomes a heavy test that you've
   implemented recursive evaluation correctly.

15-vars:

 - Shell is a programming language: it has variables
 - Two new operations:
   - Variable assignment
   - Variable use - replace the variable reference with its text content.
 - Fancy version uses hash table.
 - What's the simplest way to store key-value pairs for this assignment?

