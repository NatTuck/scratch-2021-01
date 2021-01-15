+++
title = "./07-data-structures/notes.md"
+++

# Computer Systems

## HW Questions?

# Structs

 - goats.c
 - rewrite with value goats

Stuff to mention:

 - Memory layout
 - Why copy string?
 - typedef struct because two namespaces
 - The difference between "." field access and "->" access.
 
# Vectors

C gives us only two built in kinds of data structure:

 - Structs
 - Arrays

Any other data structure we need we'll need to build ourselves. Luckily, those
two building blocks are enough to build almost all other data structures.

Most programming languages provide two core data structures that are extremely
useful in building programs:

 - A variable-size sequence (Lists, ArrayList, vector, etc)
 - Key-Value Maps (Dictionary, map, hash, TreeMap, HashMap, etc)

In homework 4, you'll get to build one of each.

We'll look at map structures next class. For now, it's time for our sequence
type:

Variable Length Arrays:

 - C provides arrays, but they have a fixed length when allocated and
   don't have a built-in mechanism to grow when needed.
 - They don't even know their own size. Really, they're just pointers
   to blocks of memory.

What we want:

 - Normal array functionality: put / get by index.
 - A "push back" operation that allows us to append an element to
   the end of the array.
 - For our arrays to remain contiguous in memory so loops work normally.

What that means:

 - We can start with a normal array.
 - When we push an item on to the end, we need to allocate new space,
   copy the elements over, and then free the old space.

Problem: Filling an array of n items takes n^2 time.

 - To add N items, that's N reallocations.
 - Each reallocation requires copying.
 
Solution: Allocate more space than we need.

 - Instead of allocating an empty array, we allocate a small array - maybe 4
   items.
 - When we run out of empty slots, we double our allocated space.
 - This means we need to track actual size and allocated space (capacity)
   seperately.
 - Inserting n items now takes O(n) time.
   - Count the operations
   - Generate a function for operations per insertion
   - Show it's O(n) for n inserts.
   - Note that inserting *an* item may still take O(n) time if it triggers
     a resize and copy.

A Variable Length Array (vector in C++, ArrayList in Java) is:

 - A struct
 - Three fields: 
   - data - a pointer to the element array
   - size - the number of items in the array
   - capacity - the size of the data array (always >= size)
 - Non-resizing operations are accomplished by simply accessing vla->data[ii].
 - To append, we insert into free spaces.
 - If there are no free slots, we call "realloc(3)" (see "man 3 realloc") to
   resize the data array to double the current capacity.

# Linked Lists

 - Draw a linked list.
 - Describe insertion sort.
 - Talk about how that really wants a garbage collector and
   shared structure.
 - Write list-sort.c

# Function Pointers

 - Show map.c
