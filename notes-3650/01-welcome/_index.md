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

 - My site: http://ntuck-neu.site
 - Course Site / Syllabus
 - Piazza
   - If you get stuck, you can ask questions here.
   - You shouldn't generally post code.
   - Not for direct messages to course staff: use email for email.
 - AMD64 hints links
 - Inkfish
 - Office Hours start next Monday.
 - Three synchronized sections; Ferd Vesely is teaching the third one.

## Inkfish

 - Show Inkfish
 - Show HW01 - we'll go over this in a bit.

## Syllabus

 - There's a schedule. It may resemble what happens.
 - Grades: Assignments.
 - Homework: These are difficult programming assignments.
 - Challenges: These are very difficult programming assignments that
   you are not expected to get 100% on.

Cheating

 - Copying code without clear, written attribution is plagiarism.
 - If you submit plagiarized work, you fail the course.
 - You're not allowed to share solution code with other students either.
 - If you cheat, you get reported to the college, which is bad.
 - You are given starter code for assignments, you can use that. That is the
   *only* code you can use without writing it or giving attribution.
 
 - There is code shown in lecture. It's not starter code, so using
   it without attribution is plagiarism. This is the one case where I
   might be lenient on the policy, but I also may just give you an
   F for the semester on the first offense.

The best way to avoid cheating (and the best way to learn the content
in this course), is to personally type your own code. Don't download
other people's solutions, don't copy and paste other people's code, etc.

## HW01 - Part 1: Setup your Environment

A local Linux VM:

 - The easiest way to do programming work is to have the development
   environment installed locally on your personal computer.
 - For Linux systems programming, Linux *is* our development environment.
 - Having it installed as your main OS is probably best.
 - But, for consistency, the assignment is for everyone to install
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

## HW01 - Part 2: First Programming Task

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

Note: For the first few homeworks you will be writing ASM programs. You
should *not* have a compiler do this for you. Submitting compiler output
for an assembly assignment is worth zero points.

```
// add1.c
long
add1(long x)
{
    return x + 1;
}
```

```
// add1-main.c
int
main(int _ac, char* _av[])
  // initial _ marks args as not used
{
    long x = add1(5);
    printf("%ld\n", x);
    return 0;
}

// gcc -o add1 add1.c add1-main.c
```

Assembly is kind of like C except there are no curly braces and you don't get to
name your variables. Instead, we get a pre-named set of registers where we can
store numbers. By convention, specific registers correspond to specific uses
like first function argument and return value.

```
// add2.S

  .global add2
  
  .text
// long add2(long x)
//   - the argument comes in in %rdi
//   - we return the result by putting it in %rax
add2:
  enter $0, $0
 
  // long y = x;
  mov %rdi, %rax
  
  // y = y + 2;
  add $2, %rax

  // return y;
  leave
  ret

// gcc -no-pie -o add2 add2.S add2-main.c
```


