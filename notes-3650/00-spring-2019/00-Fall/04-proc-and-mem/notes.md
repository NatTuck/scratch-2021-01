---
layout: default
---

# Computer Systems

## Next HW:

 - Next homework is up: Merge sort

## HW Questions?

## Memory Segments in a Process

 - Code (.text)
 - Global / Static Data (.data)
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
   - If you have non-variable size data, put that first
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

## Command Line Args

```
int
main(int argc, char* argv[]) 
{
    ...
    %rdi: a 32-bit int with # of arguments
    %rsi: The address of an array of addresses of strings.
```

## Examples

 - Sum-array stack
 - Sum-array heap
 - Assembly linked list.

