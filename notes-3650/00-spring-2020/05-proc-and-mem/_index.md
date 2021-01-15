+++
title = "./05-proc-and-mem/notes.md"
+++

# Computer Systems

## Next HW:

 - Next homework is up: Quicksort

## HW Questions?

## What's a process?

A process is what you get when you run a program.

It has a memory address space, a set of register values,
and some other stuff.

Segments in memory:

From the executable file on disk:

 - Code (.text)
 - Global / Static Data (.data)
 
Not from disk:

 - Stack
 - Heap

## Static global data

```
.data
.space X, Y (reserve X bytes, each with value Y)
```

## Alloca and the Stack

 - We can allocate variable sized data on the stack.
 - This should be done carefully
   - If you have non-variable size data, put that first (on the bottom)
     so it won't be moved around by the variable sized data.
   - After "enter", you can manually move the stack down further
     (with sub) to do a dynamic stack allocation. 
   - You should store the pointer to your allocated space.
   - Multiple dynamic stack allocations (by %rsp subtraction) work fine. 
   - "leave" will deallocate everything that's been allocated since
     the "enter" call.
 - In C, this is a non-standard "function" called "alloca".

## The Heap

 - Sometimes you don't know how big your data will be.
 - Sometimes structures outlast the function that makes them.

For these circumstances, we can allocate heap memory.

The normal way to allocate memory on the heap is to use the
function "malloc" from the standard C library.

Any memory allocated with "malloc" should be returned to the system
(exactly once) with "free" when it's no longer needed.

## Examples

 - Sum-array stack
 - Sum-array heap
 - Assembly linked list.

## Bonus Example

 - prime.c
 - The point is to explain array <=> pointer relation in C.
 - Remember to show all four variants: arrray, pointer, swap pointer, swap array.

