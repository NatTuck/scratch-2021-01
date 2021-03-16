

# Current HW: Parallel Sort with Threads

# Today's Topic: Writing a Memory Allocator

```
 +----------------------------
 |  stack (grows down)
 +----------------------------
 |
 +----------------------------
 |  the heap (pieces returned by mmap)
 +---------------------------- 
 |  the heap (pieces returned by mmap)
 +----------------------------
 |
 +----------------------------
 |  .text
 +----------------------------
 |  .data
 +----------------------------
```

Why can't we just implement malloc and 
free by calling mmap and munmap?

```
void*
malloc(size_t sz)
{
    // Problem 2:
    // - Minimum allocation size for
    //   mmap is 1 page = 4096 bytes
    return mmap(..., sz, MAP_PRIVATE|MAP_ANONYMOUS, ...);
}

void
free(void* addr)
{
    // Problem 1:
    munmap(addr, length?);
    // Somehow we need to be able to get the
    // size of an allocation given only the pointer
    // to that memory.
}
```

So we need to:

 - Optimize small allocations to fit multiple
   blocks per page.
 - Store the sizes of allocated blocks. 


```
int* foo = malloc(327);
...
double* bar = (double*) foo;
...
free(bar);
```

The traditional solution to one of these problems
is a "free list" structure.

The idea is: We keep track of blocks of memory that 
have been requested by the OS and are not currently
allocated to the user program.

For example:

 * The user requests 100 bytes
 * We request 4096 bytes from the OS.
 * We give the user a 100 byte block.
 * We still have a 3996 byte block to keep
   track of, which we can then use for future
   requests.

Importantly, we don't need to track allocated
blocks - we just give the user a pointer and
expect them to give it back.

To track size, we can take advantage of the fact
that we're the memory allocator. When the
user requests space, we can give them the space
they asked for plus some extra to store the
size.

 * User requests 100 bytes.
 * We can add 8 bytes for a size field.
 * We really allocate 108 bytes.

```
// 100 byte allocation with user code
// holding the ptr
       +-----------------
       |
       |
       |
       |  100 bytes
       |
ptr -> +-----------------
       | size (8 bytes)
x   -> +-----------------
// total space is 108 bytes
```

We need to store a list of free blocks of memory.

```
struct free_block {
    void* addr;
    long  size;
};
```

Plan A: We'll have a vector of ```free_block``s.

 * We're trying to build the allocator, so we
   don't have malloc or realloc yet.

Plan B: Linked list

 * Same problem?
 * But... we're trying to keep track of unused
   blocks of memory
 * Let's use that memory to store our linked
   list - each block can be used as a linked
   list cell.

```
// Address is the pointer *to* this structure.
struct free_block {
    long  size;
    struct free_block* next;
};
```

```
// 108 byte block on the free list
       +-----------------
       |
       | 92 bytes unused
       |
       +-----------------
       | next (8 bytes) = ...
       +-----------------
       | size (8 bytes) = 108
ptr -> +-----------------
```

## Large allocations

 * The free list is an optimizatoin for
   small allocations.
 * May still help for medium allocations, because
   it reduces syscalls.
 * For large allocations, it's just extra work - we
   should just delegate to mmap.
 * We'll use the smallest possible cutoff: >= 4k is big 

Pseudocode:

```
def malloc(size):
  size += 8   # for the size field
  if size < 4096:
     # small allocation
     block = search freelist for block >= size
     if block:
        split it, put rest on free list
        fill in size
        return our chunk of block
     else:
        page = mmap(4k)
        stick page on free list
        return malloc(size - 8)
  else:
     # big allocation
     ptr = mmap(size)
     *ptr = size
     return ptr + (8 bytes)

def free(ptr):
  size = *(ptr - (8 bytes))
  if size < 4096:
    insert block into freelist
    merge adjacent blocks
  else:
    munmap(ptr - (8 bytes), size)
```

Example:

```
for (ii in 0..4):
    xs[ii] = malloc(1800)

for (ii in 0..2):
    free(xs[ii])

yy = malloc(3000)
```

Blocks are written as (location, size)

```
free_list = [(p0+1808, 1808), (p0+0, 1808), 
             (p1+3616, 480), (p0+3616, 480)]

xs[0] = invalid
xs[1] = invalid
xs[2] = (p1+0, 1808)
xs[3] = (p1+1808, 1808)
yy    = malloc(3000)
```

Diagram of p0 before malloc of yy

```
       +-----------------
       | 480 never used
       +-----------------
       | 1808 in a block that was xs[1]
       +-----------------
       | 1808 in a block that was xs[0]
p0 ->  +-----------------
```

```
free_list = [
  (p0+3008, 1008), 
  (p1+3616, 480), 
]

 yy = (p0+0, 3008)
```

yy

 - want 3008
 - 4096 - 3008 = 1088

xs[2]

 * 1800 + 8 = 1808
 * 4096 - 1808 = 2288 

xs[1]
 
 * 1800 + 8 = 1008
 * 2288 - 1808 = 480
 * 1808 + 1808 = 3616

xs[0]

 * 1800 + 8 = 1808
 * 4096 - 1808 = 2288

```
// Assuming p0+4096 == p1

free_list = [
  (p0+3008, 2816), 
  (p1+3616, 480), 
]

xs[2] = invalid
free(xs[2])
```

## Design considerations

Should we use a doubly linked list?

  - Makes deletes a little easier.
  - Lets us do deletes with a pointer to the middle
    in O(1) time.
  - The code is noticibly more complicated.
  - For HW08, a singly linked list is strongly recommended.

Is there any benefit to sorting the list?

 - With a free list sorted in ascending order by size,
   we can find the best fitting block in the list
   in the minimum number of comparisons / list items
   traversed.
   - With this optimization, malloc is O(n).
     - Maybe this reduces us to N/2 comparisons.
   - Without it, malloc takes O(n).
     - Worst case, N comparisons.
 - With a free list sorted in ascending order by memory
   address, we can coalesce adjacent blocks while inserting
   for no additonal cost.
   - With this optimization, free is O(n).
     - Average N/2 operations.
   - With the worst naive implementation, free
     is O(n^2).
   - Without this optimization, but reasonably keeping
     the list coalesced, free is O(n).
     - Always going to take a full N operations.
   - List sorted by memory address is recommended for
     HW08, because it's easier.
     
How big should the big allocation threshold be?
   
  - For HW08, you're required to make this 4k.
  - In practice probably >= 1MB is good, but
    you'd need to benchmark.

When should we return memory to the OS?

  - Frequently, so other programs can use the memory.
  - Rarely, because calling munmap on a block just
    to mmap another block right away is silly.
  - For HW08, always unmap big, never unmap small.

## Can we beat O(n) malloc / free?

Yes.

It looks like using a linked list gets is O(n)
operations.

```
// Address is the pointer *to* this structure.
struct free_block {
    long  size;
    struct free_block* next;
};
// Minimum alloction size is sizeoff(free_block) = 16
// because otherwise we can't stick the block
// on the free list.
```


Hash table?

 - Sometimes hash tables take O(n).
 - They only do exact comparisons
   - If we're looking for a block of size 100, we won't
     find an avaiable block of size 102.

Balanced binary search tree?

 - Always worst case O(log n).
 - Searches can find nearby stuff.

```
struct free_tree_node {
    size_t size;
    free_tree_node* left;
    free_tree_node* right;
};
// Minimum alloc size = 24
```

Order tree by size:

 * malloc is O(log n)
 * free is O(n)

Order tree by memory address:

 * malloc is O(n)
 * free is O(log n)

```
struct free_tree_node {
    size_t size;
    free_tree_node* size_left; // tree ordered by size
    free_tree_node* size_right;
    free_tree_node* addr_left; // tree ordered by addr
    free_tree_node* addr_right;
};
// Minimum alloc size = 40
// malloc(16) is no longer efficient
```

 * malloc is O(log n)
 * free is O(log n)

# Buddy System

 * Let's have an array of free lists.
 * Let's have each free list store blocks
   of only one size.
 * Let's restrict those sizes to powers
   of two.
 * Let's pre-allocate a heap, say 1MB
   as an example.
   - That means we need 21 free lists.
     for 2^20, 2^19, ...
 
```
  frees[20] = ... // blocks of size 2^20
  frees[19] = ... // blocks of size 2^19
  frees[18] = ... // blocks of size 2^18
  ...
```

```
  frees[20] = []
  frees[19] = [(+2^19, 2^19)]
  frees[18] = [(+2^18, 2^18)]
  ...
```

malloc(150010)
 + 8 = 150018
 round up to 262144 = 2^18
 <= (+0, 2^18)

With this system:

 - Malloc is O(20), or O(log n), but less than O(40)
 - How fast is free?


```
  frees[20] = []
  frees[19] = [(+2^19, 2^19)]
  frees[18] = [(+2^18, 2^18)]
  ...
```

free((+0, 2^18))

 - If we XOR the address with the size, we get
   our buddy.
 - We can merge with buddy in constant time.
 
 
```
struct buddy_header {
    uint_8_t size;     // bytes = 2^size
    uint_8_t used;     // is this allocated?
    uint16_t heap;     // which heap?
}
```

 - We need a doubly linked list for constant
   time removal.

```
struct buddy_cell {
    uint_8_t size;     // bytes = 2^size
    uint_8_t used;     // is this allocated?
    uint16_t heap;     // which heap?
    uint32_t next;     // offset from start of heap
    uint32_t prev;     // offset from start of heap
}
// minimum allocation = sizeof(cell) = 12
```

 - free is O(log n) < O(32)


Note: Buddy system isn't nessisarily the thing
you want for CH02.

Modern high performance allocators for machines
with lots of memory tend to use a "bucket"
allocator.



# Questions

 - Two memory blocks are adjacent if
   b1.addr + b1.size = b2.addr OR
   b2.addr + b2.size = b1.addr
 
 


