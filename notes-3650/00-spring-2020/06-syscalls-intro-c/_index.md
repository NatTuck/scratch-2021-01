+++
title = "./06-syscalls-intro-c/notes.md"
+++

# Computer Systems

## HW Questions?

 - HW03 is due soon.
   - All or nothing tests, so TAs will look for extra partial credit.
 - HW04 is available.

## System Calls

 - We've been using the C standard library to access OS functionality,
   but that's just calling functions made of normal code.
 - In order to do things, these library functions need to call into the
   OS kernel.
 - This is done with the syscall instruction, which works a lot like
   the call instruction.
 - The Linux kernel exposes about 300 syscalls. 
   [List of Syscalls](https://filippo.io/linux-syscall-table/)

## Syscall Example

Standard file descriptors:

 - 0: stdin
 - 1: stdout
 - 2: stderr

 - Show lines.S

## Programming in C
 
 - Let's write "prime", which prints the Nth prime number.
 - Top-down design.
   - Allocate an array for N primes.
   - Pre-load first 3 primes.
   - A number is-prime if it's not divisible by any prime up to sqrt(x).
 - Makefile first.
   - gcc command
   - clean rule
   - tabs
 - Specifics:
   - Local variables are "on the stack".
   - Can allocate arrays "on the stack".
   - Standard library functions and headers:
     - stdio.h: printf
     - stdlib.h: atol
     - math.h: ceil, sqrt -- show manpage, talk about libm
   - Casts. For numbers, these convert.
  - prime

## Arrays and Pointers

 - Arrays in C are just like arrays in ASM. They're a contigous sequence of
   memory starting at some address.
 - Array variables store the address of the start of the array.
 - C knows how big the type stored in array is.
 - For arrays in ASM, we had to manually calculate the byte offsets.
 - An array of words in C would be xs[0], xs[1], etc.
 - Pointers are variables that store memory addresses with an associated type
   so "int\*" (int pointer) is the address of an int.
 - C lets you do arthmetic with pointers.
   - If you add one to an int pointer, it steps to the next int-sized chunk of memory.
     So if p is a pointer to a 4 byte int, then (p + 1) is the address increased by
     4 bytes.
   - Show sizeof: char, short, int, long, float, double, char\*, long\* 
 - Array varibles and pointers are the same thing with different notation.
 - If pp is a pointer and aa is an array, both of the same type pointing to the same
   address, then pp[3] and \*(pp + 3) are the same thing.
 - Strangely, pp[ii] and ii[pp] are the same thing too.
   - Why?

