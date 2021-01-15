+++
title = "./15-threads-pt2/notes.md"
+++

# Computer Systems

 - New assignment: Same as the last one, except:
   - Threads instead of processes.
   - Normal syscalls instead of mmaped I/O.
   - You're going to need to fix the starter code barrier
     to properly use mutexes / condvars.
 - Questions on the Assignment?

## Threads vs. Processes

Last class we introduced threads. That gives us two ways to support parallel
execution on a Linux system:

 * We can spawn multiple processes with fork()
 * We can execute multiple threads within a single process.

Key difference: With threads, all memory is shared by default.

 - Advantage: Allocating shared memory post-spawn.
 - Disadvantage: 100% data races

### History:

#### Early days

 - Before multi-processor systems parallelism didn't matter.
 - Concurrency was still useful though:
   - Running multiple programs at once.
   - Having multiple logical tasks happening within one program.
 - On Unix style systems, processes were commonly used for concurrency.
 - On early Windows / Mac systems, concurrency within a program was represented
   by cooperative threading:
   - One thread could run at a time.
   - To let other threads run, explicitly call yield()
   - Some systems had an implicit yield when a thread blocked on I/O.
 - By the 90's, systems had some sort of pre-emptive threading. This still didn't
   work in parallel, but it would automatically schedule work between threads 
   without explicit yield() calls.

#### Multiprocessors

 - Multiprocessor servers became widely available in the mid 90's.
 - Windows and Solaris had decent parallel thread support.
 - Linux didn't get fully functional threads until like 2002, so fork() was
   heavily optimized instead.
 - Result: Threads are much more efficient than processes on Windows.
 - Threads under Linux evolved from fork(), so the performance difference
   is small.
 - Multi-core desktop processors showed up around 2005, and suddenly
   parallelism became nessisary for performance.

## Conditon Variables

 - Stack
 - Condvar stack

## Other stuff

These things are a usually bad idea compared to just using mutexes:

 - show atomic-sum101; compare to mutex and parallel versions
 - write it with pthread\_spin\_lock
 - write our own spinlock with atomic\_compare\_exchange\_strong
   - need to google the docs; too new for a manpage
   - bad ideas include sched\_yield




