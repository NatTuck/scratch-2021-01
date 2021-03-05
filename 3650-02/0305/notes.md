
# Homework Questions

 - Maybe more later?

# Topics Today
 
  - The stuff you still need for the HW
  - Maybe introducing Threads

# Review of Data Races, Deadlocks

Conditions for a data race:

 - Concurrent execution, at least two processes/threads.
 - Shared memory which those tasks are accessing,
   read or write.
 - At least one concurrent task must write

Common solution: Add locks

Rules for locks (to prevent data races)

 - Each piece of shared data is protected by a lock.
 - All access to that data (read or write) must happen
   while holding the lock.

Conditions for a deadlock:

 - Circular - If there isn't a cycle, we can't deadlock.
 - Wait - If we don't wait forever, we can't deadlock.

Avoid deadlock:

 - Never hold more than one lock.
 - Lock ordering. 

You can't always avoid deadlock in real systems.

 - So complex systems may need deadlock detection
   and recovery mechanism.

## Semaphores

A semaphore is an integer and two associated operations.

sem_wait: 

 - Attempt to decrement the integer.
 - If this would bring the value below zero, block instead.

sem_post:

 - Increment the integer.
 - If there are processes blocked trying to wait, we
   wake them up to try their decrement again.

With that, we can see how semaphores build a lock:

 - We just start with an initial value of 1.
 - Then it works just like a lock:
   - We can wait once, future attempts block.
 
## Barrier

Barriers are a mechanism to avoid data races in
concurrent programs with shared memory.

They do this by separating parts of the program
in time, to eliminate the conditions for a
data race.

Example:

 - In parallel, writes to an array.
   - Each parallel process writes to a separate
     part of the array.
 - In parallel, reads from the array.
   - Each parallel process reads from the whole
     array.


```
// We run this in parallel on
// nn processes, each with a pid in 0..nn.
void
parallel_work(int pid, int* data, int nn)
{
    // write to the array
    data[pid] = generate_value();
    
    // data[3] is shared memory
    //  - it's read by every process
    //  - at least one process writes: pid == 3 

    barrier_wait();

    // read from the array
    for (int ii = 0; ii < nn; ++ii) {
       process_data(data[ii]); 
    }
}
```

What a barrier actually does:

 - Every process must wait on the barrier
   before any process can execute code
   after the barrier.

## Using a barrier in HW06.

 - There is at least one data race in HW06.
 - All data races in HW06 can be fixed by
   appropriately placed barriers.
   

## Virtual Memory, Shared Memory

```

 How many of these after fork?

   +----------------------+
   |  Stack (grows down)  |  1, but copy on write
   +----------------------+
   |                      |
   +----------------------+
   |  Heap (grows up)     |  1, but copy on write
   +----------------------+
   |                      |
   +----------------------+
   |  Shared region       |  1, shared
   +----------------------+
   |  .data               |  1, read-only
   +----------------------+
   |  .text               |  1, read-only
   +----------------------+
```

# Threads

```
 A process after we spawn an extra thread

   +-----------------------
   |  Main Stack (grows down)   this is shared too
   +----------------------+
   |                      |
   +----------------------+
   |  Second Stack (grows down)  as is this
   +----------------------+
   |                      |
   +----------------------+
   |  Heap (grows up)     |  everything shared 
   |                      |  between threads
   +----------------------+
   |                      |
   +----------------------+
   |  .data               |  still read only
   +----------------------+
   |  .text               |  still read only
   +----------------------+
```
