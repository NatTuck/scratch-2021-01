+++
title = "./19-malloc-pt3/opt-buddy.md"
+++

# Optimizing a Buddy Allocator

## Buddy System Review

 - Start with a constant heap size, e.g. 1MB
   - If we run out of space, we allocate a whole new heap.
   - So we innately need to handle multiple arenas - each buddy system
     heap is a separate arena.
 - Have an array of free list pointers.
   - One bucket for each power of two up to the heap size.
   - For a 1MB = 2^20 B heap, that's slots numbered 0..20. We don't use index
     zero since we can't do 1B allocations, but that's fine.
   - Doubly linked for O(1) insert / remove.
 - Allocate 128k
   - Once: Split 1MB => 512k, 512k => 256k, 256k => 128k
   - Again: No need to split, we've got one.
   - Again: Split 256k => 128k
   - Free the first one, no merge.
   - Free the last one, no merge.
   - Free the middle one, merge back to 1MB.

## Problem: Our free list structure too big.

```
struct cell {
    // in both cell and header
    long size;
    long arena;
    bool used;
    
    // not in header
    cell* next;
    cell* prev;
};
```

sizeof(cell) = 40

On AMD64, a reasonable minimum header size is 8. Pointers need to be 8-byte
aligned, so using a size 8 header maintains that alignment for the pointer we
return to the user.

Similarly, a reasonable minimum cell size is 16, since nobody should really
expect the allocator to be efficient for allocations smaller than 8.

So let's fit in those sizes:

```
struct cell {
    // in both cell and header
    uint32_t arena;  // up to 4B of them
    uint8_t  size;   // store which power of two
    uint8_t  used;   // used flag
    uint16_t _pad;
    
    // not in header
    int32_t next;   // offset from start of arena, -1 is EOL
    int32_t prev;   // offset from start of arena, -1 is EOL
};
```
