
# Tonight: Memory Allocator

# In a Week: Challenge 2

Any questions?

```
   +---------------------
   | address = 110
   +---------------------
   | address = 100, size = 10
   +---------------------
```

Two places that insert into the free list:

 * free
 * malloc, when there's a leftover piece from
   a split block

# Today: Solutions for Concurrency and Parallelism

The problem: Parallelism

 - Modern computers have multiple cores.
 - To take advantage of that, we need to write
   parallel programs.
   - Parallelism is non-optional for CPU limited
     programs where performance matters.
 - Default solution: threads => shared memory
   => data races => your program is wrong.

The problem: Concurrency

 - Some programs are logically concurrent even 
   though they may not care about parallelism
   for performance.
 - Example: network chat server
 - Alice and Bob connect; server may get a message
   from either alice or bob at at any time.
   - ```alice.block_listening_for_message()```
   - We need to listen for messages from both
     concurrently.
 - Having a thread for each connection means we
   can write straightforward sequential code.
   - But that means threads => shared memory =>
     data races => etc
 - And when we have a lot of connections, performance
   may matter here two.

## Primitives and Abstractions

The hardware gives us atomic instructions.

 - Most relevently, atomic-compare-and-swap.
 - This can be easily used to build a mutex,
   or cond var, or semaphore.
 - Unfortunately, even with those tools our
   program is probably wrong and slow.

Locks => Deadlocks

With these primitives, we can write any *legal*
program, but to safely (and "fearlessly") make 
programs concurrent we need to add more 
restrictions.

Some modern programming languages impose other
constraints that allow for other patterns that
may work better to allow us to write parallel
or concurrent programs.

Conditions for a data race:

 - Shared memory
 - Multiple concurrent processes
 - At least one process must write to the shared
   memory

Conditions for a deadlock:

 - Multiple processes
 - Circular
 - Wait

## Plan A: Concurrency without Parallelism

Example Platform: JavaScript

Browser:

 - We're driving a GUI
   - We don't know what order the user is going to
     interact with the controls.
   - We can't say ```btn1.block_waiting_for_click()```
 - We're making asynchronous network requests 
   - If we send a request to a server, we don't
     know when the response will come back.

Server:

 - We need to be able to handle requests from
   multiple users.
 - We don't know what order they'll come in.
 - On a server, we can generally get parallelis
   by running multiple processes even with a
   sequential language.

## Plan B: Eliminate Shared Data

Example Platform: Go

Go is basically C, with built in support for

 - Lightweight threads (Goroutines)
 - Message passing
 - (Garbage collection)
 - (Weird design choices)

Go docs say: Communicate by passing messages

The general pattern is that once one Goroutine 
sends an object to another, ownership is transferred. 
Only one goroutine ever accesses an object at once.

## Plan C: Make shared data immutable

Example: Rust

In Rust, the compiler enforces a concept called
"ownership":

 - Every object has a single owning reference.
   - When the owning ref goes away, the object
     is cleaned up.
 - You can have only one of:
   - A single borrowed mutable reference.
   - Multiple borrowed read-only references.

## Plan D: Ban Everything

Use a functional programming language.

In a functional language, all objects are immutable.
Once an object is created, it can't be changed.

Example: Erlang (aka Elixir)

An Erlang program is a collection of lightweight
"processes". There's no logically shared memory
between them, they communicate by message passing.

Optimization:

 - Objects are immutable.
 - There's no way to distinguish a copy from a shared
   refernence.
 
Erlang was developed for reliability. Specifically,
to build telecom switches.

To get crazy reliability, you need to survive
hardware failure. You need redundant hardware and
fail-over.

This means they were dealing with distributed
systems programming. So in 1989 - before anyone
considered multi-core processors or even the
web - the Erlang developers needed to solve the
basic problems of parallelism and concurrency.

Erlang itself has... awful syntax.

But the Erlang virtual machine is uniquely great.

Elixir language targets the Erlang VM.

# Plan E: Allow the data race, detect and fix it

Example language: Clojure

Clojure supports Software Transactoinal Memory

 - Once process tries to modify the state by
   executing a transaction.
 - If there's a conflict with another transaction,
   the transactoin may be aborted, rolled back, and
   retried.

Transaction advantages:

 - No data corruption from data races.
 - No deadlock 
 - No mutal exclusion needed for values that
   are only read.
 
Transactoin Disadvantages:

 - Need to handle rollbacks / replays.
   - If transactons have side effects, they may
     happen multiple times.
   - Formally, you hsould have no side effects.
 - Slow transactoins may be delayed forever by
   faster transactoins that invalidate their inputs.
 











