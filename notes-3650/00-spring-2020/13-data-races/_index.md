+++
title = "./13-data-races/notes.md"
+++

# Computer Systems

## HW Questions?

## New Homework

 - Sample Sort 
 - Parallel Quick Sort
   - Draw quick sort.
   - Easy to parallelize, but first partition is sequential.
   - Can avoid sequential code by sampling instead.

## Data Races

 - Count example
 - Discuss semaphores
   - Locks
 - Second count example

Conditions for a data race:

 - Concurrent execution of two processes / threads.
 - Shared data that both processes access (read or write)
 - At least one process writes

Barriers and data races:

 - Review the barrier code.
 - Figure out how it stops data races.

## Deadlocks

 - Real work example
 
Conditions for a deadlock:

 - Circular wait.

## Using Semaphores

 - Sem-queue example
 - This is way less safe than just using locks. Requires careful analysis.

