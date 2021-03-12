
## Page Tables

 - Every process gets its own virtual
   address space.
   - Pointers store virtual addresses.
   - The CPU needs to translate those
     virtual addresses into physical
     addresses to actually send a real
     (physical) address down the memory
     bus to do the memory operation.
 - Virtual memory is managed in pages,
   usually 4k chunks.
 - So each process need its own lookup
   table to translate virtual page numbers
   into physical page numbers.

32-bit page tables (intel i386)

 - Our virtual/physical address space size
   is 2^32 = 4GB.
 - How many 4k pages fit in that address space?
   - 4GB / 4K = 1M
 - How big is each entry in the page table (PTE)?
   - Virtual page # is the index into the table,
     so that takes no space.
   - Physical page # - 20 bits
   - Flags - 12 bits of them
   - Total = 32 bits = 4 bytes per PTE
 - How big is a page table?
   - 1M entries
   - each entry is 4 bytes
   - total = 4MB
 - Problem:
   - Page table has to be in memory
   - First Intel 80386 PCs came with 4MB of RAM
   
```
Memory address looks like:
 
 [0000 0000 0000 0000 0000] [0000 0000 0000]
      page #                  offset within a page 
```

Design goal:

 - Implement a map (20 bit integer => 32 bit value)
 - Need guaranteed constant time operations 

Solution: Multi-level page tables.

 - It's a tree.
 - Tree nodes are 1 page.
 - 4096 / 4 = 1024 entries per tree node
 - Tree root has 1024 entries, each pointing to a
   tree leaf with 1024 entries.
 - Leaves with no entries in them don't need to
   be allocated at all.
 - To allocate 1 page, we need the root and we need
   one leaf, so our minimum size for a process
   is now 12kB or 3 pages.
 
 ```
Memory address looks like:
 
 [0000 0000 0000 0000 0000] [0000 0000 0000]
      page #                  offset within a page 
 
 [0000 0000 00] [00 0000 0000] [0000 0000 0000]
   root index    leaf table    offset
```

What about 64-bit systems?

 - AMD64 actually only uses 48 bits by default.
   - How many bytes can you address with a
     48 bit memory address? 256 TB
   - That's a reasonable limit for desktops
     or laptops, even today.
 - Of those 48 bits, 12 are still the offset.
 - We now have 36 bit page numbers.
 - PTEs are now 64 bits
 - Two level page tables would give us
   500MB for each level of table.
 - How many 64-bit PTEs can we fit in one
   4kB page? 512; which means 9-bit indexes
 - How many levels of tables do we need
   if we're splitting a 36 bit page #
   into 9 bit chunks? 4
   
   
 ```
Memory address looks like:
 
 [16]       [36]      [12]
 unused     page #    offset within a page 
 
 [16]   [9]  [9]  [9]  [9]   [12]
 unused idx1 idx2 idx3 idx4  offset within a page 
```

This isn't good enough for Intel right now.

 - They wanted to ship their new Optane chips.
   - Like SSDs (non-volatile)
   - But more RAM-ish than FLASH
 - One format they ship them in is DIMMs, which
   go in RAM slots.
 - You can fit enough Optane in a big enough server
   to go over 256 TB.
 - Solution: Add another level of page tables,
   5 instead of 4.
 - How big does that make the effective memory
   address? 57 bits
 - How much memory can that address? 128 PB

```
int* xs = // two pages of stuff
int sum = 0;
for (int ii = 0; ii < 2048; ++ii) {
    sum += xs[ii];
}
```

Cost with naive 4-level page tables:

 -   2048 memory reads for the array
 - + 2048 * 4 reads for the page tables
 - = 10k

Solution: Add a cache.

 - This is called the TLB (Translation 
   Lookaside Buffer) because Page Table
   Cache was too complicated.
 - Modern systems mostly have TLBs with
   1536 entries per core.
   
Cost with 4-level page tables + TLB

 -   2048 memory reads for the array
 - + 2 * 4 memory reads to fill the TLB
 - = 2k

On other archetectures, translation can work
differently.

For example, on old SPARC processors the lookup
was handled entirely in software, so the kernel
could have used hash tables if it wanted to.

## Context Switches

Consider a single core processor running two
processes (or two threads).

When the running process is out of time for 
its turn, the OS is going to switch to the
other process. Similarly, if the running 
process does a blocking system call.

Mechanism to switch processes:

 - Stop executing program, switch to kernel
   code.
 - Save off the registers to the stack.
   - Including %rip
 - Save the stack pointer to the process record.
 - Switch page tables by setting the CR3 register.
   - Clear TLB
 - Load the stack pointer into %rsp
 - Load the saved registers from the stack
 - Continue executing at %rip

Complication: Switching page tables invalidates
the TLB.

Mechanism to switch threads in same process:

 - Stop executing program, switch to kernel
   code.
 - Save off the registers to the stack.
   - Including %rip
 - Save the stack pointer to the process record.
 - Load the stack pointer into %rsp
 - Load the saved registers from the stack
 - Continue executing at %rip

## Predicting CPU Performance

My laptop: Thinkpad X1 Yoga G3 w/ Intel Core i7-8650U

Single-thread Clock Speed: 4GHz

How long does a single clock cycle
take at 4GHz? 0.25 ns

Each core can do an add in one clock cycle.

```
for (;;) { a += 5; }
```

There are four ALUs, which can *each* do an add
per clock cycle if there's no data dependencies.

```
for (;;) { a += 5; b += 5; c += 5; d += 5; }
```

How long will this take?

```
    for (long ii = 0; ii < NN; ++ii) {
        zs[ii] = xs[ii] + ys[ii];
    }
```

First estimate: 
 - Adds
 - 500M * 0.25ns = 0.125s

Second estimate: 
 - Memory thoughput
 - 4GB @ 20 GB/s = 0.2s

Third estimate:
 - Memory latency per int
 - 2G * 60ns = 120s
 
Fourth estimate:
 - Memory latency per cache line
 - How many 4 byte ints per 64 byte cache line? 16
 - 120s / 16 = 7.5s

Actual: 4.4s
Optimized: 2.5s

Complication: Memory loads

This laptop's memory:

 - Main memory: 16GB
   - Latency: 60ns = 240 clocks
   - Throughput: 20 GB/s
 - L3 Cache: 8MB (shared between cores)
   - Latency: 11ns = 44 cycles
   - Thoughput: > 200 GB/s
 - L2 Cache: 256 KB (per core)
   - Latency: 3ns = 12 clock cycles
   - Throughput: > 400 GB/s
 - L1 Data Cache: 32kB (per core)
   - Latency: 1ns = 4 cycles
   - Thoughput: > 1000 GB/s

Caches work in 64-byte cache lines.

The CPU and cache has a built in preloader, which
tries to detect memory access patterns.

