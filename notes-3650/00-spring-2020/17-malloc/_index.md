+++
title = "./17-malloc/notes.md"
+++

# Computer Systems

## Last Homework

 - How many people got better speedups for threaded vs. processes?
 - Moral: Syscalls are slow.

## This Homework

 - We'll talk about memory allocators today.
 - Questions before we start?

## Memory Allocator

 - A new process starts with no heap space allocated.
 - To get heap space, we need to make a system call.
   - Traditionally, the call was sbrk(2)
   - sbrk still exists in POSIX, but nobody uses it.
   - We allocate memory with mmap.
 - To allocate private memory with mmap, we use MAP\_PRIVATE | MAP\_ANONYMOUS
 - The kernel can allocate in 4K pages. Requesting smaller allocations
   is meaningless - if you ask for 1 byte with mmap, you get 4K.

So a memory allocator wants to use mmap to get memory from the OS and fix a
couple of sharp edges in the process:

 - We want to handle allocations under 1 page.
 - We want to do many small allocations efficiently.
 - We want to be able to free memory without remembering the size of the allocation.
 - Especially for small allocations, we want to reuse memory rather than returning it
   to the OS and then asking for it back.

The traditional solution to this problem is to maintain a "Free List".

 - This is a list of free blocks with, logically, location and size for each.

To allocate a chunk of memory, we do the following:

 - See if there's a block of memory on the free list big enough to use.
 - If not, allocate a new block with mmap.
 - If the block is too big, split it and return the extra to the free list.
 - Remember the size of the allocated block for later.
 - Return the selected block to the user.

To free a chunk of memory:

 - Stick it on the free list.

## Problems and Solutions

This leaves some problems:

Problem 1: Where do we store the size of a chunk?

 - We're the memory allocator. We can just allocate a bigger chunk and
   store the size *in* the chunk.
 - We want to put it at the beginning of the chunk so we can find it again.

So when someone requests a chunk of size B, we allocate B + 8 bytes
layed out as follows:

 - Size (size_t = 8 bytes)
 - That many bytes of memory for the user.

We return a pointer to the memory after the size.

When the memory is freed, we can find the size by subtracting 8 bytes
from the pointer we got.


Problem 2: Where do we store the free list?

 - The free list is made up of chunks of free memory.
 - We can store the list in the memory itself.
 - Easy to lay out a singly or doubly linked list in the
   memory.
 - This makes our minimum actual memory allocation be the
   size of a list cell.

Problem 3: Fragmentation

```
   for (1..200) {
     xs[ii] = malloc(800);
   }
   
   for (1..10) {
     free(xs[ii]);
   }
   
   y = malloc(5000);
```

 - We'd like to reuse the memory rather than requesting more from the OS.
 - When we free memory, we want to check the free list to see if we can
   combine the chunk we're freeing with other chunks already on the list.
 - We may need to combine more than once (A,C on free list, free B).

Problem 4: Big Allocations

 - What if a program requests 10 GB of RAM and then frees it?
 - We'd like to return that to the OS, not put it on the free list.
 - Solution: Send large allocations directly to mmap, and then do a
   munmap when that memory is freed.
 - For allocations over some threshold size, the cost of the syscall
   is going to be irrelevent.
 - In this case we always allocate some number of whole pages.

## Homework 08

For the next homework, you'll get to write a free-list based memory
allocator: The Husky Allocator.

You will need to implement "malloc" and "free" functions.

## Design Considerations

Design tradeoffs:

 - Singly or doubly linked list?
 - Ordered or unordered list? (both are O(n) when optimized)
 - Merge adjacent list items every time or only occasionally?
 - How big is the "big allocation" cutoff?
   - At least 4k
   - Probably smaller than 1GB.
 - Should we ever return memory from the free list to the OS?

Other ideas:

 - Should we use a balanced binary tree instead of a linked list?
 - Can we be more efficient by restricting allocation sizes to
   specific values rather than allowing any size?
    - Benefit?
      - Buddy System
      - Single-size free lists.
    - Cost?
 - Single-size free lists end up being the common solution in
   real world allocators. We'll see more on that next week.

Complications:

 - We know how memory allocators and their data layout might work.
 - What happens if we use a pointer after freeing it?
 - What happens if we underflow our allocation?
 - What happens if we overflow our allocation?
 - What happens if we double free?

## Alocator Slides

 - [Here are some slides I stole from Christo](http://www.ccs.neu.edu/home/ntuck/courses/2017/09/cs3650/notes/15-alloc-slides/8_Free_Space_and_GC.pptx)
