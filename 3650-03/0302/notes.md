
# Challenge 1 Questions

 - int pipe(int pipefd[2]);
 - Fills in the array with two file descriptors:
   - read from pipefd[0] to get the stuff you
   - wrote to pipefd[1]
 - To implement the | operator, you need to
   do the pipe syscall, and then make sure
   that for "foo | bar":
   - foo has output redirected to pipefd[1]
    -bar has input redirected from pipefd[0]

# New HW: HW06

Write a parallel sorting program.

## Algorithm: Sample Sort

Paralellizing Merge Sort

    60 80 10 70 50 20 40 30

 - First, we split the array in half.
    
    [60 80 10 70] [50 20 40 30]

 - Second, we recursively merge-sort the two
   halves.
 - The two recursive calls can be done in
   parallel in separate processes.
    
    [60 80] [10 70] [50 20] [40 30]
    
    [60] [80] [10] [70] [50] [20] [40] [30]

 - Once we get to the base case, we return from
   the recursion.
 - After the recursive call, we do the actual
   work - merging the sorted arrays.

    [60] [80] [10] [70] [50] [20] [40] [30]
    [60 80] [10 70] [20 50] [30 40]
    
 - We can do this in parallel.

    [60 80] [10 70] [20 50] [30 40]
    [10 60 70 80] [20 30 40 50]

 - Final merge is sequential.

 - O(n) / 1 + 
 - O(n) / 2 +
 - O(n) / 4 +
 - ...
 - O(n) / n
 
 - Merge sort gets some parallel speedup until we
   get up to around (log n) processes, at which point
   adding more doesn't really help much.

Parallel Quicksort

    60 80 10 70 50 20 40 30

 - First, pick a pivot; partition into two parts
   around pivot. For this example, we'll take
   the last # and we'll put it in the right half.

   [10 20] [*30* 60 80 70 50 40]
  
 - Recursively quicksort both halves.
   
   [10] [*20*] [30] [*40* 60 80 70 50]
   
   [10] [20] [30] [40] [60] [*50*] [80 70] 
   [10] [20] [30] [40] [60] [50] [80] [*70*] 

 - If we do it like this, it's the same analysis
   as merge sort.
   - The initial parititon step is a sequential
     O(n) operation, and after that each subsequent
     level of recursion can double the number of
     processes.

But we can do a bit better with a couple of
observations:
   
 - We can partition into more than two parts.
   - We may be able to even do this in parallel.
 - After we partition, we can do local sequential
   sorting with perfect parallelism and no more
   communication.
 
This leads to Sample Sort.

 - If we have P processes, we'd like to partition
   the input into P equal size paritions.
 - Once we do that, we can just have each process
   sort locally.
 - Problem: How do we get equal split? We'd need to
   pick the perfect pivots. 
 - Getting perfect pivots is easy:
   - First we sort the data.
   - Then we take equally spaced values as pivots.
   - Example: If we have the numbers 1-100 shuffled,
     we can sort them and then take 10,20,30..,90
     as our pivots.
   - But if we're sorting to get started, that doesn't 
     help us sort more efficiently.
 - In sample sort, we assume that a random sampling of
   some of the input data (e.g. 1%) will give us pivots
   that are close enough to the perfects pivots.

So the steps are:

 - Randomly sample some of the data
 - Sequentially sort it
 - Pick out (P-1) perfect pivots.
   - These are equally spaced in the sorted data,
     every N/P slots.
   - If N = 1000, P = 4
   - And we sample 100 items (sample),
   - Then we sort the sample
   - Pivots are sample[25], sample[50], sample[75]
 - Fork P times to create P child-labor processes.
 - Partition our data on those pivots.
   - Do this in parallel in the worker processes. 
 - Each process sorts one partition sequentially

# Data Races & Deadlocks

A data race can happen if:

 - There is concurrent execution.
   - e.g. processes, threads
 - There is shared memory that's actually read
   or written by the concurrent processes
 - At least one concurrent process writes to the
   shared memory.

A data race means we don't know what order things 
happened, and therefore we don't know what values 
are stored in memory or registers or what our
program is going to do - it's entirely
non-deterministic.

We need some sort of solution for this problem, 
otherwise any concurrent program we write is
simply incorrect.

Most common solution to data races is to add
locks.

Using locks:

 - Each piece of shared data must be associated
   with a lock.
 - Before accessing the shared data, lock the lock.
 - When done with a single logical operation on
   the shared data, unlock the lock so other
   processes get a turn accessing the shared data.

Locks are implemented using atomic instructions.
See the textbook.

## Deadlock!

Cycle in waiting for resources.

Most commonly, one process is trying to lock a 
lock that another process is holding, when the 
other process is waiting a lock that the first
is holding.

Any time we have more than one lock in a program
we're at risk of deadlock.

Plans to avoid deadlocks:

 - Plan A: Never have more than one lock in your
   program.
 - Plan B: Never lock more than one lock at a time
   in the same process.
 - Complex plan: Apply a global lock ordering, require
   that locks always be taken in order.

Problem: These are all rules that need to be manually
enforced by the programmer. If you've got a team of
programmers, these have to be enforced via policy.

class Foo {

}

 x = new Foo();
 y = new Foo();

 // thread a
 x.lock();
 y.lock();
 
 // thread b
 y.lock();
 x.lock();


