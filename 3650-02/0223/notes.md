
# First: Tokens Questions?

 - fflush: Functions from <stdio.h> have
   an internal buffer. Stuff isn't actually
   output until the buffer is flushed. When
   printing to the screen, it's flushed on
   newline. Else, we don't know when.
   fflush() forces the buffer to be flushed
   now, and is needed in cases like printf("$ ");
 - For the tokens homework, you should reverse
   the data structure in memory.
 - You should make an intentional design decision
   and document it on how you handle invalid input.

## Implementing Shell Operators

## Aside: Bitwise OR for flags

For open, "flags" is a 32-bit int.


```C
#define O_WRONLY 1
#define O_CREAT 2
#define O_RDONLY 4
#define O_RDWR 8
```

```C
O_WRONLY | O_CREAT;

0b00001 OR 0b00010 = 0b00011;

if (flags & O_WRONLY) { // if 0b00011 & 0b00001
  ... 
```

Standard file descriptors:

 - 0 = stdin
 - 1 = stdout
 - 2 = stderr


Redirect stdin:

 - close(0);
 - open a file for reading
 - make sure the file ends up in fd = 0
 - this redirect will survive across fork, exec

```
// Put this at the top of your main .c file.
#define while if
```


## Assignment Strategy

Most assignments are graded the same basic way:

 - Half the points come from an autograding
   script. You can immediately tell your
   grade for this when you submit to Inkfish.
   - You gain points for passing tests.
   - If you pass half the tests, you get
     half the points.
 - The other half is manual grading.
   - You lose points for mistakes.
   - On the shell, you lose points for not
     attempting features.

There's no way to predict how long an assignment 
will take. To minimize the number of points you
can lose, the best strategy:

 - Submit the starter code, it might pass tests.
 - Implement enough logic to pass the first test. 
   Then submit that on Inkfish.
 - Repeat for each test, or each couple of tests.
 - Really do work one test at a time - don't get
   distracted by future tests early.

It can be *faster* to start with a simple design
that can't possibly solve the whole problem and
then progressively rewrite then to spend any effort 
worrying about "design". The reason for this is 
simple: you get the other incidental pieces working.

Example:
 
 - The shell clearly requires recursive evaluation 
   in the general case.
 - The first 8 tests don't require recursive evaluation
   or an explicit tree at all.
 - So do the easy thing first and pass 8 tests. 

Finally, my #1 recommendation for writing software:

Never write more than 10 lines of code without
running your program and testing that it's working,
even if that just means printing out an intermediate
value.

## Shell Evaluation Plan

Assumption: We have an AST, we're trying
to evaluate it.

Note: This plan is a good place to start, but
may be wrong. Specifically, other features
may put constraints on when you should fork.

 - Base case: "command arg1 arg2"
   - fork 
   - in child: exec(command, arg1, arg2)
   - in parent: wait on child
 - Semicolon: "command1; command2"
   - execute command1 recursively
   - execute command2 recursively
 - AND / OR: "command1 OP command2" (op in {||, &&})
   - execute command1 recursively and
     collect it's exit status
   - if exit status correct, execute
     command2 recursively
 - Background: "command1 &"
   - fork
   - in child: execute command1 recursively
   - in parent: don't wait
 - Background: "command1 & command2"
   - recursively execute "command1 &"
   - execute command 2 recursively
 - Redirect: "command1 OP file" (op in {<, >})
   - fork
   - in child:
     - do the redirect, messing with fds
     - recursively execute command1
   - in parent: wait
 - Pipe: "command1 | command2"
   - fork [0], in child:
     - pipe(fds)
     - fork [1], in child:
       - replace stdout with fds[1]
       - recursively execute command1
     - fork [2], in child:
       - replace stdin with fds[0]
       - recursively execute command2
     - wait on [1],[2]
   - wait on [0]










