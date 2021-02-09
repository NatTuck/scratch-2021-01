
# HW03 Questions

```
    long nn;       // nn in 16(%rsp) 
                   // where does this go?
                   // Why the stack?
    read_int(&nn); // We're passing address of
                   // nn, and registers don't
                   // have addresses.
                   // Local variables can be
                   // allocated in registers
                   // or on the stack.
```

We've got a variable, ii, that needs to survive a
function call. It can't simply go in a temporary
register, so we can:

 - Put it in a safe register and follow the callee-save
   convention.
 - Put in a temporary and follow caller-save.
 - Allocate it on the stack.

# Structures

Remember to say ```typdef struct x {} x;```

# Vectors

C only comes with two built in data structure mechanisms:

 - Arrays let us store a variable number of items of the
   same type.
 - Structs let us store a fixed number of times of varying
   types.
  
Arrays have some weaknesses:

 - They don't know their own size.
   - Idea: ```struct { long size; T* data; }```
 - They can't be resized. Once we allocate an array
   of size nn, it's that size.

Solution to both: Variable Length Arrays

What we want:

 - Normal array functionality: put, get by index.
 - A "push back" operation that adds an item to the
   end of the array, growing it if needed.
 - Arrays should be contiguous in memory so we can
   do loops normally and pass the memory to normal
   array functions. 

Plan A:

 - Start with a normal array
 - When we do push_back, we allocate a new array 
   that's one slot bigger, copy over the old array,
   then free the old array.

Problem: Filling an array of n items takes n^2 time.

First copy is 1, second is 2, ...

 - 1 + 2 + 3 + ... + n  is O(n^2)

Solution: Don't add just one slot, double the size of 
the array. (Doesn't really need to be "double", 
increasing by any constant factor (e.g. 1.5x) would work.)

Result is that inserting N items takes O(N) time.

Example:

```
 size: 0, cap: 4, data: [_, _, _, _]
 size: 1, cap: 4, data: [1, _, _, _]
 size: 2, cap: 4, data: [1, 2, _, _]
 size: 3, cap: 4, data: [1, 2, 3, _]
 size: 4, cap: 4, data: [1, 2, 3, 4]
 size: 5, cap: 8, data: [1, 2, 3, 4, 5, _, _, _]
 size: 6, cap: 8, data: [1, 2, 3, 4, 5, 6, _, _]
 size: 7, cap: 8, data: [1, 2, 3, 4, 5, 6, 7, _]
```

Most programming languages have at least two collection
types either built in or easily accessible in the stdlib:

 - Sequence type: Usually a vector / VLA like C++ or python
   or a linked list like in Racket or Haskell.
 - Key-Value map type: Usually a HashMap or TreeMap.

A Variable Length Array (vector in C++, 
Array List in Java) is:

 - A struct
 - Three fields:
   - size
   - capacity
   - data (pointer to array)
 - size <= capacity; capacity is # of slots allocated
   for data
 - Non-resizing operations simply operate on 
   vla->data[ii]
 - To append, we insert into free slots.
 - If there are no free slots, use realloc(3) to double
   the current capacity.

In Java, Linked Lists are LinkedList.


# Linked Lists

Made up of cells:

```
typdef struct list_cell {
    T head;            // this item
    list_cell* tail;   // the rest of the list
} list_cell;
```









