---
layout: default
---

# Computer Systems

## HW Questions?

 - HW02 was due.
 - HW03 is available.

## Finish Up Linked List

 - Done: car, cdr, cons
 - Todo: free_list, sum, main

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

 - lines.S
 
## Translation Example

 - rev-str-array.c => rev-str-array.S

## Explaining C Hello World

```
#include <stdio.h>
// hello.c
int
main(int _ac, char* av[])
{
    printf("address of main is: 0x%016lx\n", (unsigned long) main);
    return 0;
}
```

```
$ gcc -no-pie -S -o hello.s hello.c
```

 - .text .section .rodata (Segments contain Sections)
 - C strings go in an ".rodata" segment, which is read only.
 - .file, .type - For the debugger 
 - .cfi_* ("call frame information") - Debugger and C++ exceptions
 - Didn't use enter, but did use leave.
   - Enter is here: pushq %rbp; mov %rsp, %rbp; subq $16, %rsp
 - Stack references are negative offsets from %rbp
   
```
$ gcc -no-pie -o hello hello.c
$ ./hello
??
$ ./hello
??
$ gcc -o hello hello.c
$ ./hello
??
$ ./hello
??
```

```
$ gcc -S -o hello.s hello.c
```

 - "pie" is "Position Independent Executable"
 - Instead of compiling the program to always live at the same memory address,
   program can be moved in memory.
 - This makes some security bugs harder to exploit.
   - gets, overwrite rv on stack, return to known location
 - call printf@PLT   # indirect reference via global table to find printf
 - leaq .LC0(%rip)   # indirect reference relative to instruction pointer for local label

