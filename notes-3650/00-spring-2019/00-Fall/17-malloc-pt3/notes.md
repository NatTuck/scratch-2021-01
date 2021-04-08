---
layout: default
---

# Computer Systems

## Homework

 - Challenge 2 questions?

## Real-World Allocators

ptmalloc: The GNU libc allocator

 - Loosely ased on the traditional free-list design
 - Uses per-thread arenas
 - Tons of special case optimizations
 - Design goal seems to be to prevent the worst case from being too bad.
 
tcmalloc: Google's Thread Caching Malloc

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

## Slides, part 2

[Christo's slides, again](http://www.ccs.neu.edu/home/ntuck/courses/2017/09/cs3650/notes/15-alloc-slides/8_Free_Space_and_GC.pptx)


