
# HW Questions

 - 

# System Calls

 - If a program wants to do stuff, it needs to
   ask the OS for help, so it makes a syscall.
 - So far, we've been using the C standard library
   to do stuff, and it makes our system calls for us.
 - We can make system calls ourselves.
 - To make a system call, you use the 
   "syscall" instruction.

## Standard File Descriptors

When we start a process on Linux, there are normally three already open file descriptors:

 - 0 is stdin, reads normally read from the console
 - 1 is stdout, writes normally print to console
 - 2 is stderr, writes normally print to console

# Example: Arrays



# What does lea do?


```
# We have a mov with a memory ref as first arg.
mov 8(%rcx), %rdx

# This treats %rcx as containing a memory address,
# adds 8 to that value, and then loads 8 bytes from
# memory to %rdx

lea 8(%rcx), %rdx

# Do the exact same thing, except keep the 
# calculated address instead of doing the load.
```


 
