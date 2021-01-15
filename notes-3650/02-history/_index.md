+++
title = "Lecture 2: Some History"
+++

## First: HW Questions

 - Homework questions?
 - HW02 is available.

## Course Resources

 - In case you missed it: https://ntuck-neu.site/
 
## C -> ASM

 - "Programming" means "writing C code".
 - On Linux-like (UNIX, *nix, POSIX) systems, the operating system
   API is primarily exposed to C programs through the system C library.
 - The hardware doesn't run C though - it runs amd64 machine code (on your
   laptop) or ARM machine code (on your phone) or maybe some other machine
   code.
 - Machine code is for machines, not humans, so it's hard to read.
 - Machine code is a series of instructions. If you write the instructions
   down as text, you get assembly language.
 - To run a C program, you need to translate to machine code (or "binary").
 - Conceptually, and historically, you first translate C -> ASM, then 
   ASM -> binary.
 - You can still do this if you explicitly ask for it.

Note: For the first few homeworks you will be writing ASM programs. You
should *not* have a compiler do this for you. Submitting compiler output
for an assembly assignment is worth zero points.

(This may be where time runs out for lecture 1)

Example:

```
// A C program is a collection of functions.
// Here's a minimal program with one function
int
main(int argc, char* argv[])
{
  printf("Hello C program\n");
  return 0;
}

```

```
# Direct C => binary
$ gcc -o hello hello.c
$ ./hello

# C => asm
$ gcc -S -o hello.s hello.c
# take a look at hello.s

# asm => binary
$ gcc -o hello hello.s
$ ./hello
```

Interesting stuff in hello.s:

 - The string is there, but no newline.
 - The main function exists
   - Starts at label "main"
   - Ends at "ret".
   - Declared ".globl"
 - In the main function another function is called - not printf, but puts.
 - The optimizer got to us.

Let's tell it to be less clever:

```
# C => asm
$ gcc -fno-builtin -S -o hello.s hello.c
# take a look at hello.s
```

 - Now the string has a newline.
 - And the function called is "printf".

How about with two functions:

```
// add1.c
long
add1(long x)
{
    return x + 1;
}

int
main(int _ac, char* _av[])
  // initial _ marks args as not used
{
    long x = add1(5);
    printf("%ld\n", x);
    return 0;
}
```

```
# C => asm
$ gcc -S -o add1.s add1.c
# take a look at hello.s
```

 - Two functions: add1, main
   - each starts at label, ends at "ret"
 - In main, the value 5 is moved to "%rdi"
   - That must be where the function's first argument goes.
   - No, that's "%edi"
   - I said "%rdi", wait a second...
 - Then add1 is called
 - In add1, the value from %rdi goes to some places.
 - Eventually, "addq \$1, ..." happens to it.
 - Back in main, %rax is moved to %rsi, and printf is called.

This almost makes sense, but it's a bit of a mess. Let's figure it out.

## AMD64: ISA and ASM

Systems ends up involving more archaeology than clean-slate engineering.

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
// idiv.S
.global main
.text
main:
  enter $0, $0
  
  mov $40, %rax
  mov $30, %rbx
  mov $20, %rcx
  mov $10, %rdx
  
  // cqo 
  idiv %rdx
 
  // mov %rdx, %rdi
  // cqo 
  // idiv  %rdi
  
  mov \$longfmt, %rdi
  mov %rax, %rsi
  mov $0, %al
  call printf
 
  leave
  ret
.data
longfmt: .string "%ld\n"
```

## More History: UNIX and C

One of the most significant market monopolies of the 20th century was the AT&T
telephone monopoly. Before they were broken up in 1983, they were able to charge
crazy prices for telephone service. For example, they charged \$30/month to rent
a (landline, rotary, analog) phone from them and didn't allow other phones on
their network.

But there was one upside to this abusive monopoly: They used some of their crazy
profits on R&D. Their R&D arm, Bell Labs, is probably the most significant
contributor to modern computing: Most importantly they invented the transistor,
but they also developed the UNIX OS and C programming language.

Some researchers at Bell Labs needed an OS, so UNIX and C were developed
together. UNIX was the development environment for C. C was the programming
language to build UNIX. It was built by programmers for programmers, with the
specific idea that simple programs that work with plain text when possible are
way more likely to work and be correct than larger more complicated software.

So in the early 80's, UNIX had the big advantage of actually working and being
able to handle moderate complexity. AT&T wasn't in the OS business, so they
licensed it to anyone who wanted it for cheap and weren't afraid of including
the source code.

When AT&T was broken up, a bunch of major companies licensed UNIX and produced
their own versions. IBM had AIX, Sun had Solaris, HP had HPUX, etc.
Compatibility between these commercial UNIXes was a mess, but software that was
portable between them could be and was written. 

In the 80's and early 90's, these UNIXes were the standard in academia and
larger industry. PCs weren't seen as, and in many cases weren't, powerful and
flexible enough to overtake the servers and workstations that had overtaken
mainframes and minicomputers in these domains.

For some applications - OS research, hardware development, etc - it's really
useful to have the full source code for the computer system you're working with.
Early AT&T UNIX was one of the few heavy duty systems where that was possible.

In addition to commercial vendors, one of the source licensees of early UNIX was
the University of California at Berkley. By the 90's they had completely
rewritten the OS into the Berkley Software Distribution (BSD), but the UNIX
copyright holder sued, and it wasn't until 1994 that the legal issues were
settled and BSD could be freely distributed.

A parallel attempt to create a freely distributable source-available UNIX clone
was started in the mid 80's as the GNU project, and this became usable in 1991
with the release of the Linux kernel.

The ~2 year delay of the BSD lawsuits gave Linux enough time to take over as the
most popular free UNIX, but licensing issues mean that BSD code gets reused in
proprietary systems more. Modern Mac OS is a direct descendent of BSD, but every
reasonably large modern OS has BSD code in it today.


