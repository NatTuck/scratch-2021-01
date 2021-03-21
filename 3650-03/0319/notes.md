
# Challenge 2: Teams

 - Inkfish *requires* a team to submit.
 - Work with a partner or alone.
 - Mechanisms to request team are on Piazza.

 - (A) You start with HW08 allocator.
   - +mutex
 - (B) Write a completely new optimized allocator
 - Compare (A, B, system allocator)

# Homework Questions

 - A page is 4096 bytes, and is the unit of memory
   management for the OS and CPU.
 - A "block" is whatever you want it to be.
 - As I use block:
   - It's an entry on the free list
   - Or an allocated chunk of user memory

For AMD64 Linux, we can safely compare memory
addresses by casting to ```uintptr_t``` type.
(```uintptr_t``` is ```uint64_t``` is ```unsigned
long``` is ```size_t```).

The free list is:

 * Tracking free blocks of memory.
 * Made up of linked list cells.
 
To request memory for the free list, use mmap
to get an empty block. For HW08, these blocks
should be one page.

# Allocator Slides

 * Free list head is a global variable.

What is OpenSSL?

 - A library that implements a bunch of cryptography
   stuff that you need to build encrypted network
   apps (e.g. TLS).

What was Heartbleed?

 - OpenSSL provided a heartbeat mechanism, where
   anyone on the internet could send a request
   to an TLS server to echo back a message.
 - "Hey TLS server, the message is 'hello', send
    back those 5000 bytes."
 - What did they have in memory after the buffer?
   - Cryptographic keys
 - The OpenBSD people looked, and they found a
   custom memory allocator from like 1996.
   - Last in first out.
   - Never zero memory.
   - When the OpenBSD developers removed the
     custom allocator and just used normal malloc
     the problem went away.
  - Moral: If you care about security, don't write
    custom memory allocators.
 
# Bucket Allocator

```
    frees[0] = // blocks of size 4
    frees[1] = // blocks of size 8
    frees[2] = // blocks of size 12
    frees[4] = // blocks of size 16
    frees[5] = // blocks of size 24
    frees[6] = // blocks of size 32
    frees[7] = // blocks of size 40
    frees[8] = // blocks of size 48
```

Simplest set of sizes would be:

 - Powers of two
 - Midpoints between those.

If we had a free list for each of those sizes
up to 1 meg, that'd be 2*log(1M) = 40 free lists.

If we allocate a full page to each, that'd
take 40 * 4k = 160k of RAM.

Following this plan, malloc and free are O(1).

The internal fragmentation (from rounding up sizes)
can be traded off against the fixed overhead.

Problem: We still have the overhead of storing the size
for every single allocation. Storing an 8 byte size
field for 4 bytes of data is a 3x overhead.

Can we avoid the size field?

 - We're already allocating full pages of each size.
 - Every allocation on each page is the same size.
 - What if we just have one size field for each page?

```
1 4k page:
  +---------------------------
  |  252 size-16 blocks
  +---------------------------
  |  Does mutex go here?
  |   What if someone has a pointer to
  |   an adjacent page and locks it?
  |   Oh no! Dining Philosophers again!
  +---------------------------
  |  used bitmap (32 bytes)
  +---------------------------
  |  next, prev ptr (16 bytes)
  +---------------------------
  |  size = 16 (8 bytes)
  +---------------------------
```

Where does mutex go? Good question. 






