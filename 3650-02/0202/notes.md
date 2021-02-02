

# Homework 2 Questions?

# Today: Processes

A process is what you get when you run a program.

 * It's a running instance of a program.

When we run a program:

 * ```$ ./args``` # in shell
 * Click on launcher button in GUI

Operating system creates a process.

 * Create a memory address space 
 * Reads executable file on disk.
 * Load pieces of the executable into memory
 * Create some other memory regions in the new
   address space.

Assembly programs have sections, e.g. .text, .data

When assembled into an executable file, these sections
exist there too.

 * .text section is executable machine code
 * .data section is the data from the static
   data section (i.e. strings and stuff)

When the OS loads up our executable, these end up
loaded into memory.

Our new program gets an address space that looks
something like this:

(Assume 32-bit machine)

```
 4G +----------
    |
    +----------------
    |  stack
    +...(grows down)....
    |
    +...(grows up)...
    |  heap
    +----------
    |  .data
    +----------
    |  .text
 0  +----------
```

```
// Another way to get stuff in our data section
    .data
.space 10, 35   // 10 bytes, each containing the number 35
```
 

# Argv in assembly

argv is an array of char*, aka char\*\*

In C, "pointer" and "array" are frequently interchangable.

Therefore,

```
argv[1] // is of type "char*" (aka "string" or "null terminated
   array of char")
argv[1][1] // is of type "char"
``` 

So in assembly:

 * You can get argv[1] with a fixed offset mov.
 * To get ```argv[1][1]```, you'll need to first
   load ```argv[1]``` then do a second load.

 
 
