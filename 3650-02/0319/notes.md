
# Teams for CH02

 - You need a team.

# Allocator Questions?

 - sizeof(size_t) = 8 bytes
 - ```char*,void*,uintptr_t``` are all ways
   to do direct address math in bytes.

# Allocator Slides

If you malloc(5000)

 - +4 for the size field
 - we need 5004 bytes
 - size field is filled in as "5000"
 - hopefully we put the remaining ~3000 bytes
   from the two pages back on the free list

Splitting free space:

 - Every block must be big enough to be a node.
 - Here sizeof(node_t) = 8

 - If we have 12 bytes on the free list (size = 8
   in slides convention)
   - We always allocate all 12 bytes.
   - Because two nodes can't fit in 12 bytes.

Header vs. Node

 - We're using the same memory for both.
 - The size field is in the same place either way.
 - On malloc, we reinterpret node as header,
   size stays in the same place.
 - On free, we reinterpret header node,
   size stays in the same place.
 - "reinterpret" mechanically just means "cast"
 - Every block of memory is exactly one of:
   - Header + User memory
   - Node + Unused space

How do you place a node or header on to the heap?

 - Our pointers to "heap" memory initially come
   from mmap.
 - To get a node or header, we pick a valid pointer
   and cast it to node* or header*.
 - A valid pointer is just the result of taking
   a pointer from mmap and adding any number of
   bytes to it that will result in it remaining within
   the allocated region.

```
    void* base = mmap(... 4096 ...);
    uintptr_t addr = (uintptr_t) base;
    header* hdr = (header*)(addr + 700);
    // hdr is a valid header
    //  - it's between base and 
    //    (base + 4096) - sizeof(header)
```


```
    struct goat {
        int size;
        int color_code;
    }
    
    goat* gg = malloc(...);
    gg == &(gg.size);
```

## Multiple Arenas

Plan A: One free list.

 - Problem: Contention between multiple threads.
 
 
Plan B: More than one free list.

 - We call each one an "arena".
 - Threads have some mechanism for selecting a free
   list to use for each allocation, with an intention
   of minimizing contention.
 - How many arenas?
   - Maybe two per hardware thread?
   - ten?
 - Which arena does thread t use for an allocation
   as its first choice?
   - arenas[t % NO_ARENAS]?
   - arenas[rand()]?

How do we determine which arena memory came from?

 - Plan A: Add an arena id to the header.
 - Plan B: Can you figure it out from the
   memory address?

What is OpenSSL?

 - An opensource implementation of a bunch
   of cryptography useful for implementing network
   protocoals like SSL / TLS.

What was Heartbleed?

 - A famous OpenSSL bug.
 - OpenSSL provided a "heartbeat" mechanism, which
   allowed network clients to send arbitrary data
   and have the network server sent it back.
 - There was a size field.
 - OpenSSL just believed the size field, so you
   could send "hello; length=5000" and it would
   allocate a buffer "hello" and send you the
   5000 bytes of memory staritng with h.
 - OpenSSL liked to store cryptographic keys in
   memory.

More context:

 - The OpenSSL developers decided to implement
   their own memory allocator.
 - This was simple - reuse memory as fast as
   possible, never zero anything.
 - The OpenBSD developers just deleted the 
   custom allocator, and Heartbleed stopped
   sending crypto keys.
 - Moral: Don't write a custom allocator for
   security-critical code.

# Techniques for CH02

Optimized Bucket Allocator

```
 // probably global
 // or directly mmaped
 frees[0] =  // blocks of size 4
 frees[1] =  // blocks of size 8
 frees[2] =  // blocks of size 12 
 frees[3] =  // blocks of size 16
 frees[4] =  // blocks of size 24
 frees[5] =  // blocks of size 32
```

frees[3] points to entries of size 16

optimization: point to a page of blocks of
size 16

```
    +------------------------
    |  256 blocks of size 16
    +------------------------
    | bitmap: 256 bits = 32 bytes
    | of "used" flags
    +-------------------------
    | next page ptr
    | prev page ptr
    +-------------------------
    | size = 16 (8 byte)
    +------------------------
```









