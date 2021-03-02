
# Challenge 1 Questions?

 - Fork gives you two processes, so everything
   after that will happen twice.
   - This includes printing buffered data, so
     you may want to fflush() before fork.
     - In some edge cases you may want
       to use _exit
     - Remember to usually never use _exit,
       mostly it breaks stuff.
   - If you don't want a child to finish out
     the program's code, it needs to exec
     or exit.
 - Parentheses mean subshell, so you want
   nodes in the AST for them, and you want
   to fork for each subshell.

# Homework 6

## Sample Sort

 - A parallel version of Quicksort

Simplest sort to visualize parallelizing: MergeSort

   10 70 40 80 20 50 30 60

 - First, we split the input in half.

   [10 70 40 80] [20 50 30 60]

 - Then, we recursively mergesort the halves.
 - For parallel mergesort, we can do this in
   two separate processes.

   [10 70] [40 80] [20 50] [30 60]
 
 - We keep going until we have trivial arrays.
 
   [10] [70] [40] [80] [20] [50] [30] [60]

 - On the way back up the recursion, we do the
   actual work of sorting when we merge the
   results of the two recursive calls
   
  {[10] [70]} {[40] [80]} {[20] [50]} {[30] [60]}
  [10 70] [40 80] [20 50] [30 60]

  {[10 70] [40 80]} {[20 50] [30 60]}
  [10 40 70 80] [20 30 50 60]
  
  {[10 40 70 80] [20 30 50 60]}
  [10 20 30 40 50 60 70 80]

Parallel cost of merge sort

 - Merge is O(n)
 - At each level, that can be parallelized over a different
   number of processes.
   
 - O(n)/1
 - O(n)/2
 - O(n)/4
 - ...
 - O(n)/n

 - log n levels
 - Don't really get a decent speedup after ~log(n)
   processors.

Quicksort

 - First, we partition the array into two pieces.
   - Select last item as pivot.
   - Split array around pivot, leaving pivot
     as first item of second half.
 
   10 70 40 80 20 50 30 60
   {10 40 20 50 30} {*60* 70 80}

 - Then, we recursively sort the two halfs
   
   [10 40 20 50 *30*] [60 70 *80*]
   [10 20] [*30* 40 50] [60 70] [*80*]
   [10] [20] [30 40] [50] [60] [70] [80]
   [10] [20] [30] [40] [50] [60] [70] [80]
   
 - O(log n) / 1 +
 - O(log n) / 2 +
 - O(log n) / 4 +
 - ...
 - O(log n) / n

Quick sort observations:

 - We can use more than two partitions.
 - Once we partition, we never need to do any post
   processing that requires consideration of the
   rest of the array.
 - If we split into P partitions (where P is our number 
   of processors) then we can sort in parallel with no 
   further communication.
 - Problem: For perfect speedup, the split needs to be
   perfect.

Idea: Sample Sort

 - We could get a perfect split by sorting the data
   and then taking the perfect pivots.
 - (e.g. if there are 100 items and P = 10, then
    good pivots would be sorted_items[10, 20, 30, 40,
    50, 60, 70, 80, 90]).
 - Unfortunately, sequentially sorting to setup for
   parallel sort misses the point.
 - But, we can get nearly as good results by sampling
   a portion of the input (e.g. 1%), sorting that, and
   taking the perfect pivots for the sample.
   
 - This means we can fork (P times) once at the beginning
   and then each process can locally do a sequential sort.

# Today: Data Races and Deadlocks

Conditions for a data race:

 - There must be concurrent execution.
   - Multiple threads or processes running
     logically "at the same time".
 - There must be shared memory (or some other similar
   shared resource).
 - At least one process must write to the shared
   memory.

To avoid a data race, avoid one of those conditions.

Lock pattern:

 - Every piece of shared data should have an associated
   lock.
 - Whenever that data is accessed by a process (reads 
   *and* writes), that process should first lock the 
   lock.
 - When the process is done with the atomic work
   it wants to do, it should unlock so others get
   a turn.
 - This avoids the data race by avoiding concurrent
   access to the shared memory.

Problem: Deadlock

 - If we have more than one lock, we need to 
   worry about deadlock.
 - A deadlocked program just stops. No log messages,
   no errors, just stuck, forever.

Solutions to deadlock:

 - Don't have more than one lock.
 - Never lock more than one lock at a time.
 - Lock ordering: Always take locks in some constant
   global.
 - Bad news: All the solutions require programmers
   to impose constraints on how programs are written
   and then enforce those constraints perfectly.





