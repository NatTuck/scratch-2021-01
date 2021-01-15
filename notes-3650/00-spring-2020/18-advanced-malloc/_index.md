+++
title = "./18-advanced-malloc/notes.md"
+++

# Computer Systems

## Basics

Online Lectures:

 - If you're watching this video, you've figured out how to get the online
   lecture.
 - Each lecture will have four components:
   - Core Lecture Content
     - Example: This Video
   - Secondary Lecture Content
     - Example: The jemalloc Video
   - Shared Q&A / Discussion
   - Informal Q&A / Discussion
 - You are expected to have reviewed the core lecture content before the
   scheduled lecture time.
 - Secondary lecture content is additional material that is part of the lecture.
   You're expected to go over it, but it isn't critical to review before the
   Shared Q&A.
 - At the start of the scheduled lecture time a Q&A session will be held on
   Microsoft Teams, primarily as text chat.
   - This is a synchronous event. It will begin promptly at the start of the
     scheduled lecture block, and will end when there are no more questions
     of general interest for 5-10 minutes.
   - We'll assume that everyone participating has reviewed the core lecture
     content.

Challenge 2:

 - Challenge 2 is posted.
 - You need to request a team by Tuesday or you're working alone.
 - The assignment is to create an optimized memory allocator.
   - The base assignment is to pass the tests and be faster than your HW08
     allocator modified to be thread safe.
   - The challenge is to beat the system allocator.
 - Your allocator should *work* in the general case, and be very
   fast on the two provided test cases.
 - Test cases:
   - Collatz Conjecture
   - Linked List
   - Vector

## Allocator Slides

[Here are some slides I stole from
Christo](http://www.ccs.neu.edu/home/ntuck/courses/2017/09/cs3650/notes/15-alloc-slides/8_Free_Space_and_GC.pptx).

Cover slides up through GC.

## Strategy for CH02

 - You're starting from a simple free-list allocator.
 - You have two problems:
   - The linked list data structure and the resulting malloc and free routines
     are slow.
   - Having a single shared free list protected by a single mutex results in a
     lot of contention. This can be avoided by having more data structures and
     more mutexes (i.e. multiple arenas) or by having some thread-local data
     structures (i.e. per-thread caches).
 - You want to do the simplest thing that could possibly work, then benchmark
   it.
 - If that's not fast enough, you want to do the simplest optimization that
   could possible work then benchmark that.
 - One tool to consider learning and trying here is gprof. It's a profiler that
   will tell you exactly where your program is spending its time.

## Old Content

### Advanced Buckets

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

Here's the jemalloc video: https://www.youtube.com/watch?v=RcWp5vwGlYU

 - Keep in mind that this is a video about further optimizing an already
   optimized allocator.
 - Keep an eye out for the design elements of jemalloc that the speaker *isn't*
   excited about in this talk - some of those (like shared chunk metadata) are
   the secret sauce that makes jemalloc fast.

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


## Overflow



