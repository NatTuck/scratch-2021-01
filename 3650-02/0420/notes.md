
# Challenge 3 / Extra Credit Questions?

# Semester Review

What was "computer systems"?

 - We explored the interface between application
   programs and the computer systems they run
   on.
 - A computer system is:
   - A computer hardware platform (~ Instruction
     Set Archetecture)
   - An Operating System
 - Specific Example for this semester:
   - AMD64
   - Linux (Debian 10)
 - APIs provided by the system:
   - Machine instructions 
   - System calls

Overall, we did a few deep dives into specific
interfaces rather than trying to cover everything:

 - You should know a lot about memory allocators.
 - Possibly less about process scheduling
 
Two broad take-aways: 

 - There's no magic in any of this, just moderately
   complicated C code.
   - You should now be prepared to read (and even
     modify) pretty much any component of a modern
     OS - if it's open source you can go do that
     right now if you want.
 - Systems involves archaeology.
   - New elements tend to either:
     - The new thing is an alternative to an existing
       component, and now there are two options.
     - The new thing is a new layer.
       - New layers simplify things in the simple case.
       - Abstractoins are leaky, and you end up
         needing to understand all the layers anyway,
         so this just adds complexity.

Things get complicated:

 - Historically, ext style filesystems lost data
   on crash.
 - To avoid this - especially for config files - a 
   common pattern was used:
   - The new version of the file was written to a
     temporary file.
   - Then you'd rename the temp file over the
     real file.
 - This worked pretty well, the rename happened
   after the write, so this effectively made file
   updates atomic.
 - Problem: ext4 added an optimization called
   delayed block allocation
   - That potentially re-ordered the write
     to *after* the rename.

Old sequence:

 - Create empty temp file
 - Write contents
 - Rename temp over file

New sequence:

 - Create empty tmep file
 - Rename over real file
 - Write contents

This was a serious data loss bug that broke
systems.

This didn't violate any written standard. The documented
contract of the filesystem didn't guarantee any ordering
for anything.

The API to a computer system isn't what you document,
it's what you ship.

So ext4 was patched to detect this specific case and
not delay writes after renames.

## Basic Computer Organization

```

 +-------------+                   +------------+
 |    CPU      |===================|     RAM    |
 | A B C D  IP |                   |            |
 +-------------+                   +------------+
       ||
       ||
       ||
     +------------+
     | I/O device |
     +------------+
```

What the CPU does:

 * Load an instruction from memory at %IP
 * Execute that instruction
   * Increment %IP
 * Repeat

## Two Programming Languages: AMD64 Assembly + C

 - Two programming languaes
 - One mental model for program execution
   - Logically, C compiles to assembly
 - Computer executes a series of instructions
 - Assembly is a way to write down those instructions
 - Kinds of instructions:
   - Arithmetic instructions
   - Labels, jumps, conditoinal jupms
   - Load / store instrucitons
   - Fucnctions
   - System calls
 - Concepts
   - Registers
   - Memory + Memory Address
   - Names shared across modules
 
## Assembly Assignments

 - Wrote a couple of simple assembly programs
 - Translated larger C program into assembly

## Looking at C

 - Data structures

## Processes and Virtual Memory

 - A modern computer system can run more than one
   program at a time.
 - Older systems ran one program at a time.
   - Those programs could access system resources
     like hardware directly.
 - Each instance of a running program is a "process".
 - Separate processes are isolated by various mechanisms
   implemented in the OS and hardware.
 - The hardware is *virtualized*, so that each process
   seems to get full access to it.
 - Most relevently, that involves virtual memory.

```
   +------------------------+
   |                        |
   +------------------------+
   |      stack             |
   +------------------------+
   |                        |
   +------------------------+
   |      heap              |
   +------------------------+
   |                        |
   +------------------------+
   |      .data             |
   +------------------------+
   |      .text             |
   +------------------------+
```

## Process API

 - fork() - creates a process by copying an existing
            process
 - exec() - takes an existing process and loads a new
            program to run in it
 - waitpid() - waits for a process to finish

Simpler standard library API:

 - system() - creates a new process, runs a shell command,
              and waits for it to finish

## Files and File descriptor

 - File descriptor table
 - open / close / read / write
 - pipe
 - dup

## Challenge 1: The shell

Conclusion: Debugging across fork is annoying.

## Virtual Memory API

One function:

 - mmap

Does everything:

 - Allocate normal memory
 - Allocate shared memory
 - We can hook up files for mmap I/O
 - etc

Further,

 - All memory mappings are "backed" by something so
   that memory can be swapped out.
 - Executable files and libraries are loaded into
   memory via mmap.

## Processes and Threads

 - Fork + mmap shared => shared memory between
   multiple concurrently executing processes.
 - Mutable shared memory => data races => locks =>
   deadlock => suffering.
 - Same problem when we use threads and allocate
   memory at all.
 - On Linux, threads and processes are pretty
   similar:
   - GDB works more easily with threads.   
 
## Parallel Sorting

Sample sort:

 - processes + mmap I/O
 - threads + file I/O

We have multiple cores, so we should be able to
get a parallel speedup.

You couldn't get a perfect parallel speedup:

 - System calls are slow.
 - Slow algorithm choices are slow.
   - You can go faster with more than 3*P samples.
   - 100*P gives much more even distribution
 - Partitioning wasn't done in parallel - it can be.

## Memory Allocator

 - memory comes from mmap, but we can't just call
   mmap to get memory
   - minimum allocation is 4k, smaller allocations
     are inefficinet
   - the mmap/munmap interface doesn't match the
     malloc/free C standard

Basic solution: a free list

Challenge: Optimized, thread safe allocator

## OS kernels

 - The kernel is the component of the OS that can
   break the process isolation abstractoins to
   access hardware and manage resources
 - Specific code: xv6

Assignment: Add a system call


## File Systems

 - FAT gave us files, directories
 - ext added inodes
   - are hardlinks useful?
 - ext2 added block groups for performnace
 - ext3 added journaling for reliability
 - ext4 added more optimizations 
 - CoW filesystems are the fancy modern thing

## Stuff to do in the future

Classes:

 - Networks and Distributed Systems
 - Compilers

Other stuff:

 - Explore stuff you find interesting.
 - Write code!

## Any questions?



