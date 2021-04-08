---
layout: default
---

## Computer Systems: Last Lecture

## Challenge Questions?

 - Any?
 - Do your TRACE!
 - Come to my office hours with any grade issues ASAP. Can't fix stuff after
   the grade deadline.

## Semester Review

### Computer Systems

What was "computer systems"?

 - We explored some of the interface between a computer program and the computer
   system it runs on.
 - Specifically, we looked a common class of computers with a concrete operating
   system and instruction set archetecture: Linux on AMD64.
 - "If you're writing a program for a computer running Linux on AMD64, what
   can your program do? What enviornment does it run in?"
 - We didn't cover everything. Instead, we looked at specific examples in some
   depth.

Two broad things to take away from the course:

 - There is no magic in any of this, just a bunch of moderately complicated C
   code. At least on open source sytems if you need to know the details, you can
   go look and expect to understand what's going on with some effort.
 - Systems can involve Archaeology. Design decisions are hard to change once
   people are using the resulting interfaces. Things rarely go away once adopted,
   more frequently new development creates alternatives or extra layers.

Things get complicated:

 - Historically, ext style file systems lost data on crash.
 - To avoid this when writing important files (e.g. system config files), a
   common pattern was to write data to a new temp file and rename over the old
   one.
 - This was, in practice, a safe operation. The write happened first, and the rename
   was atomic.
 - Problem: ext4 has an optimization called delayed block allocation. Data blocks
   aren't written until you had a bunch of them. Unfortunately, that can reorder
   the rename to before the data writes.

Old sequence:

 - Create empty temp file.
 - Write data.
 - Rename over config file.
 
New sequence:

 - Create empty temp file.
 - Rename over config file.
 - Write data.

Needless to say, this was a serious data loss bug that broke systems. Now ext4
special cases renames that overwrite a file to not let writes be reordered past
them. But there was never a specification that guaranteed this behavior, just
people writing code to target the actual system.

### Basic Comp Org

 - **Draw the CPU-FSB-RAM-I/O diagram**
 - mmaped devices and interrupts

### C and AMD64 Assembly

 - Two new programming languages.
 - One new mental model for program execution.
 - The computer executes machine code: a sequence of instructions.
 - Assembly lists that sequence of instructions
 - Low level machine concepts:
   - Registers
   - Arithmetic instructions
   - Memory and memory addresses (an address fits in a register)
   - Labels, jumps, and conditional jumps
 - AMD64 assembly includes some higher-level concepts:
   - Functions
   - The call stack
   - Names shared across modules
   - System calls
 - C feels significantly higher level than assembly
   - But C is executed by compiling to assembly
   - The translation process is largely straightforward
   - The big usability win is named local variables

### Assembly Assignments

 - Since C and Assembly are functionally the same, building an assembly
   program is basically the same as building a C program.
 - In fact, starting from C-like pseudocode is a good idea.
 - The AMD64 calling conventions are designed to allow functions to be
   written independently.
 - So C-like pseudocode can be translated to ASM one function at a time
   and the result should work.
 - Next semester I think I'm going to add another assembly assignment.

### Processes and Memory

 - A modern OS can run multiple programs at the same time.
 - Each instance of a running program is a process.
 - Older computers ran one program at a time - it had direct access to the
   hardware, including I/O devices and memory.
 - A modern OS virualizes the hardware so that each program feels like it has
   the whole computer to itself.
 - Memory is virtualized directly - the OS and hardware conspire to lie to the
   running program about memory addresses.
 - I/O devices are accessed through system calls.
 - **Draw the physical and virtual address space diagrams.**

### Process API

 - fork() - copy a process, including all it's memory (except CoW)
 - exec() - load a new program into the current process
 - waitpid - wait for a running process

### Files and File Descriptors

 - File descriptor table
 - open
 - pipe
 - dup

### The Shell

We explored the process and file descriptor APIs by building a shell.

Conclusion: Debugging across fork is annoying.

### Virtual Memory API

One function:

 - mmap

Does everything:

 - Allocate normal memory (private, not backed by a specific file)
 - Allocate shared memory (not backed by a file, but stays shared across fork)
 - Map a file into virtual memory (loads and stores become reads and writes)
 - Map a CoW copy of a file (backed by a file until you write, then becomes normal
   private memory)

Further,

 - All memory mappings are backed by something. If not a file, then by swap.
 - Executable files and libraries are loaded into memory by mmapping sections of
   the file, this lets the same physical memory for them be shared between
   multiple processes.

### Processes and Threads

 - With fork and mmap we can have shared memory across processes.
 - Mutable shared memory => data races => locks => deadlocks => suffering.
 - Alternatively, threads.
 - On Linux, threads and processes are pretty similar. GDB is happier with
   threads, and it's easier to kill a misbehaving multi-threaded program than
   a bunch of forked processes.

### Parallel Sorting

 - With multiple processes / threads on a multi-core system it'd be nice to get
   a parallel speedup.
 - We explored this with processes, threads, mmap I/O, and regular file I/O.

### Memory Allocator

 - mmap lets you allocate memory, but having to do a system call for each allocation
   would be slow.
 - Only being able to allocate full pages makes direct calls to mmap even less
   suitable for small allocations.
 - So systems provide malloc and free functions implemented on top of mmap.

Allocator assignment #1:

 - Simple allocators can be pretty straighforward using free lists.
 - The idea of keeping the list structure for free blocks in the same memory
   area where user data will go once the memory is allocated is kind of a trick.
 - Coalescing requires some work, and is what makes building an allocator that's
   both correct and fast somewhat challenging.
 - **draw a free list**

Allocator challenge:

 - Optimizing an allocator for high performance in a program with multiple
   threads can get pretty tricky.
 - Need locks to avoid data races.
 - Need to minimize locks because they're slow.
 - Data can be shared across threads, but we still want to be able to coalesce.
 - **draw ideal tcmalloc: blocks up to 4mb globally, up to 64k in local caches**

### OS Kernels

 - A modern operating system manages the hardware and provides an environment
   for user programs to run in.
 - The code that runs when the machine first boots and actually talks directly
   to the hardware is called the kernel
 - Kernel code runs with full access to everything - the hardware enforces
   access restrictions on all other running code
 - The kernel provides its services to user programs through system calls

### xv6

 - To explore kernels, we looked at a complete operating system with some
   historical relevency: xv6
 - xv6 boots on 32-bit x86 systems (which aren't quite the same as amd64)
 - For a homework assignment, you had to trace the process of making a
   system call through the various source files

### File Systems

 - Our last topic of the semester was a reasonably deep dive into file systems.
 - The concept of files and directories on a computer should be pretty familiar,
   but the details of implementing them get pretty complicated.

First Example: FAT

 - A file is a chain of data clusters, allocated anywhere on disk.
 - Clusters are allocated to files through the file allocation table, an array
   of one integer per cluster at the beginning of the disk.
 - A file's data is referenced by the index of the first cluster. Subsequent
   clusters are arranged in a linked list formed by the FAT.
 - Directories store all file metadata and a reference to the first cluster.
 - Simple, but gets fragmented and kills spinning-disk performance.

Second Example: ext-family

 - Each file (filesystem object) has an inode, which contains its metadata,
   including the list of data blocks.
 - Compared to FAT avoids linked-list traversal to find block list.
 - Compared to FAT allows for hard links.
 - ext2 adds: block groups, which improve locality and reduce seeks
 - ext3 adds: a journal, finally preventing corruption on crash
 - ext4 adds: efficiency improvements: extents, btree directories, etc 

## Transactional File System

This was an addtional part of the filesystem assignment last semester.

Motivation:

 - Filesystems store shared mutable data, so there's a risk of data races
   if multiple programs access the same file.
 - Writes to files aren't atomic, so if a crash occurs between writes a file
   might end up in a corrupted state.
 - Transactions would solve both problems.

Idea:

 - Add per-file transactions.
 - Opening a file starts a transaction and snapshots the current state of the
   file for this file handle in memory.
 - Closing or syncing the file commits the transaction.
 - If the file was modified by another process during that time, the commit
   fails and the file is reloaded from disk.
 - There's a new operation: rollback.
 - This violates the POSIX rules: fine.

Two complications: 

 - Our file access is now stateful. We need to do stuff with the "open" callback
   and use the fuse\_file\_info struct.
 - We need to provide a new operation for users. We can do this using the ioctrl
   callback. An ioctrl does something non-standard described by an integer. We're
   going to hijack the "rewind this tape drive" code for file rollback.

Tour:

 - Start in the tests.
 - Show open, ioctl, and the use of a fd in e.g. read/write.

## Have a good winter break

