
# Homework Questions?

Segmentation fault:

 - Program tries to access memory at an invalid
   address.
 - Debug by running in gdb.
 
Stuff that causes segfaults:

 - Instructions with parens in them.
 - Stack access
 - Function calls / returns

## System Calls

 - If you want your program do do anything, you'll
   need to make a system call.
 - We've been using C stdlib calls to do this so
   far.
 - These C library functions are implemented
   as a series of machine instructions.
 - To do a system call, we use the ```syscall``` 
   instruction, which works a lot like ```call```.
  
## Standard File Descriptors

When we run a program, there are three already
open file descriptors (in the FD table):

 - 0: stdin - defaults to reading from the console
 - 1: stdout - defaults to writing to the console
 - 2: stderr - also defaults to writing to console

## Write a program called "prime"

## Arrays and Pointers

 - Arrays in C are just a shorthand for pointer
   operations.
 - The array index operation is a pointer operation

```
aa[bb]   ===     *(aa + bb)
```

How does it turn ii into the proper offset taking
into account the sizeof each element?

```
int aa[10];
char *bb;

aa[5] = 4;     // becomes asm (aa + 5*sizeof(int))
bb[3] = 'q';   // becomes asm (bb + 3*sizeof(char))
```

```
int aa[10]; // aa is effectively type "int*"

aa + 3      // in assembly, this is aa + 3*sizeof(int)
```







  
  
   


