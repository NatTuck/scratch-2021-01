
# Systems

# HW Questions?

# Today: Page Tables

 - Every process gets its own virtual address space.
 - Memory is managed in pages (usually 4k blocks)
 - That means that every process has a lookup table,
   the page table, which maps virtual pages to
   physical pages.

Starting with 32 bit memory addresses.

```
 [0000 0000 0000 0000 0000] [0000 0000 0000] 
        page #              offset within page
```

An address can be split:

 - 20 bits for page #
 - 12 bits for page offset

How many pages are there in an Intel 32-bit
virtual address space (with 4k pages)?

 - 2^20 = 1M = 1024*1024 pages = 2^20 entries
 - How big is a page table entry?
   - Virtual page # is the index, so 0 bytes.
   - Physical page #   (20 bits = 3 bytes)
   - We need some flags, so figure 64 bits = 4 bytes
 - How big is a page table?
   - 4MB for each process
 - The orgional 80306 processors from intel shipped
   in machines with 4MB of RAM.
  
Multi-level page tables   
   
 - Almost all of the pages in the address space 
   are unused.
 - Can we use a hash table?
   - No, worst case O(n) is not acceptable for
     memory accesses.
 - We can use a fixed-depth tree, or nested page
   table.
 - We have a top level table, which fits in a
   single page. So we can fit 1024 entries
 - Each entry in the top level table points to
   a second-level table - also with 1024 entries
   - which finally gives the physical address.
 
 ```
 [0000 0000 00]   [00 0000 0000] 
 top-level index  second-level index
 
 [0000 0000 0000 0000 0000] [0000 0000 0000] 
        page #              offset within page
```

Result: Mapping N pages takes (N/1024)+1 pages 
of page table.

So mapping 4MB of RAM takes:

 - 1024 pages
 - optimized, that's one second level table
 - + 1 top level table

 - This means minimum size for page tables
   per process is 8kB.
 - How many processes can the 386 have running
   before filling its 4MB of RAM with page tables?
   - Only 500. 

More complicated for 64 bits:

With 64-bit addresses, we can't quite do a two-level
multi-level page table.

 - We still have 4k pages.
 - 64 - 12 = 52, so our page #'s are 52 bits.
 - Wait... AMD64 only actually uses 48 bits of the address.
   - So we care about 48 - 12 = 36
   - Actual page # sizes are 36 bits
 - Now we have bigger PTEs, because 36 > 20.
   - Page table entries really are 64-bits = 8B now.
   - How many PTEs fit in a 4k page?
   - 2^9 = 512
 - Does 36 evenly divide by 9?
   - 36/9 = 4
 - So now we have 4 level page tables.
   - Each page table is 4k with 512 entries.
   - Doing 4 lookups lets us translate a 36 bit
     virtual page # into a physical page #.

The 64-bit address is split up into:

 - Top level index (9b)
 - 2nd level index (9b)
 - 3nd level index (9b)
 - 4nd level index (9b)
   - = 36
 - + 12 page offset
   - = 48

Is 48 bits really enough?

 - What's the largest amount of RAM we can
   address with a 48-bit address?
   - 256 terabytes

How big would effective memory addresses
be with 5 level page tables?

 - 57 bits
 - max 128 petabytes

Problems with page tables:

 - We want instructions like "mov (foo), %bar"
   to use virtual addresses.
 - For every memory access, we need to do a
   translation from virtual => physical addresses.
 - With 4 level page tables, that means every
   memory access actually takes 5 accesses.

Solution:

 - Add a cache.
 - Pretty frequently an access to some page is
   followed quickly by an access to the same page.
 - This cache is in hardware, it's called the
   Translation Lookaside Buffer (TLB) because
   Page Translation Cache would have made too
   much sense. It normally has 1536 entries.

```
int* xs = // two pages of stuff;
int sum = 0;
for (int ii = 0; ii < 2048; ++ii) {
    sum += xs[ii];
}
```

 - Without TLB that's 2048 accesses * 5  = 10k
 - With a TLB that's 2048 accesses + 2*4 = 2k

# Context Switches

 - Different processes have different page
   tables.
 - Switching to another process will invalidate
   all prevoius translations - in a simple implementation,
   we need to flush the TLB.
 - This is a reason why threads may be slightly
   faster than process.
   - Threads don't need a heavy context switch,
     and therefore can keep the TLB.

Sequence to switch threads (on a single CPU core):

 - Save the registers to the stack.
   - Including %rip
 - Point the stack pointer at the other thread's
   stack.
 - Load registers from stack.
 - Continue.

Sequence to switch processes (on a single CPU core):

 - Save the registers to the stack.
   - Including %rip
 - Point the page table register at the new
   processes's root page table and flush TLB.
 - Point the stack pointer at the other thread's
   stack.
 - Load registers from stack.
 - Continue.

# Predicting CPU performance

My laptop: Thinkpad X1 Yoga G3
  w/ Intel Core i7-8650U

```
CPU MHz:             2400.090
CPU max MHz:         4200.0000
CPU min MHz:         400.0000
```

We're going to assume that one core runs at 4GHz.

If the processor runs at 4GHz, how long in time
does it take for one clock cycle? 0.25 nanosecond

Each core on this CPU can do an add in one cycle
so code this should run at about 4 billion
adds per second:

```
for (;;) { a += 5; }
```

There are actually 4 ALUs, and each of them
can do an add per cycle if there are no
data depenencies:

```
for (;;) { a += 5; b += 5; c += 5; d += 5; }
```

We still run at 4 billion loop iterations
per second.

```
    for (long ii = 0; ii < NN; ++ii) {
        zs[ii] = xs[ii] + ys[ii];
    }
```

How long does this take?

First approximation:

 - 500m adds * 0.25ns each = 0.125s

Second approximation:

 - Memory throughput limits to 0.2 seconds

Memory in this laptop:

 - Main memory: 16GB
   - Latency: 60ns = 240 clocks
   - Throughput: 20 GB/s
 - L3 cache: 8 MB (shared)
   - Latency: 11ns = 44 cycles
   - Throughput: > 200 GB/s
 - L2 cache: 256KB (per core)
   - Latency: 3ns = 12 clocks
   - Throughput: > 400 GB/s
 - L1d cache: 32KB (per core)
   - Latency: 1ns = 4 cycles
   - Throughput: > 1000 GB/s

 - 500M * 60ns = 30 seconds

One more consideration: Cache works in cache
lines of 64 bytes. So we can have 16 ints
per cache line.

 - So our new estimate is 2s
 - Or 4s if I'm wrong about xs and ys simultaneously.


```
A map

00 = A
01 = B
10 = C
11 = D

First level

 0 => [0 => A, 1 => B]
 1 => [0 => C, 1 => D]

Sparse table:

10 = C
11 = D

 0 => ??
 1 => [0 => C, 1 => D]

```




 
 
 




