---
layout: default
---

# Computer Systems
 
 - New Homework
 - Let's take a look at xv6

## xv6: An Operating System

 - Clone the xv6 repo.
 - git fetch --tags
 - git checkout xv6-rev8
 - git checkout -b xv6-rev8

At this point, we have the same version as the NEU-local docs.

References: 

 - Repo clone: git clone http://github.com/mit-pdos/xv6-public
 - Local docs: http://www.ccs.neu.edu/course/cs3650/unix-xv6/
 
Building and running it:

 - open dedicated terminal
 - make
 - Comment out the error line.
 - make
 - make qemu-nox
 - ls
 - cat README
 - need to kill qemu to exit

Boot process:

 - bootasm.S
 - bootman.c
 - main.c
   - main
   - mpmain
 - proc.c: scheduler
 - stop here
   
Stuff we've already done:

 - usermalloc.c
   - start in morecore
   - uses sbrk
   - uses 4k chunk size
   - look at malloc
   - freep is free list
   - note: return p+1
   - look at free
   - note: return p-1
 - sh.c
   - builds a parse tree
   - executes it
   - neat example of tagged polymorphic struct usage

Stuff we will do:

 - quick look at fs.c
 - we'll have too many slides on this later

Cool stuff:
 - vectors.S
 - trap.c
   - Interrupts!
   - Timer
   - IDE (hard disk)
   - Keyboard
   - Serial port
 - echo.c
   - The echo command.

## IPC: Sockets

 - Pipes
 - Named pipes.
 - UNIX sockets.
 - IP sockets: UDP or TCP
   - UDP: Unreliable packets; basically just IP.
   - TCP: Like a pipe, but might go to another machine.
 - TCP server:
   - socket() -> gives us an FD
   - bind()   -> specifies address
   - listen() -> marks us as a server
   - accept() -> accepts connection, gives new FD
   - now we can send / recv data like read / write to a pipe
   - shutdown() -> signal end of data
   - close()  -> clean up fd
 - TCP client:
   - socket()  -> gives us an FD
   - connect() -> connect to a server
   - now we can send / recv data like read / write to a pipe
   - shutdown() -> signal end of data
   - close()  -> clean up fd

All of these mechanisms are slower than mmaping shared memory,
because they require copying data and system calls to read/write
messages. But... sockets are pretty fast. And socket code is
way easier to get right than shared memory.

 - Take a look at get.c



