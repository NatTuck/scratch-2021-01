
# First: Homework Questions

 - Do we ever free small allocations?
   - Not until the process ends, and then 
     implicitly.


# Then: Bonus Topic - Solutions for Concurrency and Parallelism

## Problem 1: Parallelism

 - Modern computers have multiple cores
 - To take advantage, we need to write parallel
   programs. 
   - Parallelism is non-optional for CPU-bound
     programs where performance matters.
 - By default, that means threads => shared memory
   => data races.
   - Therefore, by default, all parallel programs
     are wrong.

## Problem 2: Concurrency

 - Some programs are logically concurrent, even if
   they don't need a parallel speedup.
 - Example: Multi-user network chat server.
 - Alice and Bob connect to the server.
   - They can both send messages at any time.
   - We don't know what order they'll send messages in.
   - A natural way to solve the problem is one thread
     for each connection.
     - We can write straightforward sequential code to
       handle each user.
     - When we want to commncate between threads,
       we're back in shared memory => data races => wrong
       program.
   - Once we get a lot of connections, performance matters,
     and having a parallel speedup is good.

## Primitives & Abstractions

How do we write correct concurrent/parallel programs?

 - The hardware gives us atomic instructions.
 - The key one for fixing this sort of problem is
   atomic compare and swap.
 - Those let us build mutexes and condition variables.
 - Even with that, our programs are probably wrong
   and slow.

The primitives are nice - they let us write any program
that the hardware allows. 

To make our programs correct we need to add constraints 
- either by convention or enforced by the tools we used.

Some modern languages impose specific constraints and
concurrency patterns that help us write correct parallel
or concurrent programs.

Conditions for a data race:

 - Multiple concurrent processes
 - Accessing the same shared data
 - At least one must be writing

Conditions for a deadlock:

 - Holding a lock
 - While waiting to acquire a different lock
 - Possibility of a cycle

## Plan A: Concurrency without Parallelism

Example Platform: JavaScript

In Browser:

 - User clicks a button
 - Triggers request to a remote server
 - Maybe something happens here
 - Later a response comes back and needs to be processed.
 - GUI or Network would be enough to have
   concurrency.

Server programs:

 - Accept incoming requests
 - Perform DB query - may take time to return
 - Need to get parallel speedup for multiple requests

Thoughts:

 - We definitely have concurrency here
 - Most of the stuff we're doing is pretty quick though
   - we may not need real parallelism, we're just
   routing/handling simple events.
 - We can get away with just a simple, sequential, event
   loop.
   
## Plan B: Eliminate Shared Data 
 
 - Communicate between threads by passing messages.
 - Sending a message transfers ownership (and exclusive
   access) from on thread to another.
 
Example Platform: Go (no actual code example)
  
 - Go has built-in message passing.
 - "Communicate by message passing"

Timeline:

 - Thread 1 owns object.
   - Thread 1 creates object.
   - Thread 1 modifies object.
   - Thread 1 sends object to Thread 2
 - Thread 2 owns object.
   - Thread 2 modifies object.

## Plan C: Make shared data immutable

Example language: Rust

Rust explicitly enforces a set of rules about
ownership of objects.

 - Every object has a single owning reference.
 - We can either:
   - Take multiple read-only references.
   - Take one mutable reference.
 - Therefore, if there are multiple references to
   an object then they're read only references.
   - If it's writable, it can't be shared.
 
## Plan D: Be fully Functional 
 
Example platform: Erlang (aka Elixir)

We can eliminate data races by making all data
immutable. Once an object is created it cannot
be changed.

Erlang programs are sturcutred as a collection
of lightweight "processes". 

 - Communication between processes is by message passing.
 - Because data is immutable, we can safely pass
   pointers that remain shared.

The point of the Erlang design is reliability. 

 - Reliability means hardware redundancy.
   - One machine breaks, automatically fail-over
     to another.
 - Hardware redundancy means a distributed system.
 - Distributed means concurrent + parallel. 
 
## Plan E: Accept the race, fix it later with Transactional Memory

 - Databases have a good solution for data races:
   - Try making the change
   - Detect possible data races
   - If there was a data race, roll back the change
     and retry
 - We can use this same plan for objects in memory.

Example Language: Clojure

 - This is another functonal language, using immutability
   to avoid data races.
 - To update a shared value, we replace the entire old
   value with a new value.
   - This means replacing a refernece with another
     reference.

Clojure has the concept of a "ref", or a potentially
shared refernce that can be updated transactionally. 

Transaction advantages:

 - No data corruption from data races.
 - No deadlock
 - No mutual exclusoin needed for values that aren't
   written to during a transaction.

Transaction disadvantages:

 - Need to handle rollbacks / replays.
   - If transactions have side effects, those may
     happen multiple times.
 - Slow transactions can be delayed indefinitely
   by faster transactions that invalidate their input.


