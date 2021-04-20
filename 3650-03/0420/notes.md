
# Challenge 3 or Extra Credit Questions?

 - ioctl 
   - Nothing for the challenge
   - /dev/cdrom - set the write speed

# Semester Review

What was "computer systems"? What did we talk
about this semester?

 - We looked at the programming interface between
   application programs and a computer system.
 - A computer system has two parts:
   - A computer hardware platform (~ Instruction
     Set Archetecture)
   - An operating system
 - Each of those provides an API:
   - A set of machine instructions 
   - A set of system calls

We didn't cover everything. Instead we covered some
specific examples in some depth.

Two broad take-aways:

 - There's no magic in any of this. It's just moderately
   complicated C code.
 - Understanding systems is as much Archaelology
   as Engineering.
   - New parts of the system tend to be either:
     - An alternative for an existing component.
     - A new abstraction layer built on top of
       the pile.

Things get complicated:

 - Historically, ext file system lost data on crash.
 - To avoid this wehn writing important files - like
   config files - pepole followed a pattern:
   - First the new version of the file was written
     to a temporary file.
   - Then it would be renamed over the real file.
 - In practice, this made these writes atomic.
 - Problem: Ext4 had an optimizaiton called
   delayed block allocation
 - This allowed the data blocks to be reordered
   to after the rename.

Old sequence:

 - Create new empty temp file
 - Write data to new file
 - Rename over origional

New sequence:

 - Create new empty temp file
 - Rename over origional
 - Write the data to the file, delaying sync for
   like 5 seconds

This was a pretty frequent data corruptoin bug
that had to get fixed.

Ext4 was technically correct - they were following
the written spec.

The written spec isn't the real spec. The spec is
what real systems actually do.

## Basic structure of a computer

```
  +--------------+                    +---------+
  |   CPU        |====================|  RAM    |
  | A B C D   IP |                    |         |
  +--------------+                    +---------+
       || 
       ||
   +-------------+
   |  I/O device |
   +-------------+
```

What the CPU does:

 * Read an instruction from memory at the address
   stored in %IP
 * Execute that instruciton
   * Increment %IP, usually
 * Repeat

## Two languages: C and AMD64 Assembly

 - Two languages; one mental model for program
   execution.
   - Logically, C code compiles to assembly
 - Computer executes a series of machine instructions
 - Assembly is just a human-readable text form of
   those instructions
 - Kinds of instructions:
   - Arithmetic
   - Load / store
   - Jumps, conditoinal jumps
   - System calls
 - Ohter relevent concepts:
   - Functions
   - Memory and memory addresses
   - Registers
   - Call stack
   - Names shared across modules
 - Big difference between C and assembly:
   - The ability to name local variables

## Assembly Assignments

 - Wrote a couple of simple programs
 - Translated a larger program C => ASM

## Processes and Virtual Memory
 
 - OS and hardware work to isolate separate 
   running programs.
 - This is done through a couple kinds of 
   virtualizatoin
 - Virtualizing hardware:
   - Instead of accessing the screen directly,
     programs have an OS API (syscalls) to
     print to the screen.
   - Virtual memory 
 - An running instance of a program is called
   a process.

```
 +--------------------------+
 |                          |
 +--------------------------+
 |      stack               |
 +--------------------------+
 |                          |
 +--------------------------+
 |      heap                |
 +--------------------------+
 |                          |
 +--------------------------+
 |      .text               |
 +--------------------------+
 |      .data               |
 +--------------------------+
```

## process API

 - fork() - Clone the current process, now
            you've got two of them.
 - exec() - Load a new program into the current
            already running process.
 - waitpid() - Wait for a process to exit

The common case

 - system() - Create a new process, run a shell command
       in it, and then wait for it to exit.

## Files and File Descriptors

 - FD table
 - open / close / read / write
 - pipe
 - dup

## Challenge 1: The shell

Conclusion: Debugging across fork is obnoxious.


## Virtual Memory API

One function does everything:

 - mmap

This lets you:

 - allocate memory
 - allocate shared memory
 - map files into memory for I/O
 - map files into memory read-only

Further,

 - All memory mappings are backed by something
   in case they need to be swapped out.
 - Executable files and libraries are loaded into
   address spaces with mmap as read-only memory.

## Processes and Threads

 - fork + mmap shared means we have shared memory
   across processes.
 - Mutable shared memory => data races => locks
   => deadlock => suffering.
 - Same thing with threads and any memory
 - On Linux threads are basically the same
   as processes.
 - Thread advantage: GDB likes them better
 
## Parallel Sorting

 - Sample sort with:
   - processes + mmap i/o
   - threads + open/read/write/close
 - Goal: we've got multi-core processors, we should
   be able to get a parallel speedup
 - Problem 1: Syscalls are slow
 - Problem 2: Slow algorithms are slow
   - 3*P isn't enough samples
   - 100*P gives much better speedup
   
## Memory Allocators

 - we can allocate memory with mmap

Problems:

 - minimum size of one page
 - syscalls are slow
 - API mismatch with malloc / free
   - munmap requires size, but free doesn't 

Simple solution: free list

Challenge 2: Optimized, thread-safe allocator

## OS Kernels

Assignment: Add a system call to a kernel

Kernel: xv6

## File Systems

 - FAT: files, directories, nested directories, metadata
 - ext: inodes
   - new feature: hardlinks
 - ext2: block groups
 - ext3: journaling
 - ext4: optimiztions
 - CoW file system
 
## Stuff to Explore in the Future

Classes:

 - Networks and Distributed Systems
 - Compilers

Other recommendatoins:

 - If it's interesting, study it.
 - Write code!












