
# First: Homework Questions?

 - Mutex: This is a lock that comes from
   the posix threads library.
   - They have a friend: Condition variables
 - Semaphore: This is an archaic synchronization
   primative from POSIX that can be used between
   processes, and can be used as a lock.

# Today: Setting Up for Memory Allocator

## Page Tables

 - Each process has its own virtual address
   space.
 - Virtual memory is managed in pages
   (usually 4k chunks).
 - That means that each process needs its
   own lookup table that will translate
   virtual page #'s into physical page #'s.

## 32-bit i386 page tables

 - How many 4k pages fit in a 32-bit address
   space?
   - How many bytes can we address with 32 bits? 
   - 2^32 = 4GB
   - 4G / 4k = 1M
 - Each entry in the page table has:
   - Virtual page #, but that's the index into the
     table, so it takes zero bits.
   - Physical page # - 20 bits
   - flags - 12 bits
     - stuff like readable, writable, executable,
       not-really-in-RAM-ask-the-kernel
   - total = 32 bits = 4 bytes per PTE    
  - 1M entries * 4 bytes/entry = 4MB
  - Problem: The origional PCs with 80386 processors
    in ~1989 shipped with 4MB of RAM.
  - This can't possibly be how it really works.
 
What are we actually trying to do?

  - We want to map (20 bit #) => (a 32 bit #)
  - Hash Table?
    - Problem: Hash tables sometimes take 
      O(n) to do stuff.
  - Requirement: Guaranteed O(1)
  - Binary Tree?
    - But O(log n)
    - We know n is 2^20, log n = 20
  - What about a tree with a larger branching
    factor?
  - Let's make a tree node be one full page.
  - 4kB / 4B = 1k

Solution: Two level page tables

  - Root node with 1k entries,
  - It points to 1k leaf nodes, each with 1k
    entries.
  - That's all 1M pages.
  - The benefit: we don't need to allocate leaf
    nodes if they're empty.
  - Minimum process size is 1 page + 1 leaf node
    + 1 root node = 3 pages = 12kB.

```
Memory address:

 [0000 0000 0000 0000 0000] [0000 0000 0000]
   page #, 20 bits       offset in page, 12 bits
 
 [0000 0000 00] [00 0000 0000] [0000 0000 0000]
  root index     leaf index    offset in page, 12 bits
```

How about on 64-bit AMD64?

 - AMD decided to use 48-bit effective addresses.
   - How much RAM can we address with 48 bits?
   - = 256 TB    (16 big hard disks)
 - 48 bit address - 12 bit page offset = 36 bits
   for page #
 - PTEs are now 64 bits = 8 bytes
 - Each level of page table still fits in a 4k page
 - 4096 / 8 = 512
 - How many bits is an index into a 512-entry 
   table? 9
 - 36 bits for page number / 9 bits per level = 4

```

[16]    [36]      [12]
unused  page #    offset within page

[16]    [9]  [9]  [9]  [9]      [12]
unused  idx1 idx2 idx3 idx4    offset within page
```

Problem: This sounds slow.


```
int* xs = // two pages of stuff
int sum = 0;
for (int ii = 0; ii < 2048; ++ii) {
    sum += xs[ii];
}
```

With naive 4-level page tables:

 -   2048 memory loads for the array accesses
 - + 4 * 2048 memory loads to check the page tables
 - = 10k

Solution: Add a cache

 - The cache is called the TLB or Translation 
   Lookaside Buffer (apparently "Page Table
   Cache" is too clear)
 - On modern systems each CPU core has one,
   and it has 1536 entries.

With 4-level page tables + TLB:

 -   2048 memory loads for the array accesses
 - + 4 * 2 memory loads to check page tables
 - = 2k
 
## Context Switches

On a single processor core:

Here's how switching between processes works:

 - We end up in Kernel code
   - An interrupt happened.
   - Or we did a system call
 - Save off the current register values to
   the stack (includes %rip).
 - Save off the %rsp to the process control
   block.
 - Switch to the new virtual address space.
   - This means setting the CR3 register.
   - All TLB entries are invalid, clear it.
 - Load the new process's %rsp from the PCB.
 - Restore the register values.
 - Return from kernel code.

Here's how switching between threads within
a process works:
 
 - We end up in Kernel code
   - An interrupt happened.
   - Or we did a system call
 - Save off the current register values to
   the stack (includes %rip).
 - Save off the %rsp to the process control
   block.
 - Load the new process's %rsp from the PCB.
 - Restore the register values.
 - Return from kernel code.

# Predicting CPU Performance

My laptop: Thinkpad X1 Yoga G3 w/
    Intel Core i7 8650U

Speed: 4GHz

How long does it take to do one clock
cycle at 4GHz? 0.25 ns

Each core can do an add in one clock cycle.

```
for (;;) { a += 5; }
```

There are 4 ALUs, each of them can do an
add in each clock cycle as long as there
aren't data dependencies.

```
for (;;) { a += 5; b += 5; c += 5; d += 5; }
```


How long does this code take to run?

```
    for (long ii = 0; ii < NN; ++ii) {
        zs[ii] = xs[ii] + ys[ii];
    }
```

Estimate 1: The adds

  - 500M * 0.25ns = 0.125 s

Estimate 2: Memory throughput

  - 4 GB @ 20 GB/s = 0.2 s

Estimate 3: Memory latency

  - 500M * 60ns = 30s

Estimate 3: Memory latency per cache line
 
  - How many 4 byte ints fit in 64B cache line?
  - 30s / 16 = 1.88s

Actual time: 4.3s, optimized: 1.88

Memory in this laptop:

 - Main Memory: 16 GB
   - Latency: 60ns = 240 clocks
   - Throughput: 20 GB/s
 - L3 Cache: 8 MB (shared between cores)
   - Latency: 11ns = 44 clocks
   - Throughput: > 200 GB/s
 - L2 Cache: 256 KB (per core)
   - Latency: 3ns = 12 clock cycles
   - Thorughput: > 400 GB/s
 - L1 Data Cache: 32 KB (per core)
   - Latency: 1ns = 4 clock cycles
   - Throughput: > 1000 GB/s
 
Caches operate in cache lines, which are 64 
bytes each. 
 
The CPU should have a prefetcher, which tries
to predict access patterns and preload data.


