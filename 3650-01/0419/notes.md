
# Questions: Challenge 2, Extra Credit?

# Double check your grades on Inkfish!

 - If Inkfish is wrong about something, let me know.
 - If you have complaints about grading:
   - There's a two week limit on grade issues.
   - Hard deadline on grade fixes: Next Wednesday

# Today: Last Lecture; What did we cover this semester?

## Computer Systems

What did we cover in "computer systems"?

 - What happens in your computer.
   - The archetecture of stuff.
   
 - The interface that a computer system exposes
   to developers.
 - A computer system is the pairing of a specific
   computer platform / instruction set archetecture
   and a specific operating system.
 - Our concrete computer system:
   - AMD64
   - Linux, specifically Debian 10
 - Each of those components provides an interface
   to programmers:
   - ISA provides an instruction set.
   - OS provides system calls. 
 - We didn't cover all of this, instead we looked
   at specific examples in some depth.

Two broad things to take away from this course:

 - There's no magic in any of this, just a bunch of
   moderately complicated C code.
   - On open source systems, you can go read and
     modify the source code yourself.
 - Systems can involve archaeology.
   - Stuff rarely goes away once adopted.
   - New development tends to be:
     - An alternative to something.
     - Another layer on top of whatever's already there.

Things get complicated:

 - Historically, ext filesystems lost data on crash.
 - To avoid this when writing important files (e.g.
   config files), the following pattern was used:
   - Write the full file out to a new temporary file.
   - Once the temp file was written, rename it over
     the old config file.
 - This practice worked pretty well at avoiding
   lost/corrupted config files.
   - The write happened first.
   - The rename was atomic.
 - Problem: ext4 has an optimizaiton called delayed
   block allocation 
   - Data blocks aren't written until a bunch of
     them are queued up to maximize the length of
     writes that can be done.
   - Problem: Now the rename can be reordered before
     the write.

Old sequence:

 - Create empty temp file.
 - Write data.
 - Rename over old file.

New sequence:

 - Create empty temp file
 - Rename
 - Write data

This was a serious data loss bug that broke systems.

All due to:

 - An optimization
 - That didn't break any rules in the API contract

But that's no excuse to break stuff. Updates shouldn't
break user code.

Once an operating system publishes a public interface,
the real behavior of that interface *is* an API contract.

So now ext4 filesystem special cases this sequence to
prevent renames from being reordered before writes to
the same file.

## Basic Computer Organization

```

 +---------+               +-------------+
 |  CPU    |<=============>|    RAM      |
 | A B C D |               |             |
 +---------+               +-------------+
```

What the CPU does:

 * Load instruction @ %RIP from RAM
 * Execute it
 * Repeat

## C and AMD64 Assembly

 - Two programming languages
 - One mental model for execution
 - The computer executes a series of machine
   code instructoins.
 - C code translates directly to machine code.
 - Low level machine concepts:
   - Registers
   - Arithmetic instructions
   - Memory and memory addresses (a memory
     address fits in a register)
   - Labels, jumps, conditional jumps
 - AMD64 assembly has some higher level concepts:
   - Functions
   - The call stack
     - enter / leave
   - Names shared across modules
   - System calls 
 - C feels significantly higher level than assembly
   - C is executed by compilation (logically) to assembly
   - Huge win: Named local variables

## Assembly Assignments

 - Building an assemlby program is basically the same
   as building C program.
 - Starting from C-like pseudocode helps.
 - AMD64 calling conventoins allow functions to be
   built independently.
 - Translate pseudocode to assembly one function at
   a time.
   
## Processes and Memory 

 - A modern OS can run mulitple programs at the same time.
 - Each instance of a running program is a process. 
 - Older computers ran one program at a time and gave
   those programs direct access to hardware.
 - A modern OS virtualizes the hardware so that each
   program feels like it has full access to it while
   allowing the OS to enforce sharing rules.

Virtual memory:

```
 +---------------------------+
 |                           |
 +---------------------------+
 |   Stack                   | 
 +---------------------------+
 |                           |
 +---------------------------+
 |   Heap                    | 
 +---------------------------+
 |                           |
 +---------------------------+
 |   .text                   | 
 +---------------------------+
 |   .data                   | 
 +---------------------------+
```

## Process API

 - fork() - Clones the current process
 - exec() - Runs a new program in the current process,
            replacing what's there.
 - waitpid() - Wait for a process to exit.

There is a higher level API to run a program:

 - system() - Fork, exec, run a shell command, and waits.

## Files and File Descriptors

 - File descriptor table
 - open / close / read / write
 - pipe
 - dup

## The shell

We built a shell, which used all of the previous
APIs.

Conclusion: Debugging across forks is annoying.

## Virtual Memory API

One function:

 - mmap

This controls the page tables associated with
a running process, and therefore can be used to:

 - Allocate memory like malloc
 - Allocate shared memory
 - Map files into memory so they can be read/written
   with load/store instructions.
 - Map a file into memory CoW so changes in memory
   are temporary.
   
Further,

 - All memory mappings are backed by some location on
   disk.
   - Mapped files are generally backed by the file.
   - Anonymous mappings are backed by swap.
 - Executable files and libraries are loaded by mapping
   them into memory read-only.

## Processes and Threads

 - With fork + mmap, we can share memory across processes.
 - Mutable shared memory => data races => locks => deadlock
   => suffering.
 - Alternatively, threads.
 - On Linux, threads and processes are pretty similar.
   - GDB is happier with threads.
   - It's easier to kill a misbehaving mult-threaded
     program.

## Parallel Sorting

 - With multiple processor cores and multiple threads/processes
   we should be able to get a parallel speedup.
 - We explored processes, threads, mmap I/O, and regular I/O.

Conclusions:
 
 - System calls are slow.
 - The algorithm wasn't great for parallel speedups,
   a bigger sample might have been better.

## Memory Allocator

 - mmap lets us allocate memory
 - mmap works in pages, so smaller allocations are
   ineffcient
 - mmap is a system call, and those are slow 
 - munmap taks a size, so we can't just delegate
   malloc/free to mmap/munmap
 - Simple solution: Free list allocator

Challenge 2: Optimized, thread-safe allocator

Real world allocator strategies:

### Google's tcmalloc

"Thread-cache malloc"

 - A bucket allocator
 - A global array of buckets, handling fixed sizes
   up to 4MBs.
   - Buckets: 16, 24, 32, 48, ..., 1MB, 1.5MB, 2MB, 3MB, 4MB
 - Each thread has a local array of buckets handling
   fixed sizes up to 64k.
 - To coalesce memory in local caches, they added a
   garbage collection thread.

### Facebook's jemalloc

Ideal version:

 - A bucket allocator
 - Allocate a 2MB chunk for each bucket size up to
   64k
 - Uses pointer arithmetic to find metadata at the
   beginning of chunks.

## OS Kernels

 - Kernel code has direct access to manage the hardware.
   - Needs full access to the hardware.
   - Manages restrictions on user programs.
 - The OS overall provides an environment for user programs
   to run in.
 - Provides an API ot user programs: syscalls
 
## xv6

 - Based on a real historical OS: v6 UNIX
 - Simpler than production modern OSes

Homework: Added a system call.

## File Systems

 - We looked into these in some depth.
 - Most people should be familar with files and 
   directories, but the implementation gets complicated.

 - FAT
   - need to be able to track which data blocks go with
     which files or directories
   - how do directories map names to files
 - ext filesystem: separate out metadata into inodes
 - ext2: block groups
 - ext3: journaling
 - ext4: more optimizations
 - CoW filesystems are the shiny modern thing

## Stuff to Explore in the Future

Classes:

 - Networks and Distributed Systems
 - Compilers

Other stuff:

 - If something is interesting, explore it.
 - Write code!

## That's the semester! Questions?

## have a wonderful summer!










