+++
title = "./02-amd64-assembly/notes.md"
+++

## First: HW Questions

 - Homework questions?
 - HW02 is available.

## Course Resources

 - In case you missed it: http://khoury.neu.edu/~ntuck/

## AMD64: ISA and ASM

Last time we compiled a simple program and the assembly didn't make much sense.
To explain what's going on, we need some history:

Intel released the 8086 processor in 1978. It was based on the earlier 8008
processor from 1972, which was based on the 4004.

At some point there must have been an 8 bit microprocessor.

 - It had a 8-bit data bus connecting it to memory and maybe other stuff.
   - That means a processor and RAM connected by 8 wires. (10 wires?)
   - ++++++ DRAW PICTURE ++++++
 - How much RAM can we address with 8 bits?
 - In addtion to RAM, this system gives us another place to put stuff called
   registers. For an 8-bit processor, each register is 8 bits.
 - This processor had 5-ish registers:
   - general: a, b, c, d 
   - special: ip ("instruction pointer")
 - What processors do is execute instructions. Kinds of instructions:
   - Arithmetic: Example: add \$5, %c
     - Actually probably "add \$5", implicitly operating on %a.
   - Test: cmp $5, %c
   - Conditional branch: jge bigger_label
   - Movement instruction: mov %b, %d; mov (%b), %d
   - A bunch of other stuff. You'll want to have a reference sheet.
 - Instructions tend to operate on at least one register.
 - Instructions can operate on memory addresses. If they do, the CPU needs
   to stop and read or write from RAM.

The 8086 was a 16-bit microproessor. That means:

 - It had a 16-bit data bus connecting it to memory and stuff.
 - How much RAM can we address with 16 bits?
 - The 8086 had 9-ish registers:
   - "general purpose": ax, cx, dx, bx, si, di, bp, sp
     - x is for extended; new registers are "indexes" and "pointers". 
   - "special purpose": ip, (segment registers, status register)
   - Can access low half of %ax with %al, high half with %ah.

The 80386 or i386 was a 32-bit microprocessor, backwards compatible with
the 8086. This was the first "Intel x86" processor:

 - It had a 32-bit data bus.
   - How much RAM can we address with 32-bits?
 - It had 32-bit registers.
   - If you used the old (16-bit) names (eg. %ax), you got the least significant
     16-bits of the register.
   - Each register got a new name with an "e" at the front to refer to
     the full 32 bit "extended" register:
     - eax, ecx, edx, ...

The AMD Athlon 64 was a 64-bit microprocessor, backwards compatible with the
Intel 8086 and i386. This was the first "AMD64" (x86\_64, x64, Intel 64; not
ia64 even though x86 is sometimes called ia32) processor:

 - It had a 48-bit data bus, designed to be extended up to 64-bit later.
   - How much RAM can we address with 64 bits?
   - How about 48 bits?
 - It had 64-bit registers.
   - If you used the old names (e.g. %ax, %rax), you got the least significant
     16 or 32 bits of the register.
   - Each register got a new name with an "r" at the front to refer to
     the full 64 bit register.
     - rax, rcx, rdx, ...
   - 8 new general purpose registers were added: %r9, %r10, ..., %r15

## add1 again

(grab the asm from last class)

Argument registers: The convention for AMD64 Linux assembly is to pass
arguments in registers: %rdi, %rsi, %rdx, %rcx, %r8, %r9, ...stack

Functions return a value in the %rax register.

## Another Assembly Example

First, Scan through the AMD64 instruction list on course site.

Example: fact (iter)

```
$ gcc -no-pie -o ifact ifact.s
```

## idiv example

```
.global main
.text
main:
  enter $0, $0
  
  mov $40, %rax
  mov $30, %rbx
  mov $20, %rcx
  mov $10, %rdx
  
  # cqo 
  idiv %rdx
 
  # mov %rdx, %rdi
  # cqo 
  # idiv  %rdi
  
  mov $longfmt, %rdi
  mov %rax, %rsi
  mov $0, %al
  call printf
 
  leave
  ret
.data
longfmt: .string "%ld\n"
```

More Examples:

 - fact (recursive)
 - strlen

 - fact hits the recursion problem, talk about register conventions
 - Talk about the ABI: https://github.com/hjl-tools/x86-psABI/wiki/X86-psABI
 - Programmer Manual: https://support.amd.com/TechDocs/24594.pdf
 


