
# CS 3650: Computer Systems (02)

Instructor: Nat Tuck

Course web site: https://ntuck-neu.site/

# What is Computer Systems?

Where does this course fit in?

This course focuses the idea that we target specific
computers with our computer programs.

So we need to look at what a concrete computer platform
/ computer system lets us do.

A computer system (or platform) is the combination
of a type of processor (or "Instruction Set Archetecture")
and an Operating System.

Instruction Set Archetectures:

 - PowerPC
 - x86
 - Intel 8080
 - MIPS
 
 - ARM64
 - amd64 / intel64

Operating System:

 - Windows
 - GNU+Linux
 - Mac OS
 
 - Open source BSDs: OpenBSD, FreeBSD, NetBSD, 
   DragonflyBSD, etc
 - Android
 - iOS

Our computer system for this semester will be:

 - ISA: AMD64 
 - OS: Debian 10

# Getting a program to do stuff

 - To do stuff, we need to use hardware resources.
   - Print text on the screen, we need to ask the screen
     to do that for us.
   - Read from keyboard? Must access keyboard.
 - Problem: More than one program at the same time.
   - Two programs read from keyboard at the same time.
   - User presses a key.
   - What happens?
     - Both programs respond.
     - Only one program responds ("in focus")
 - Solution: Add an extra program to act as referree for
   access to shared resources.
     - The "operating system" or OS Kernel.
     - Only the kernel accesses hardware.
     - User programs must ask kernel for shared
       resource stuff.
 - Another benefit to an OS and having programs target
   that rather than the hardware directly is portability.
   - Only need to port the OS / kernel to new hardware
     platforms.



   

