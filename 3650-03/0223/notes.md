
# Tokenizer Homework Questions?

 - You cannot return a pointer to a locally
   stack allocated value from a function,
   because the stack frame will be gone.
 - If you have a value in a function,
   you can return it.
 - How you handle invalid input is a design
   choice - make it, document it, implement
   it as documented.
 - Get rid of any excess build / test
   artifacts before submitting.
 - A Makefile is required. Definitely don't
   submit binaries.
 - Plagiarism is a claim (explicit or implicit) 
   that you wrote something when it's someone 
   else's work. Self-plagarism is impossible
   by definition.
 - Tokenize: "|||||||||||" - pick some behavior

# Today: Implementing Shell Operators

## Standard file descriptors

Every program starts with three open file descriptors:

 - 0 is stdin
 - 1 is stdout
 - 2 is stderr

## Doing an input redirect

 - Open the new file we'll be reading from
 - Close stdin
 - dup the fd
 - Run some code that reads from stdin, including
   executing a new program with exec

## Assignment Strategy

Grading for most assignments follows a consistent
pattern:

 - Half of the points are for automated tests.
   - You gain points for passing tests.
   - If you pass half the tests, you get
     half the points.
 - The other half is manual grading.
   - You lose points for making mistakes.
   - On the shell, you lose points for not
     attempting features.

It's really hard to predict in advance how long
an assignment will take. Predicting programming
assignment time is basically impossible.

It's always possible that you won't have enough
time and therefore won't finish 100%.

I suggest this plan:

 - Submit the starter code. It might pass tests.
 - Implement enough logic to pass the first automated
   test. Submit that to Inkfish.
 - Repeat this for every test or handful of tests.

It can be *faster* to start with a simple
design that doesn't solve the whole problem and
then incrementally improve it than to attempt
"big design" up front.

Getting something to work at all requires writing
a bunch of utility functions and infrastructure.

Example: In the shell, the correct solution involves
building an AST and then writing a recursive
evaluation function to traverse it.

But... doing neither of those things will pass 10/15
test cases or so. I strongly recommend starting by
not having a complex AST structure or recursive
evaluator.

Finally, my #1 recommendation when programming:

Never write more than 10 lines of code without
running your program and minimally testing that
it works. That may involve printing out
intermediate values.

C testing framework:
https://libcheck.github.io/check/

## Shell Evaluation Plan

Precondition: We've parsed our input
into an Abstract Syntax Tree.

Note: This plan may not be right, expecially
consider that there might be restrictions on
when you can fork.

Base case: "command arg1 arg2"
 
 - fork
 - in child: exec(command, arg1, arg2)
 - in parent: wait

Semicolon: "command1; command2"

 - execute command1 recursively
 - execute command2 recursively 

And / Or: "command1 OP command2" (OP in &&, ||)

 - execute command1 recursively,
   capturing exit code
 - if we've got the correct exit code,
   execute command 2 recursively

Background: "command1 &"

 - fork
 - in child: run command1 recursively
 - in parent: don't wait

Background: "command1 & command2"

 - Run "command1 &" recursively
 - Run command2

Redirect: "command1 OP file" (OP is <, >)

 - fork, in child:
   - open the file
   - change the file descriptor table to replace
     stdin or stdout
   - run command1 recursively
 - in parent, wait on child

Pipe: "comand1 | command2"

 - fork [0], in child:
   - pipe(fds)
   - fork [1], in child:
     - hook up fds[1] to stdout
     - run command1 recursively
   - fork [2], in child:
     - hook up fds[0] to stdin
     - run command2 recursively
   - wait for kids [1], [2]
 - parent waits on child of [0]


