
# Homework questions?

 - The free list tracks free blocks of memory.
 - If you give the user a block of memory,
   that block is no longer free but the
   remaining portion is if split.

# Challenge 2

 - Team assignment
 - You must request a team by next Tuesday
 - Extend the HW08 allocator:
   - Make it thread safe
   - Make it fast on the test cases
 - Your allocator should *work* the the general
   case even as it's optimized for the specific
   test cases.
 - Test cases:
   - Collatz Conjecture
   - Linked lists
   - Vectors

# Allocator Slides

 - Done

# Bucket Allocator

In a bucket allocator, we have an array of
free lists called buckets.

 - Each bucket only tracks blocks of some
   fixed size.
 - Which sizes is up to you, but you've got a
   fragmentatoin vs. too many sizes tradeoff.

Example:

 - Each freelist in the array tracks blocks
   of size 8 + 2^ii
 - This could also just be a fixed list.

```
  [0] => (size 9)
  [1] => (size 10)
  [2] => (size 12)
  [3] => (size 16)
  [4] => (size 24)
  ...
```

 - We're going to pre-allocate a page for each list
   up to size 2048
 - log2(2048) ~ 11, so we need like 11 free lists,
   or 11 pages, or 44k.

Problem: Where do we put the size field to avoid
needing to store it in the allocations?

Answer: At the beginning of the chunk (= 1 page in
the example).

So the page for size=24 allocations might look like
this:


```
 +-------------------
 | 168 24-byte blocks
 +-------------------
 | bitmap, used field for each block
 | 170 bits = 22 bytes
 +-------------------
 | next, prev (8 bytes each)
 +-------------------
 | mutex (8 bytes?)
 | beware deadlock on linked list removals
 +-------------------
 | size = 24 (8 bytes)
 +------------------- 
```

 * 4k / 24 = 170
 * ceil(170 / 8) = 

Complicaiton: What if we need more than 168
24-byte blocks.

 - Have a linked-list of chunks.

Optimized bucket allocator:

 - Fast
 - Complicated
 - Ridiculously tunable.
 
Problem: Multiple threads?

 - One mutex per chunk.
 - Linked list removals may need
   a ```pthread_mutex_try_lock``` scheme
   with a don't delete me flag

## Thread-local storage

Each thread has its own local free list.

Advantage:

 - No data races
 - No locks

Problem: How do we free into another thread's
local free list?

 - This is tricky.

Plan A:

 - Have a global freelist for non-local frees.
   - With a lock.
 - When we malloc or free from the local list,
   sometimes check the global list.
   - Maybe do it 1/10th of the time.
   - Maybe do it when the free list gets
     empty.
   - Some other time.

Plan B:

 - Give each thread an "inbox" for remote frees.
   - With a lock.
 - When we free, we also check the inbox.


## Multiple Arenas

Example implementation:

 - On first allocation, allocate an array
   of (# of hardware threads)*2 free lists.
 - Each thread has a thread local variable
   storing the index of its favorite free list.
 - When we try to access a freelist to allocate:
   - ```pthread_mutex_trylock```
   - If that fails, ```favorite = (favorite + 1) % KK```.
   - Then we do ```pthread_mutex_lock``` and block.
 - Allocated header includes area #. 
 
 









