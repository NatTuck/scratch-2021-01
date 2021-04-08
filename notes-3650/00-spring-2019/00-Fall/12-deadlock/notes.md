---
layout: default
---

# Computer Systems

## HW Questions?

## Review Data Races

Conditions for a data race:

 - Concurrent execution of two processes / threads.
 - Shared data that both processes access (read or write)
 - At least one process writes

Locks and Data Races:

 - A lock stops concurrent execution right when a data race would otherwise
   occur.

## Deadlocks

 - Real work example
 
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

## Using Semaphores

 - Sem-queue example
 - This is way less safe than just using locks. Requires careful analysis.

## Virtual Memory and Fork: A Review

 - Draw the virtual memory diagram.
 - Allocate some shared memory.
 - Fork.
 - Point out that shared memory is shared, and non-shared writable memory
   soon isn't.

