
# CS3650: Computer Systems

Instructor: Nat Tuck

Course site: https://ntuck-neu.site/


# Where does 3650 fit in?

 - Fundies 1: No such thing as a physical computer
 - Fundies 2: Move to Java, can do more stuff
 - This course: We admit that there's a concrete
   computer system that we're working on

A "computer system" consists of:

 - An instruction set archetecture (ISA), or
   a specific "kind" of processor.
 - An operating system

Instruction set archetectures:

 - x86
 - x86_64, aka AMD64, aka Intel64, aka x64
   - Your Desktop / Laptop
 - ARM64
   - Probably your phone
   - Maybe your very new Mac
 - PowerPC
 - MIPS

ISA defines what instructions a processor can execute,
and standardizes the general layout of computer hardware
(e.g. how does it boot)

Operating Systems:

 - Windows
 - UNIX
   - Linux
     - + GNU "GNU is Not UNIX"
       - Debian 10
       - Fedora
       - Ubuntu
       - SuSE
       - Arch
       - Red Star OS
     - Chrome OS
     - Android
   - Mac OS
 - iOS
 - FreeRTOS
 - Temple OS
 

Our computer system (or "platform") for the semester:

 - ISA: AMD64
 - OS: Debian 10

# Why Operating Systems?

 - Software uses hardware resources
 - Example: Read a keypress from keyboard
 - Historically, programs could just talk to the hardware
 - Problem: What if there's more than one program accessing
   the same hardware?
 - If there are two programs both reading a key from the
   keyboard at the same time, and you press a key, what
   happens?
   - Both programs get the input.
   - Only one program gets the input? How do we 
     decide which one?
     - Don't care? Random?
     - Focused window gets the input
  - Solution: Add a program to manage shared resources.
    - This is the Operating System
    - Probably the OS Kernel
    - The OS kernel is the only thing that accesses 
      hardware, othre programs that want to access shared
      resources ask the kernel to do it for them.

# Logging in to Inkfish

Khoury Account

https://my.khoury.northeastern.edu/

https://my.ccs.neu.edu/




