
# First: HW Stuff

 - Don't use global variables to pass
   arguments to functions.

```
int y;

int 
add_two_numbers(int x)
{
    return x + y;
}

    y = 4;
    add_two_numbers(7);
```

But what about with threads?

```
struct two_ints {
    int x;
    int y;
}

void*
thread_main(void* arg)
{
    two_ints* stuff = (two_ints*) arg;

}
```

Writing to a file in the thread

 - open the file with open(2)
 - write to the file with write(2)
 - different threads need to write to
   different places in the file
   - we need to get to the place, look for
     some sort of "seek" function to adjust
     the cursor
   - if two threads try to seek at the same time,
     then the file cursor itself could be subject
     to a data race
   - each thread may want to "open" the file itself
     to get a separate file descriptor (and cursor)

# Today: Memory Allocators

```
 +-----------------------
 | stack (grows down)
 +-----------------------
 |
 +-----------------------
 | 
 +-----------------------
 |  .data
 +-----------------------
 |  .text
 +-----------------------
```

To get "heap" memory 
we do ```mmap(... MAP_PRIVATE|MAP_ANONYMOUS ```


```
// This doesn't quite work
void*
malloc(size_t size)
{
    return mmap(... size ...);
}
```

Two complications:

 - mmap works in pages. There's no way to allocate
   less than 4k
   - This is a problem for small allocations.
     Imagine allocating linked list cells (~ 16B/each)
     if the miniumum allocation is 4k.
   - So we need to be able to allocate more than one
     thing per page.
 - munmap takes a size, free does not
   - The implementation of malloc/free must store the
     size of allocations somewhere itself.

Some other considerations:

 - We'd like to reuse memory after it's freed, rather
   than always calling munmap.
 - We don't want to leave memory allocated forever,
   we eventually want to give it back to the OS.
 
Interesting property of mmap / Linux memory management
in general.

 - mmap allocates virtual memory
 - Storing stuff in that memory results in physical
   memory being allocated

ex.

 - char* buffer = mmap(100MB)
 - Write 9KB of data to the beginning of that buffer
 - We actually use 12K of physical memory
   
## Traditional / Simple Design: A Free List Allocator

We maintain a "free list" data structure, which
is a list of blocks of memory that have been allocated
by the OS but are not used by the user program.

When the user calls malloc

```
   // example
   int* xs = malloc(10 * sizeof(int));
```

 * First, we check the free list for a large enough
   block to satisfy the request.
   - Where do we store the size?
   - We're the allocator, so we can allocate
     extra space for the size.
 * (a) If there's a big enough block:
   - We'll split off a piece as big as we need for
     our allocation, putting the rest back on the
     free list.
   - Return that block to the user.
 * If we don't have a big enough block:
   - We request a large block from the operating
     system (at least 4k, with mmap)
   - Now we've got a big enough block, goto (a)

```
  struct block {
    void*  addr;
    size_t size;
  };
    
  free_list = [(addr+48, size=4048)];
   
  int* xs = malloc(10 * sizeof(int));
         // malloc(40);
         //   40 + sizeof(size_t) = 48
         // returns addr+8 (a size=40 block)
  // the block we return is:
  //    - @ addr, we store size = 48
  //    - @ addr+8, we have 40 bytes for the user
    
  free(xs);
         // we get back addr+8
         // we can find size now, it's at xs-8
```

  - free list is empty
  - mmap(4k)

Complication: Where do we store the free list?

  - Plan A: Vector
    - Problem: We're writing the allocator, so we
      don't have malloc.
  - Plan B: Linked list
    - Apparently the same problem.
    - Neat trick: We're tracking blocks of unused
      anyway.
    - We can use the blocks of memory themselves
      as linked list cells.

```
   // pointer that got us here is
   // the memory address
   struct free_list_cell {
      size_t size; // this is also where 
                   // the size header goes for
                   // allocated memory
      struct free_list_cell* next;
   };
   // The block of memory we're using
   // may be (probably is) larger than
   // sizeof(free_list_cell).
``` 

Example 1:

```
  for (0..3) {
    xs[ii] = malloc(1800);
  }
  
  for (0..1) {
    free(xs[ii]); 
  }
  
  yy = malloc(2000);
```

```
    // initial value
    struct free_list_cell* free_list = 0;
    
    free_list = [(size=400b1), (size=400b0)]
    
    xs[0] = (size=1808b0#0)+8
    xs[1] = (size=1808b0#1)+8
    xs[2] = (size=1808b1#0)+8
    xs[3] = (size=1808b1#1)+8
```

 - Free list is empty, mmap 4k
   - 1800 + 8 = 1808
   - 4096 - 1808 = 2288
 - Free list has one item on it
   - 1800 + 8 = 1808
   - 2208 - 1808 = 400
 - Free list has one item on it - size 400
   - 1800 + 8 = 1808

```
    free_list = [
      (size=1808b0#1), // these two blocks
      (size=1808b0#0), // are adjacent
      (size=400b1), 
      (size=400b0)
    ]
    
    xs[0] = (invalid)
    xs[1] = (invalid)
    xs[2] = (size=1808b1#0)+8
    xs[3] = (size=1808b1#1)+8
```

 - free(xs[0])
 - free(xs[1])
 - malloc(2000)
   - 2000 + 8 = 2008


```
  // coalesce adjacent blocks
  free_list = [
      (size=1608b0), // merged
      (size=400b1), 
      (size=400b0)   // problem: this one should
                     // get coalesced too
  ]
  
  yy = (size=2008b0)
```

## Coalescing

 - Whenever we free a block of memory we want
   to merge it with all adjacent blocks, so
   the blocks on the free list are as big as
   possible.
 - Otherwise we get fragmentation, and we end
   up mmapping too much.

Two ways to accomplish this:

 - If the free list is unsorted and may not
   have been properly coalesced previously,
   this requires on O(n^2) scan.
 - If the free list is sorted in order of 
   memory address and we keep things
   coalesced, then insert-and-coalesce can
   be done in O(n).

## Big Allocations

 - If the user says free(malloc(10GB)), we want
   to return that to the OS.
 - For big allocations, the cost of the syscalls
   is unlikely to be a performance issue.
 - So we can safely just delegate big allocations
   to mmap and munmap.
 - For the homework, big is > 4k.
 - For an efficient allocator, big probalby is
   bigger than that.

## Pseudocode for HW08

```
malloc(req_bytes):
    size = req_bytes + 8
    if (size > 4k)
        return mmap(size) // with stored size
    else
        return search_free_list(size) || mmap;

free(ptr)
    size = (size_t)(8 bytes before ptr)
    if (size > 4k)
        munmap(...)
    else
        stuff_on_free_list_and_coalesce
```

For HW08:

 - Singly linked list
 - Sorted in order of memory address

## Design Tradeoffs

Doubly linked list?

 - Singly linked lists are simple to write.
 - Singly linked lists are more compact.
 - Doubly linked lists let us do inserts in
   constant time given a pointer to the previous
   or next item - but that doesn't help us yet.

Ordered or unordered list?

 - In an unsorted list, cons is O(1).
   - Problem: Coalescing gets expensive.
 
How big should the "big" cutoff be?

 - Only way to be sure is to benchmark.

Performance:

 - malloc is O(n)
   - We potentially have to scan the whole
     free list to find out there isn't a big
     enough block (before calling mmap).
 - free takes O(n)
   - Inserting into free list takes O(n)
     since we're maintaining sorted order.
     
Minimum allocation size:

```
   struct free_list_cell {
      size_t size;
      struct free_list_cell* next;
   };
```

 - ```sizeof(free_list_cell)``` = 16 bytes
 - Since every block of memory, when freed,
   needs to go on the free list, or minimum
   allocation size is 16 bytes.
 - That corresponds to malloc(8).
 - malloc(1) has a 16x overhead


Can we do better than O(n) for these operations?

 - Clearly a linked list tends towards O(n)
   operations. Can we use a better data
   structure?
   - HashMap?
     - This lets us quickly answer questions
       about exact equality. Is there a block
       on the free_map of size exactly 309?
     - That doesn't find a block of size 310.
     - Might be good for finding adjacent blocks.
     - HashMaps are sometimes O(n), which isn't 
       great if we want consistent fast performance.
    - Balanced binary tree
     - This lets us answer questions like "is there
       a block >= size?" in O(log n) time.
     - So if tree orders by size, we can do
       O(log n) malloc.
     - If the tree orders by address, we can do
       free (and coalesce) in O(log n) time.

```
// Pointer to free_tree_node is our
// memory block address.
struct free_tree_node_v1 {
    size_t size;
    ftn* left;
    ftn* right;
}
// minimum allocation is now 24

struct free_tree_node_v1 {
    size_t size;
    ftn* size_left;
    ftn* size_right;
    ftn* addr_left;
    ftn* addr_right;
}
// minimum allocation is now 40
```

 - If we're in two trees at the same time
   then
 - malloc is O(log n)
 - free is O(log n)

## The "buddy system"

What if we restricted the sizes of possible
allocations?

 - Let's only allow allocations of power-of-two
   sizes.
 - Let's have one big heap, allocated up front.

When the program starts, or the first allocation,
we'll allocate a 2^20 byte heap.

Instead of a free list, we'll have an array of
21 free lists.

```
  // frees[ii] points to free blocks of
  // size 2^ii bytes.

  frees[20] = []
  frees[19] = [(+2^19, size=2^19)] 
  frees[18] = [(+2^18, 2^18)]
  frees[17] = [(+2^17, 2^17)]
  frees[16] = [(+2^16, 2^16)]
  ...
```

```
  // this is a header on both allocated
  // and free blocks
  struct buddy_system_header {
    uint_8_t size_category; // which power of two
    uint_8_t used; // flag
    uint_16_t heap_id;
  }
  
  struct buddy_system_list_cell {
    uint_8_t size_category; // which power of two
    uint_8_t used; // flag
    uint_16_t heap_id;
    uint32_t prev; // offset from start of heap
    uint32_t next; // offset from start of heap
  };

  // minimum allocation size is still only
  // 12 bytes
```

malloc(65000)

 - Round up to next power of two and add
   a size header.
 - That puts us in the 64k = slot 16 category
 - result = (+0, 2^16) 

free( (+0, 2^16) )

 - Look at size field, this is slot 16
 - Then we need to try to find our "buddy",
   or the other half of our block one size
   category up.
 - We can do that in constant time with
   pointer arithmetic.

```
    addr = ...
    buddy address = addr XOR 2^16
```
 - If buddy is free, merge.
 - If buddy is not free, just insert this
   block onto correct list.




