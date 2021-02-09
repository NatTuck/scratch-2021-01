
# Homework Questions on HW03


# Structs

Remember to do ```typedef struct x { ...} x```

# C Built in Data Types

 - Arrays, unknown size, same type
 - Structs, fixed size, different types

# Common Structures

 - Sequence - Variable Length Arrays or Linked Lists
 - Key => Value Map - HashMap and TreeMap

# Variable Length Arrays

Problems: 

 - Arrays are fixed length once allocated
 - Arrays don't even know their own size

What we want:

 - Normal array functoinality: put / get by index
 - A ```push_back``` that inserts a new item at the end
   of the array, growing if needed.
 - Our data to be contiguous in memory for 
   interoperability with stock array tools.

Plan A:

 - Start with a normal array.
 - Store it in a { size, data } struct.
 - When we push an item, allocate a new data array
   with size one larger, copy over the old items,
   and free the old array.

Problem: Inserting n items with ```push_back``` 
takes O(n^2) time.

First copy is 1 item, then 2, then 3, ..., then n.

 - 1 + 2 + 3 + ... + n = O(n^2)

Solution: Don't just grow by one. Instead, we're going
grow by a fixed factor. Doubling in size each time
is the simplest.

Result: Inserting n items with ```push_back``` ends
up taking O(n).

Data structure is { int size, int cap, T* data }.

Start with capacity 4 and size 0, and insert 100 
items. How many items to insert or copy?

```
100
+ 4 + 8 + ... + 64 <= 128
<= 228 < 3x
```

A Variable Length Array (vector in C++ or ArrayList 
in Java) is:

 - A struct
 - Three fields:
   - data - pointer to array
   - size - number of items currently in array
   - cap  - size of allocated array
 - size <= cap
 - Non-growing operations just work on vla->data[ii].
 - To append, we insert into next free space.
 - If there's no free space, we call realloc(3) to
   double the size of the data array (and thus the
   current capacity).

# Linked Lists

Basic structure:

```
struct cell {
  T head
  cell* tail
}
```



























