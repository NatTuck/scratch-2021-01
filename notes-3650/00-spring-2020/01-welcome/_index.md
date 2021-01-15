+++
title = "./01-welcome/notes.md"
+++

## Welcome to 3650

 - Instructor: Nat Tuck
 - Course: CS3650 - Computer Systems

Where does this course fit in?

 - You're a CS major, or maybe from COE.
 - You can write computer programs.
 - In this course, we explore some of the details of how
   actual programs run on concrete computers.

The plot:

 - To do things, programs need to use hardware resources.
 - 1980 personal computer: one program at a time.
 - Two programs at a time means conflicts (who gets input from
   keyboard? don't want to mix output to line printer!)
 - Add a dedicated program to talk the the hardware: the OS. Other
   programs ask the OS to access shared resources for them.
 - To ask the OS to do stuff for you, you make a system call.
 - This class is about writing programs that use system calls.
 - System calls are different on different operating systems,
   so we need to pick a specific one to use.
 - We're using Linux. More specifically, Debian 10.
 - Even with an OS, programs are still written to target a specific
   hardware archetecture.
 - Compiled programs are binary data - machine code - and different
   kinds of processors have different machine codes.
 - We'll be using the normal archetecture for desktop / laptop computers,
   the AMD64 archetecture.
 - A platform is the combination of processor archetecture and OS,
   for us that's AMD64 Linux.

## Course Resources

 - My site: http://khoury.neu.edu/~ntuck
 - Course Site / Syllabus
 - Piazza
   - If you get stuck, you can ask questions here.
   - You shouldn't generally post code.
   - Not for direct messages to course staff: use email for email.
 - AMD64 hints links
 - Bottlenose
 - Office Hours start Monday.

## Bottlenose

 - Show Bottlenose
 - Show HW01 A and B.
 - Explain HW01a
 - Delay HW01b

## Syllabus

 - There's a schedule. It may resemble what happens.
 - Grades: Assignments.
 - Homework: These are difficult programming assignments.
 - Challenges: These are very difficult programming assignments that
   you are not expected to get 100% on.

Cheating

 - Copying code without clear, written attribution is plagarism.
 - If you submit plagarized work, you fail the course.
 - You're not allowed to share solution code with other students either.
 - If you cheat, you get reported to the college, which is bad.
 - You are given starter code for assignments, you can use that.
 
 - There is code shown in lecture. It's not starter code, so using
   it without attribution is plagarism. This is the one case where I
   might be lenient on the policy, but I also may just give you an
   F for the semester on the first offense.

The best way to avoid cheating (and the best way to learn the content
in this course), is to personally type your own code. Don't download
other people's solutions, don't copy and paste other people's code, etc.

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
 - Eventually, "addq $1, ..." happens to it.
 - Back in main, %rax is moved to %rsi, and printf is called.

This almost makes sense, but it's a bit of a mess. Let's figure it out.

## AMD64: ISA and ASM

Intel released the 8086 processor in 1978. It was based on the earlier 8008
processor from 1972, but...

The 8086 was a 16-bit microproessor. That means:

 - It had a 16-bit data bus connecting it to memory and maybe other stuff.
   - That means a processor and RAM connected by 16 wires.
 - How much RAM can we address with 16 bits?
 - In addtion to RAM, this system gives us another place to put stuff called
   registers. For a 16-bit processor, each register is 16 bits.
 - The 8086 had 9-ish registers:
   - "general purpose": ax, cx, dx, bx, si, di, bp, sp,
   - "special purpose": ip, (segment registers, status register)
 - What processors do is execute instructions. Kinds of instructions:
   - Arithmetic: Example: add $5, %cx
   - Test: cmp $5, %cx
   - Conditional branch: jge bigger_label
   - Movement instruction: mov (%sp), %dx
   - A bunch of other stuff. You'll want to have a reference sheet.
 - Instructions tend to operate on at least one register.
 - Instructions can operate on memory addresses. If they do, the CPU needs
   to stop and read or write from RAM.

The 80386 or i386 was a 32-bit microprocessor, backwards compatible with
the 8086. This was the first "Intel x86" processor:

 - It had a 32-bit data bus.
   - How much RAM can we address with 32-bits?
 - It had 32-bit registers.
   - If you used the old names (eg. %ax), you got the least significant
     16-bits of the register.
   - Each register got a new name with an "e" at the front to refer to
     the full 32 bit "extended" register:
     - eax, ecx, edx, ...

The AMD Athlon 64 was a 64-bit microprocessor, backwards compatible with the Intel
8086 and i386. This was the first "AMD64" processor:

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

And that's where we are today. Let's write an add2 program by hand in amd64
assembly:

```
# add2.s

  .global main
  
  .text
# long add2(long x)
#   - the argument comes in in %rdi
#   - we return the result by putting it in %rax
add2:
  enter $0, $0
 
  # long y = x;
  mov %rdi, %rax
  
  # y = y + 2;
  add $2, %rax

  # return y;
  leave
  ret

main:
  enter $0, $0

  # long x = 5;
  mov $5, %rdi
  
  # y = add1(x)
  call add2
  # result in %rax

# printf("%ld\n", y)
#  - first arg goes in %rdi
#  - second arg goes in %rsi
#  - for a variable arg function, we need to zero %al
#    - %al is the bottom 8 bits of %ax/%eax/%rax
  mov $long_fmt, %rdi
  mov %rax, %rsi
  mov $0, %al
  call printf

  leave
  ret
  
  .data
long_fmt: .string "%ld\n"
```

To compile this simple hand-written assembly, we use:

```
$ gcc -no-pie -o add2 add2.s
```

## HW01a

A local Linux VM:

 - The easiest way to do programming work is to have the development
   environment installed locally on your personal computer.
 - For Linux systems programming, Linux *is* our development environment.
 - Having it installed as your main OS is probably best.
 - But, for consistentency, the assignment is for everyone to install
   exactly Debian 10 64-bit in a VirtualBox virtual machine.
 - If you aren't developing on the VM and you run into weird problems later in
   the semester, use this VM to rule out configuration issues.

The CCIS server:

 - ssh ntuck@login.ccs.neu.edu
 - This is a shared Linux server.
 - This is a generally useful tool, and it will be possible
   to do some of your homework on this server.
 - Working directly on a remote server is a good reason to learn
   a command line editor like vim.
   
 - Show Putty, WinSCP web page
 - Show ssh and scp on Linux
 - Show Cyberduck page

## HW01b - First Programming HW

 - Download starter code.
 - Write some simple C and ASM code.
 - Make sure it compiles and runs.
 - Pack it back up and submit.

This assignment is mostly about structure, process, and getting annoyed
at the autograder.

Keep in mind:

 - A C (or asm) program is a collection of functions. 
 - These functions can be in one source file or in a bunch of different files.
 - C functions and ASM functions are the same thing. You can mix them together
   in the same program.
 - It's easiest if each file is all-C or all-ASM.

Object file example:

 - add1.c
 - add2.c
 - main.c

```
$ gcc -c -o add1.o add1.c
$ gcc -c -o add2.c add2.c
$ ...
$ gcc -o example add1.o add2.o main.o
```

## Another Assembly Example

 - Scan through the AMD64 instruction list on course site.
 - Example: cond\_br

```
$ gcc -no-pie -o prog prog.s
```
