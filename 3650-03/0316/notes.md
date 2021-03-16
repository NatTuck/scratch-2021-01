
# Homework: Parallel Sort with Threads

# Today: Building a Memory Allocator

```
   +------------------------
   |  stack (grows down)
   +------------------------
   |
   +------------------------
   |  heap (allocated with mmap)
   +------------------------
   |  heap (allocated with mmap)
   +------------------------
   |
   +------------------------
   |  heap (allocated with mmap)
   +------------------------
   |  .text
   +------------------------
   |  .data
   +------------------------
```

Why can't we just delegate malloc/free
to mmap/munamp.

```
void*
malloc(size_t sz)
{
   // Problem 2: Minimum mmap size
   // is 1 page = 4k.
   return mmap(..., sz, MAP_PRIVATE|MAP_ANONYMOUS, ...);
}

void
free(void* addr)
{
    // Problem 1: What's the size?
    munmap(addr, length?);
}
```

We need our allocator implementation to:

 * Remember the size of each allocation
   so that free works.
 * Pack multiple small allocations into
   a single page.

We could just store a global hashmap
of (addr => size). 

Problem: We'd need to allocate memory
for the hash map - and we don't have
an allocator yet.

Better plan: We're the allocator, we can
just make the allocations slightly bigger
to have space for the size.

 * If user requests 100 bytes.
 * We actually allocate 108 bytes.
 * So we we have space for a size_t

```
User requested 100 bytes

We allocate a 108 byte block

       +----------------
       | 100 bytes
ptr -> +----------------
       | 8 bytes
       +----------------
```

## Free List

A free list tracks a list of blocks of memory
which we have requested from the OS (with mmap)
but which have not been allocated to the user.

These blocks can be split up to handle small
allocations, they can be reused, etc.

```
struct free_block {
    void*  addr;
    size_t size;
};
```

So the free list is just a vector of free_blocks?

Nope, that would require a memory allocator.

How about a linked list?

Nope, that would require a memory allocator.

Wait... we're storing blocks of unused memory.

 * If we're storing a block of unused memory
 * We can use it to store stuff.
 * A linked list cell is stuff.

```
// addr is the pointer to the struct
struct free_block {
    size_t size;
    free_block* next;
};
```

```
struct block_header {
    size_t size;
}
```

Side effect: Minimum block size we can
deal with is sizeof(free_block). We can't
allocate smaller stuff, because we couldn't
put it on the free list when it got freed.

## Big Blocks

The free list is an optimization for small
blocks, we need it to handle allocaoitns < 4k.

It may be an optimization for medium blocks,
to save syscalls.

For large blocks it's a waste of time - we should
just delegate to mmap / munmap.

For HW08, we'll use 4k as the threshold. Smaller
than that we look on the free list, bigger we
mmap.

## Pseudocode

```
malloc(size):
  size += 8 // for size field
  if size < 4k:
    ptr = find a block on the free list
    (if no block, mmap a new page)
    *ptr = size
    return ptr + (8 bytes)
  else:
    ptr = mmap
    *ptr = size
    return ptr + (8 bytes)

free(addr):
    size = *(addr - (8 bytes))
    if size < 4k:
       add to free list
       make sure we coalesce by merging
         adjacent blocks
    else:
       munmap
```

Example:

```
for (ii in 0..3):
    xs[ii] = malloc(1800)

for (ii in 0..1):
    free(xs[ii])

yy = malloc(3000)
```

Memory blocks written as: (page+offset, size)

```
free_list = [(p1+3616, 480), (p0+3616, 480)]

xs[0] = (p0+0, 1808)
xs[1] = (p0+1808, 1808)
xs[2] = (p1+0, 1808)
xs[3] = (p1+1808, 1808)
```

xs[2]

 - want a block of size 1808

xs[1]

 - 2288 - 1808 = 480
 - 1808 + 1808 = 3616

xs[0]

 - 1800 + 8 = 1808
 - 4096 - 1808 = 2288

```
free_list = [
  (p0+1808, 1808), (p0+0, 1808), 
  (p1+3616, 480), (p0+3616, 480)
]

xs[0] = invalid
xs[1] = invalid
xs[2] = (p1+0, 1808)
xs[3] = (p1+1808, 1808)
```

free(xs[1])
free(xs[0])

```
```

```
// This is bad, our heap is
// fragmented
      +------------------------
      |  480 unused
      +------------------------
      |  1808 unused
      +------------------------
      |  1808 unused
p0 -> +------------------------
```

```
// This is better
      +------------------------
      |  4096 unused
p0 -> +------------------------
```


```
free_list = [
  (p0+3008, 1088),
  (p1+3616, 480),
]

xs[0] = invalid
xs[1] = invalid
xs[2] = (p1+0, 1808)
xs[3] = (p1+1808, 1808)
yy    = (p0+0, 3008)
```

yy

 - 3000 + 8 = 3008

## Design Tradeoffs

Singly or doubly linked list?

 - Single list is smaller (thus smaller minimum alloc).
 - Single list is easier to implement.
 - Doubly linked list allows O(1) removal of elements
   given the element pointer.
 - For HW08: Singly linked list strongly recommended.

Should we keep the list sorted?

 - If we keep it sorted by size of block.
   - We can always find the best fitting block.
   - We can always stop searching without wasting
     time looking at bigger (or smaller) blocks.
   - With this optimization, malloc is O(n).
   - Without it, malloc is O(n).
 - If we keep it sorted by memory address.
   - Coalscing is easy, we can do it while inserting
     in a single pass.
   - With this optimization, free is O(n).
   - With the worst naive implmentation, free
     is O(n^2).
   - With a reasonable implementation that always
     coalesces, free is O(n).
   - Sorted by memory address is simplest to implment,
     and is recommended for HW08.

How big is the big allocation cutoff?

  - Smallest possible is 4k.
  - Required for HW08 is 4k.
  - In practice, >= 1M is probably better.
  - You'd need to benchmark.

When do we return memory to the OS?

  - Advantage: Other programs can use the memory.
  - Disadvantage: We'll probably use more memory
    later, and no need to waste syscalls.
  - For HW08, we unmap big allocations and
    never unmap small allocations.

## How to optimize further. Can we beat O(n)?

Observation: We're using a linked list. Those
tend to have O(n) operations. If we used a 
different data structure, we might get different
asymptotic complexity.

Hash Table

 - Sometimes O(n)
 - Only good for equality comparisons.
   - If we ask for 100, we don't find 103.

Balanced Binary Tree

```
// Addr of struct = addr of block
struct free_node {
    size_t size;
    free_node* left;
    free_node* right;
}
// Minimum alloc size = 24
```

 - Order by size
   - malloc is O(log n)
   - free is O(n)
 - Order by memory address
   - malloc is O(n)
   - free is O(log n)

```
// Addr of struct = addr of block
struct free_node {
    size_t size;
    free_node* size_left;
    free_node* size_right;
    free_node* addr_left;
    free_node* addr_right;
}
// Minimum alloc size = 40
```

 - Both!
   - malloc is O(log n)
   - free is O(log n)
 - n < 2^40, so log n < 40, so O(log n) ~ O(1)

## Buddy System

 - Pre-allocated heap (1M)
 - Array of free lists (21 slots)
   - Each slot only holds free blocks of
     size = 2^ii
 - Restriction: We only track free blocks that
   are power-of-two sizes, so all allocations
   are rounded up to a power of two (after adding
   8 bytes for the header)


```
  frees[20] = []  // 2^20 size blocks
  frees[19] = [+2^19]    // 2^19 size blocks
  frees[18] = [+2^18]    // 2^18 size blocks
  frees[17] = []    // 2^17 size blocks
```

malloc(151151) => (+0, 2^18)

 - 151151 + 8 = 151159
 - round up to a power of two = 262144 = 2^18

free(malloc(151151) => (+0, 2^18))

 - We can find our buddy by XORing our address
   with our size.
 - Our buddy is the other half of the block we
   were split from.
 - If that block is free, we merge.
 - Else, we just insert this block on the 
   appropriate free list.

```
struct buddy_header {
    uint8_t  size;    // bytes = 2^size;
    uint8_t  used;    // boolean
    uint16_t heap;    // which heap?
}

struct buddy_node {
    uint8_t  size;    // bytes = 2^size;
    uint8_t  used;    // boolean
    uint16_t heap;    // which heap?
    uint32_t next;    // offset from start of heap
    uint32_t prev;
}
// minimum allocation = 16 bytes
```

 * Less efficient because we're rounding
   up allocaitons to powers of two.

The buddy system is neat, but it's not nessisarily
what I recommend for the challenge.

Another popular strategy is what I call "buckets",
which we'll cover in a future class.






