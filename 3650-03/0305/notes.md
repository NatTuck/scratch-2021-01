
# Today

 - Review of data races & deadlock
 - What's a barrier?
 - Homework questions
 - Threads maybe?
 - Confusing semaphore example

# Data Races & Deadlock

Conditions for a data race:

 - Two threads (or processes) executing concurrently
 - The threads access (either read or write)
   shared data
 - At least one thread must write to the shared data

Standard solution: Locks

Rules for using locks to avoid data races:

 - Every piece of shared data has an associated lock.
 - Any thread (or process) accessing that data must
   first lock the lock.
 - Unlock the lock when done accessing shared data.

Once we have two or more locks, we need to worry about
deadlock.

What are the conditions for deadlock?

 - Circular - If there can't be a cycle, no deadlock.
 - Wait - If threads don't block waiting for a lock,
          no deadlock.

Common ways to avoid deadlock.

 - Never lock more than one lock at a time.
 - Enforce lock ordering.
 - If lock attempts time out, that can avoid deadlock,
   but the logic gets complicated and the performance
   might be bad.

## Semaphores

A semaphore is an integer with two associated operations:

sem_wait:

 - Try to decrement the integer.
 - If that would reduce the value below zero, instead
   block.

sem_post:

 - Increment the integer.
 - If there's anyone else waiting on the semaphore,
   wake them up so they can try to decrement again.

Simplest use is creating a lock. If the inital value
is one, then sem-wait is lock, and sem-post is unlock.

Semaphores have extra flexibility, you can start
with other values.

 - If you start with 0, then anyone who waits
   blocks until someone else posts.
 - If you start with a value > 1, then more than one
   thread/process can do a sem_wait before we start
   blocking attempts.

# Barriers

A barrier is a tool that we have for synchronizing
between concurrently executing threads/processes to
avoid deadlock.

```
    // Given a fixed number of processes
    // concurrently executing:

    // A: some code
    barrier_wait()
    // B: more code
```

A barrier guarantees that the (B) code doesn't execute
in any process until every process has finished
the (A) code.

How does barrier fix data race?

```
// Code, running in parallel on nn processes.
void
do_work(int pid, long* xs, long nn)
{
    // Here, we write to xs[3]
    xs[pid] = init_value();
    
    // What if we put in a barrier?
    barrier_wait();

    for (long ii = 0; ii < nn; ++ii) {
        // Here, we read from xs[3] in
        // another process
        do_stuff(xs[ii]);
    }
}
```


# Barrier in HW06

 - There's at least one data race in the HW06
   task.
 - Each data race can be fixed by sticking in
   a barrier in the right place.
 - You need to do that.


# Exciting New Topic: Threads

## First, memory layout for process

```
                            How many after fork?
  +--------------------+
  | stack (grows down) |    1, but marked COW
  +--------------------+    so pretty quickly 2
  |                    | 
  +--------------------+
  | heap (grows up)    |    1, but marked COW
  +--------------------+    so pretty quickly 2
  |                    |
  +--------------------+
  | Shared memory      |    1, and it's shared
  +--------------------+
  | .text              |    1, read-only
  +--------------------+
  | .rodata            |    1, read-only
  +--------------------+
```


```
  +--------------------+
  | stack (grows down) | main thread, shared between threads
  +--------------------+
  |                    | 
  +--------------------+
  | stack (grows down) | new thread, shared between threads
  +--------------------+
  |                    | 
  +--------------------+
  | heap (grows up)    | shared between threads
  +--------------------+
  |                    |
  +--------------------+
  | .text              | read-only
  +--------------------+ 
  | .rodata            | read-only
  +--------------------+
```

A thread is a thing with:

 - Some registers
   - Includes %rip
 - A stack
 - The ability to be scheduled for execution
   on a CPU core.

