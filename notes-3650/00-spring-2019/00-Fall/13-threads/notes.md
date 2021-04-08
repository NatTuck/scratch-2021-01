---
layout: default
---

# Computer Systems

 - New assignment: Same as the last one, except:
   - Threads instead of processes.
   - Normal syscalls instead of mmaped I/O.
   - You're going to need to fix the starter code barrier
     to properly use mutexes / condvars.
 - Questions on the Assignment?

## Threads

 - Show create.c
 - Show sum101 with threads & mutexes.
 - Show sum101 with separate sums.

## Threads vs. Processes

 - Draw virtual memory diagram for a process, then fork.
 - Draw virtual memory diagram for a process, then spawn a thread.
 
Key difference: All memory is shared by default.

 - Advantage: Allocating shared memory post-spawn.
 - Disadvantage: 100% data races

## Conditon Variables

 - Stack
 - Condvar stack

