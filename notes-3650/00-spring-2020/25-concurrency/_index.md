+++
title = "./25-concurrency/notes.md"
+++

# Computer Systems

Last couple lectures:

 - Today: 
   - COW FS commentary
   - Solutions for Concurrency and Parallelism
 - Next Tuesday is our final lecture, where we'll do quick semester review.

## Solutions for Concurrency and Parallelism

The problem (parallelism):

 - Modern computer have multiple cores.
 - To take advantage, we need to write parallel programs.
   - Parallelism is non-optional for programs where performance matters.
 - By default, that means threads => shared memory => data races.
 - So by default, all parallel programs are wrong.

The problem (concurrency):

 - Some problems are logically concurrent even though there may not
   need parallelism for performance.
 - Example: Network chat server.
 - Alice and Bob connect to the server. The server doesn't know which
   user will send a message first, so it needs to listen to both.
 - Having a thread for each connection allows the program to be written
   in a straightforward way, but threads => shared memory => data races.
 - And when we have a *lot* of connections, performance matters again,
   and it seems reasonable that a concurrent program can get a parallel
   speedup.

## Primitives & Abstractions

How do we write correct concurrent & parallel programs?

 - The hardware gives us atomic instructions.
 - Those let us build mutexes & condvars or semaphores.
 - But even with those, our programs are probably wrong and slow.

The primitives are nice because they make it possible to write any program that
the hardware allows. But to make our programs correct, we need to add
constraints - either by convention, or in the tools we use.

Some modern languages pick impose specific constraints and concurrency patterns
that help.

Conditions for a data race:

 - Concurrency + premption or parallelism. 
 - Shared data.
 - Mutable data.
 
 Conditions for a deadlock:

  - Hold and wait
  - Circular dependency
  - mutual exclusion with no pre-emption

If we eliminate any of those conditions, we can avoid the problems.


## Plan A: Concurrency without Parallelism

Example Platform: JavaScript

Stuff JS programs need to handle:

Browser:

 - User clicks a button.
 - Request data from a server, do something when the response comes in.
 - Trigger a movie to start playing.

Server:

 - Accept incoming request, perform database query, send response quen query finishes.

Thoughts:

 - These things are all innately concurrent. We don't know when they'll happen.
 - These things are fast to process. Most of the time the program is waiting for
   some external event to occur.
 - We don't need parallelism - we can use a sequential event loop.

Demo: show javascript/sample.html


## Plan B: Eliminate Shared Data

 - Communicate by message passing.
 - Sending a message transfers "ownership" from one thread to another.

Example language: Go

No Go demo, but Go is one system where message passing in shared memory
without immutability is promoted as the default solution.

Draw picture:

 - Threads 1 and 2 have a shared channel ("message queue").
 - Thread 1 allocates object on shared heap (ptr on stack -> heap)
 - Thread 1 sends ptr to thread 2.
 - By convention, Thread 1 doesn't keep a pointer.
 - No data races (outside the message queue).

## Plan C: Make Shared Data Immutable

Example Language: Rust

Example: rustsum/src/main.rs


## Plan C: Ban Everything

Example Platform: Erlang (aka Elixir)

We can eliminate data races entirely by making data all data immutable. Once an
object is created, it cannot be changed.

Erlang programs are structured as a collection of lightweight "processes".
Communication between processes is by message passing. Because data is
immutable, it's safe to pass pointers to shared data as messages - although
Erlang can also be run distributed across multiple machines, in which case
messages are copied.

In the distributed context, mutation doesn't really make sense anyway.
Mutating the local copy can't effect a remote copy of the "same" object.

This model is great for concurrency, and great for executing concurrently
structured programs in parallel for a speedup. It's not the greatest for
parallel speedup though - Erlang runs in an interpreter, and mutation tends to
be pretty good for fast computation.

Erlang's main design goal is reliability. If some piece of the system crashes,
another piece (potentially on another machine) can notice and restart it.

Example: elixir / demo.ex - startPrinter, startSender


## Plan E: Accept Races, Fix Later with Transactional Memory

Example Langauge: Clojure, a LISP on the JVM.

Like Erlang, it takes the immutability path to deal with concurrency, but
instead of message passing it has a concept of "refs", which are mutable
references to immutable data.

Refs can be updated transactionally. Rather than avoiding data races,
transactions detect them and roll back / replay any transaction that ran on old
data.

Transaction Advantages: 

 - No data corruption from data races
 - No deadlock
 - No mutual exclusion needed for values that aren't written to during a transaction.

Transaction Disadvantages:

 - Need to handle rollbacks / replays. If transactions have side effects, those
   side effects may happen multiple times.
 - Slow transactions can be delayed pretty much forever by faster transactions
   that invalidate their inputs.

Transactions are the same strategy that databases use for concurrent updates.

Example: clojure / tmem.clj


## Another Approach: Data Parallelism

Example Technology: OpenCL

OpenCL is a programming system for building programs that run on graphics
cards. Graphics cards, or GPUs, are a bit different from regular CPUs. Rather
than having one processor with a couple cores, they have a bunch of
"processors", each with hundreds of "shader units". A shader unit is basically
 a single vector ALU - something that can execute arithmetic instructions on
4 or so values in parallel.

On a GPU, it's perfectly reasonable to plan to execute 2000 additions in
parallel in one clock cycle.

The trick is that GPUs really like to perform the *same operation* in parallel.
In fact, each individual processor can generally only load one program to run
on its hundreds of shader units.

So instead of the basic addition operation adding together two numbers, on a
GPU it adds together two arrays. The arrays generally represent mathematical
vectors or matrices, but that's just a mental model. Anything where you want
to operate on entire arrays at once will work great on a GPU.

This programming model of performing the same operation in parallel on many
different values (elements of the array) is called data parallelism. It's
required on GPUs, but it's common on supercomputers too. When you have a
cluster of 1000 PCs, it's easier to think about them working together on one
array calculation than to reason about them individually.

Example: opencl/tpose/*

