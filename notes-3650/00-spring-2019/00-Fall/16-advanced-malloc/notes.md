---
layout: default
---

# Computer Systems

## Homework

 - HW07 Questions?
 
Challenge 2:

 - CH02 is up. 
 - You have a partner. Confirm that they exist ASAP.
 - Your goal: Write an optimized memory allocator.
 - Your allocator should *work* in the general case, and be very
   fast on the two provided test cases.

## Free List: Quick Review

 - In a simple free-list based memory allocator, "malloc" finds blocks of memory
   to allocate by looking on a global linked list of free chunks.
 - If the found chunk is too big, we split it and put the rest back on the
   free list.
 - If no chunk is big enough, we mmap up a new block of memory.
 - To free a block, we add the block to the free list and then coalesce adjacent
   free chunks to avoid fragmentation.
 - Maybe we special-case large allocations to skip the freelist entirely.

## Problem 1: Both malloc and free are - at best - O(n)

malloc:

 - A call to "malloc" may require traversing the entire list. 
 - Even with first fit, the first fit may be the last item on the list.

free:

 - A call to "free" may require traversing the entire list.
 - Either the list is in sorted order, or a full traversal is needed
   to handle coalescing.
 - We can delay coalesing (e.g. only do it 1/10 frees), but then 
   coalesing ends up costing O(n^2).

### Plan A: Use binary trees.

 - To speed up malloc, we could use a binary tree index on block size to find
   the best-fit free block in O(log(n)) time.
 - We can't allocate it separately, so it would need to live in the free blocks.
   It'd cost at least 16 bytes, so that'd double the minimum allocation.
 - To speed up free, we could use a binary tree index on address to find this
   block's neighbors in O(log(n)) time.
 - This requires replacing the "next" ptr with left and right ptrs, so now our
   minimum allocation size is 40 bytes.

```
struct free_chunk {
  size_t size;
  free_chunk* size_left;
  free_chunk* size_right;
  free_chunk* addr_left;
  free_chunk* addr_right;
};
```

 - This code would be pretty complicated, and a 40 byte minimum allocation size
   is probably way too much overhead for small allocations.

### Plan B: The Buddy System

 - It turns out that by adding a simple constraint, we can get the benefits of both
   trees without actually storing them anywhere.
 - The constraint: All allocated blocks of memory will have power-of-two sizes.
 - Let's take our minimum allocation to be 16 bytes, and our maximum total memory
   used to be 16 MB. That gives us 20 different sizes.

malloc:

 - On startup (or first malloc) we allocate the heap, an 16 MB chunk.
 - Instead of a free list, we have a global array of 20 free lists: one for each size.
 - When a malloc occurs:
   * We add space for a header and round our size up to the nearest power of
     two.
   * If there's a free chunk of that size, we take it.
   * If there's no chunk, we (recursively) allocate a chunk of the next size up and
     split it into two halves. We leave the extra one on the appropriate free list.
 - malloc now takes O(log(n)) (where n is the size of the heap / the maximum
   number of blocks allocated) time.

free:

 - With this scheme, free requires some trickery.
 - We could just prepend our block onto the list of blocks of that size, but
   that could lead to fragmentation.
 - We want to be able to determine if this block's "buddy" - the other half of the
   one-size-bigger chunk it was split from - is also free. If so, they should be
   coalesced and added to the free list for larger blocks.
 - Finding the buddy by scanning is easy, but takes linear time.
 - We can find the buddy by pointer arithmetic.
   - If our address is divisible by twice our size, we're the left buddy and the
     right buddy is at (addr + size).
   - If our address is not divisible by twice our size, we're the right buddy
     and the left buddy is at (addr - size).
 - Once we find our buddy, we need to be able to determine if it's free and if so
   we need to be able to remove it from the free list.
 - We want a free flag so we can see if blocks are free.
 - We want a doubly linked free list so we can remove blocks given a pointer
   to them.

The naive solution doesn't really help. The minimum allocation is no smaller than
for the binary trees plan.

```
// Header: 9 bytes
struct buddy_header_1 {
  size_t size;
  bool   free;
};

// Free block: 25 bytes (probably 32-bytes)
struct buddy_free_block_1 {
  size_t size;
  bool   free;
  void*  next;
  void*  prev;
};
```

Optimized plan:

```
// Header: 2 bytes
struct buddy_header_2 {
  uint8_t rank; // log2(size)
  bool    free;
};

// Free block: 10 bytes
struct buddy_free_block_1 {
  uint8_t  rank; // log2(size)
  bool     free;
  uint32_t next; // offset from heap base
  uint32_t prev; // offset from heap base
};
```

 - Sizes are power of two, so we can store them in one byte as the log-base-2
   of size.
 - 16 bytes for next and prev pointers is a lot.
   * Can we use 4-byte pointers instead?
   * That'll limit our heap to 4GB.
   * We'll be doing lots of allocations under 4GB, so that's still useful.
   * How to find base of heap? Allocate heap aligned to 4GB, then we can just
     mask off lower 32-bits.
   * That means we can allocate multiple heaps and our free function will still
     work.
 - Result: We can get away with a 16-byte minimum allocation.

Buddy system advantages: 

 * log2(n) malloc and free.
 * No more structure overhead than a simple free list.
 * No external fragmentation.

Buddy system disadvantages:

 * Internal fragmentation is pretty bad - in some cases an allocation will
   take up twice as much space as it needs.
 * Power of two allocations are always the worst case.
 * We might be able to beat O(log(n)) for our operations.

### Plan C: Buckets

The array-of-freelists that the buddy system uses is great for allocations. If
there's already a free item of the right size, it can be returned in constant
time.

In a "bucket" allocator, we shoot for that O(1) allocation performance by
completely separating our free lists. When an allocation request of a given size
comes in we allocate a new block of memory specifically to put on that
size-specific free list (or "bucket").

If we allowed any arbitrary size allocation, then this could result in up to
4096 free lists just for sizes up to one page. That's feasible, but it wouldn't
have great performance. Instead, a bucket allocator only handles a set list of
sizes - commonly powers of two and the half points between powers of two. So for
sizes up to a page, the list might be:

```
4, 8, 12, 16, 24, 32, 48, 64, 96, 128, 192, 256, 
384, 512, 768, 1024, 1536, 2048, 3192, 4096
```

That's 20 sizes, so allocating a page for each size might cost 80k.

Problem: Where can we put the size field to avoid having power-of-two allocations
overflow to the next bucket up?

Solution: At the beginning of the page. All the allocations are the same size,
and we can find the beginning of the page with pointer arithmetic, so no need
for multiple size fields. This means that our free list becomes a list of blocks
(pages) with at least one free slot.

Problem: How do we coalesce?

Solution: Since a 64-byte block is always a 64-byte block, we only need to worry
about the case where an entire page is free so we can unmap it. We can store
"free" bits as a bitmap at the beginning of the page. Even for 4 byte allocations
the bitmap only needs to be 128 bytes = 16 words = 2 cache lines long.

Problem: How do we handle sizes allocations >= 1 page? Rounding down the pointer
no longer gets you to the beginning of the block if there are multiple
allocations per block?

Solution: Each allocation needs its own block. Then it's fine. Maybe it's more
efficient to use a block size > 1 page.

Bucket allocator advantages:

 - O(1) malloc and free
 - Great locality of reference for fast allocation of fixed-size objects.

Bucket allocator disadvantages:

 - Stuck with some of both internal and external fragmentation.
 - Startup allocation is pretty big (each initial allocation from a bucket
   allocates at least a page)
 - No locality of reference between different sized objects.

## Problem 2: Multiple Threads

Any allocator that's based on a shared global data structure (a freelist or
array of freelists) will cause trouble with multiple threads. That global
structure is shared data, and if we protect it with a lock then we can't malloc
or free in multiple threads in parallel.

### Plan A: A Per-Thread Cache

This story makes the most sense if we start from a single global traditional
freelist.

 - each thread gets its own "cache" freelist.
 - malloc tries the local cache first, and goes to the global free list only
   when that fails
 - frees go to the local freelist
 - Occasionally, when the local free list gets too big, either the whole thing
   or a big chunk of it gets moved to the global free list and coalescing
   happens.

Advantages:

 - Lock contention on the global freelist is significantly reduced.
 - Threads get good locality of reference since they reuse the same memory blocks
   several times.

Disadvantages:

 - The global freelist lock is still a single bottleneck.
 - Fragementation is increased since we can't coalesce across threads.
 - This doesn't extend well to buddy system, since buddies may be stuck in
   different caches.
 - This doesn't extend well to buckets - we'd need to worry about data races on
   chunk metadata.

### Plan B: Multiple Arenas

The word "arena" in memory allocators is used to refer to the data structure
associated with a complete self-sufficient allocator: the free list or array of
free lists.

By having several arenas, we can allow each arena to have its own mutex. With
enough arenas (e.g. 1 per thread), this can avoid lock contention entirely
except on objects that are allocated in one thread and freed in another.

This mechanism works pretty well with buckets. The block metadata mechanism
allows freeing to work independent of which arena the block is associated with,
as long as the block metadata includes a mutex.

## Real-World Allocators

ptmalloc: The GNU libc allocator

 - Loosely ased on the traditional free-list design
 - Uses per-thread arenas
 - Tons of special case optimizations
 - Design goal seems to be to prevent the worst case from being too bad.
 
tmalloc: Google's Thread Caching Malloc

 - A bucket allocator
 - Each thread has a cache with an array of the smaller buckets
 - A garbage collector runs periodically to move items from thread caches
   back to the global buckets.

jemalloc: Facebook's Multi-Arena Allocator

 - A bucket allocator with multiple arenas
 - Arenas = 4 * CPU cores
 - Uses 2MB (rather than 4k) chunks
 - Most of the optimization is in chunk management
 - Widely considered to be the fastest current general purpose allocator, but
   the win over ptmalloc isn't huge.

## madvise

One the key tricks that makes jemalloc good is the madvise syscall with the
MADV_DONTNEED flag. This tells the kernel that for one or more pages of memory
the program wants to keep the virtual pages but doesn't need the data anymore.
Future access to these pages may allocate new physical pages of zeros.

Another neat tool is MADV_HUGEPAGE. This requests that the kernel use 2MB pages
rather than 4k pages. This means each TLB entry handles 2MB rather than 4k,
which can speed things up significantly.

There is also support for 1GB pages, but those are too big for most use cases
currently. Maybe for machines with TB of RAM...

Unfortunatly, MADV_DONTNEED doesn't work with huge pages.
