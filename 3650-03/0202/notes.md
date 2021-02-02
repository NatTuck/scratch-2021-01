
# Homework Questions

 - None of them
 - done: Segmentation Fault Example

# Processes

 * A process is what you get when you run a program.
 * A process is a running instance of a program.

When we run a program, the operating system:

 * creates a process
 * allocates an address space
 * loads the executable file's data into memory
 * starts running the code (at "main")

Address space diagram for a process (assume 32-bit):

```
4G +-------------
   |
   +-------------
   |  stack
   +...(grows down)...
   |
   +...(grows up)...
   |  heap
   +-------------
   |  .data
   +-------------
   |  .text
   +-------------
   |
 0 +-------------
```

# How to calculate how much space to allocate with enter

```asm
my_function:
    push %r12   // Count the pushes
    push %r13
    ...
    enter $0, $0  // if there are even pushes
    enter $8, $0  // if there are odd pushes
    // if you actually need to allocate space
    // to use with enter, adjust accordingly
    //
    // Allocating to the stack:
    // nn is 0(%rsp)
```

# Rules for malloc

 - Calling malloc(nn) gets you a pointer
   to nn bytes of space you can use.
 - Before the program exits, you must call
   free on that pointer exactly once.
 - Definitely don't free twice. It's "undefined
   behavior".










