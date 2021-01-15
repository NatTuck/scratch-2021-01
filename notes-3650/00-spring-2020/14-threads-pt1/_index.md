+++
title = "./14-threads-pt1/notes.md"
+++

# Computer Systems

## HW Questions?

## Review Data Race & Deadlocks

Conditions for a data race:

 - Concurrent execution of two processes / threads.
 - Shared data that both processes access (read or write)
 - At least one process writes

Locks and Data Races:

 - A lock stops concurrent execution right when a data race would otherwise
   occur.

Conditions for a deadlock:

 - Circular wait.

Avoiding deadlock:

 - Never hold more than one lock.
 - Lock ordering.

## Semaphores

A semaphore is an integer and two associated operations.

sem_wait: Attempt to decrement, but if this would bring the value below zero,
          block instad.

sem_post: Increment the semaphore. If there are processes blocked on the
          semaphore, give them a chance to try decrementing.

So if we start with a value > 0, that allows sem_wait to restrict access to
a resource to that many processes. For the specific value 1, this is a lock.

If we start with a value of zero, processes waiting on the semaphore block
immediately and can be woken up by a post operation.

## Barrier

 - Review the barrier code.
 - Figure out how it stops data races.

## Virtual Memory and Fork: A Review

 - Draw the virtual memory diagram.
 - Allocate some shared memory.
 - Fork.
 - Point out that shared memory is shared, and non-shared writable memory
   soon isn't.

## Introducing Threads

 - show create.c
 - Discuss how threads change the virtual memory story.

## Using Semaphores

 - Sem-queue example
 - This is way less safe than just using locks. Requires careful analysis.

## Insufficiently careful analaysis of sem-queue

Initial model: queue array has infinite size

 - qii stores the index where we will insert the next item
 - isem stores the number of logically free slots in the queue
 - Inserting is safe: qii is atomicly incremented after each insert, so
   it always points to a never-before-used slot.
 - qjj stores the index where we will extract the next item
 - qjj is incremented after each extract, so it will always point to the next
   item that hasn't been extracted
 - osem stores the number of full slots; initially zero
 - osem is incremented only after an insert, so decrementing it is guaranteed
   to provide access to a slot that has been written its single time

Modification: Queue is finite size.

 - qii and qjj are used MOD(size). This causes slots to be reused both for writing
   and for reading as the counter wraps around.
 - We need to guarantee that reuses never overlap with the previous use.
 - At most (size) items can be in the queue at once. This guarantees that no
   matter where the queue data starts in the buffer, it can never overfill by
   wrapping around.
 - A read can never wrap around either, since that would require an osem value
   greater than size.
 - We have an invariant that isem + osem <= size. Why?
 - Does that demonstrate that a read can never wrap around to a write?
 
Modification: Integers are finite size.

 - Unsigned integers wrap on overflow.
 - As long as the buffer size evenly divides into the integer range, then
   this isn't a problem.
 - Why?

## More with Threads

Now we have two ways to support parallel execution on a Linux system:

 * We can spawn multiple processes with fork()
 * We can execute multiple threads within a single process.

Key difference: With threads, all memory is shared by default.

 - Advantage: Allocating shared memory post-spawn.
 - Disadvantage: 100% data races

With threads, we can get broken behavior even easier than before.

 * Show thread sum101 examples.

