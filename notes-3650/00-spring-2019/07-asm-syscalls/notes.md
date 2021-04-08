---
layout: default
---

# Computer Systems

## HW Questions?

 - HW03 was due.
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

## HW04

 - Matrix transpose
 - Show the Perl tools
   - gen-matrix
   - transpose
   - equality

## Translation Example

 - rev-str-array.c => rev-str-array.S

