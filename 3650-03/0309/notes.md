
# Homework Questions?


# More on Threads

With threads, we now have two ways to
do parallelism and concurrency.

 - We can call fork() and spawn a proces.
 - We can call pthread_create and spawn
   a thread.

Key difference: With threads, all memory
is shared by default.

 - Advantage: We can allocate (or reallocate)
   new shared memory.
 - Disadvantage: 100% of what's in memory
   could be subject to a data race

## Parallelism and Concurrency

Parallelism is a hardware thing. Two processes
run in parallel if they're actually running
simultaniously on two seperate pieces of
hardware (e.g. CPU cores). This potentially
gives us parallel speedup.

Concurrency is a conceptual design pattern
sort of thing. Two events are concurrent if we
can't assign a order to when they happened. When
we don't want to enforce a specific sequence of
events, we may need to have software specifically
designed to be concurrent.

## History

 - Before multi-processor systems, parallelism
   didn't matter.

### UNIX Minicomputers - maybe 1975

 - Concurrency was added to support multiple
   concurrent users.
 - The initial mechanism to support this concurrency
   was processes.
   - We want virtual address spaces to avoid bugs
     in my program crashing your program.
   - The OS will do pre-emptive scheduling, one program
     hogging the CPU will be stopped by the OS to give
     other programs a turn.

### Personal Computers, especially with GUIs

 - Early Windows / Mac systems handled concurrency
   with co-operative thread systems.
   - Every thread that's doing work needs to occasionally
     call "yeild()" to give other threads a turn.
   - Even multiple programs running at the same time
     was best modeled as threads - there wasn't
     modern memory protection.
     
### Multiprocessors

 - Multi-processor servers were readily available
   by 1997.
 - Windows and Solaris had good parallel thread
   support.
 - Linux didn't have parallel threads until 2002.
 - Linux was the common operating system for
   web servers, with the Apache webserver.
   - Every request to an apache webserver
     triggered a fork() and then was processed
     in the child.
   - So fork() was hyper-optimized on Linux
 - When Linux eventually got parallel threads,
   they were based on fork()

# So, today

 - Threads on Windows are fast.
 - Linux threads are slightly slower.
 - Linux processes are only very slightly
   slower than that.
 - Windows processes are slow in comparison.
 
 - So by default, cross platform parallelism
   probably means threads.
 - Exception: If you want memory separation.

 - As of 2005, processors are multicore.
 - Today, you can't even get a single core cellphone
   much less laptop or desktop.
 - So programs that are CPU-limited but not parallel
   are probalby wrong.

# Programming language Tier List

D. Perl
F+. Rust
F. Every other language






