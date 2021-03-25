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



