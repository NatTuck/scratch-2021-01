+++
title = "./20-os-kernels/tour-notes.md"
+++

 
## xv6: An Operating System

References: 

 - Repo clone: git clone http://github.com/NatTuck/xv6.git
 
Building and running it:

 - open dedicated terminal
 - make
 - make qemu-nox
 - ls
 - cat README
 - can exit with halt

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



