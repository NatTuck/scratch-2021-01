

# CS3650, Section 1

# Old Instructor: Ferd Veselly
# New Instructor: Nat Tuck


# Homework Questions?

 - mmap gets you a pointer to the
   beginning of the file
 - you need to get two things out:
   - long (size)
   - array of floats
 - once you've calculated the correct
   pointers, you can cast to the right thing
   ```float* data = (float*) void_ptr_where_floats_are;```

# Review of Data Races and Deadlock

A data race is when two processes are accessing
shared data, a write occurs, and we don't know
what order things happened in. That means we
don't know what data we're working with anymore,
and so our program is unpredictable.

Conditions for a data race:

 - Concurrent execution (of at least two processes/threads)
 - Shared memory
 - At least one process must write

Common solutoin for a data race is a lock.

To use locks:

 - Every piece of shared data must have
   an associated lock.
 - Every time we access the shared data (read or write)
   we must be holding the lock.
   - Access process locks before access.
   - Unlock when we're done.

Conditions for Deadlock:

 - Circular
   - If we can't have a cycle, we can't deadlock.
 - Wait
   - If we can't wait forever (i.e. locking has a
     timeout) that could solve deadlocks too.
     
## Virtual Memory Layout + Concurrency

```text
 After we fork
 +------------
 | stack (grows down)       1 copy, but Copy-on-Write
 +------------              quickly we have two copies
 | 
 +------------
 | heap (grows up)          1 copy, but COW
 +------------
 | 
 +------------
 |  Shared memory from mmap    1 copy, shared
 +------------
 |  .rodata               1 copy, read-only
 +------------
 |  .text                 1 copy, read-only
 +------------
```

What about with threads?

```
 After we spawn a second thread
 +------------
 | stack (grows down)   read-write, potentially shared
 |   address 7254
 +------------          
 | 
 +------------
 | stack 2 (grows down)   read-write, potentially shared
 |   ptr to 7254
 +------------          
 | 
 +------------
 | heap (grows up)      read-write, potentially shared
 +------------
 | 
 +------------
 |  .rodata           read-only
 +------------
 |  .text             read-only
 +------------
```

# Parallelism vs. Concurrency

Parallelism is a hardware thing, when two things
are actually happening at the same time.

Concurrency is a conceptual model thing, when 
you can't assign an order to two events happening.

# Threads vs. Processes

We have two ways to get concurrency on Linux:

 * We can call fork() and get another process.
 * We can call pthread_create and get another thread.

Key difference: With threads, all memory is shared
by default.

 - Advantage: Can still allocate shared memory
   after spawning thread.
 - Disadvantage: 100% data races

## History

### Way back in time
 
  - Before multi-core processors, parallelism didn't matter.
  - Concurrency *did* matter though.
    - Running multiple programs at the same time
      initially just for multiple users.
  - On Unix-style systems, processes were used for
    concurrency.
  - Processes on UNIX were always pre-emptively scheduled,
    because there's no reason to think the programs that
    the physics majors are writing will do something
    silly like call "yeild()"

### Personal Computers

  - Graphic user interfaces are sort of innately
    concurrent.
  - Multiple applications run in multiple windows.
  - One application does more than one thing at a time.
    - Your photo editor both runs the "blur" function
      and listens for you pressing the cancel button.
  - This innate need for concurrency within one program
    lead to threads.
  - Both threads and processes were cooperative on
    early GUI/PC systems. Every thread needed to
    occasoinally call "yeild()" to check for events
    or the whole UI would freeze up.

## Multi-processor machines

 - Multi-processor servers became available in the
   late 90's.
 - How to do parallelism?
 - Windows and Solaris had good parallel
   thread support.
 - Linux didn't get fully functional parallel
   threads until like 2002.
 - So people used processes for parallelism.
   - The Apache webserver would fork() on each
     incoming web request.
   - fork on Linux became *hyper* optimized.
 - Processes were never that well optimized 
   on Windows.
 
### Results

 - Threads are much more efficient than processes
   on Windows.
 - Threads under Linux are based on fork(), so
   they're a little slower than on Windows.
 - Full fork() is only a little slower than that.
 - If you want parallelism/concurrency with
   cross platform support, it's resonable to
   default to threads.
   
   

   
