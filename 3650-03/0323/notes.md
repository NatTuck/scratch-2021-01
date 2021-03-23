
# Allocator Questions?

```
  +------------------------
  | addr = 110
  +------------------------
  | addr = 100, size = 10
  +------------------------
```

# Today: Higher Level Solutions for Concurrency and Parallelism

Problem: Parallelism

 - Modern computers have multiple cores.
 - To take advantage, we need to write parallel programs.
   - Parallelism is non-optional for CPU-limited 
     programs where performance matters.
 - By default, that means threads => shared memory
   => data races => your program is wrong.

Problem: Concurrency

 - Some programs are logically concurrent, even if
   they don't need a parallel speedup.
 - Example: network chat server
 - Alice and Bob connect to the server. We don't
   know who will send a message first.
   - ```bob.block_waiting_for_message()``` is wrong 
 - As we get a lot of concurrency, performance may
   start to matter, and we may want that parallel
   speedup.
 - Threads allow us to handle concurrency easier
   by writing each piece as simple sequential code.
 - But... threads => shared mem => data races => etc

## Primitives 

The hardware gives us atomic instructions.

 - Most importantly, ```atomic_compare_and_swap```.

Those let us build mutex, cond var, semaphore, etc.

Even those are pretty low level. We can build 
anything that the hardware physically allows,
but mostly we're going to tend to build things
that are wrong and slow.

Some modern language impose specific constraints
that help by supporting patterns other than
the normal mutex patterns.

Conditions for a data race:

 - Concurrent execution of multiple threads
 - Shared memory
 - At least one thread must write

Conditions for a deadlock:

 - Circular
 - Wait

## Plan A: Concurrency without Parallelism

Example platform: JavaScript

Browser:

 - Managing a GUI
   - Multiple controls, we don't know what order
     the user will hit them in.
   - Cant ```btn1.block_wait_for_click()```
 - Async Network requests
   - Send request to server
   - Response comes back after unknown delay
 - Solution: Event loop

Web servers:

 - Network messages come in from multiple users
   in arbitrary order.
 - Reuqests to app services (e.g. a DB)
 - Again, event loop.
 - But... you need parallelism here.
   - On a server, you can always just fork() and have
     two processes with two event loops.

## Plan B: Eliminate Shared Data

Example language: Go

Go is basicaly C with:

 - Lightweight threads ("goroutines")
 - Built-in message passing
 - (Garbage collection)

Go suggests communication by message passing.

 - Every object is exclusively accessed by one thread.
 - Sending an object changes which thread has 
   exclusive access.

## Plan C: Make shared data immutable

Example: Rust

Rust enforces a concept of "ownership":

 - Every object has a single owning reference.
 - At any given time you can do one of:
   - You can borrow a single mutable reference to 
     the object
   - You can borrow multiple read-only references
     to the object

## Plan D: Ban Everything

We'll use a functional programming language.

Everything is immutable. Once objects are 
constructed, they can't be changed.

Example: Erlang (aka Elixir)

Erlang programs are structured as a collection
of lightweight processes. Communication between
processes is by messing passing. Logically, nothing
is shared. Optimization: Stuff is immutable, so
nobody can tell if we do share.

Erlang was designed for reliability, for building
telecom switches in the late 80's.

 - High uptime requires hardware redundancy.
 - Hardware redundancy means you're building
   a distributed system.
 - Distributed systems are concurrency + 
   parallelism + no concept of "simultaneious"
   all in hard mode.

So the developers had to solve concurrency and
parallelism, in the 1980's, that's:

 - Before multi-core processors.
 - Before the web.
 
Problem: Erlang has awful syntax.

Solution: Elixir

## Plan E: Accept the races, detect and fix it later

Specifically, with Software Transactional Memory

With transactions, you try generating a new version
of the shared data, detect conflicts, if so we roll
back the changes and try replaying the transaction.

Example Platform: Clojure

Transaction Advantages:

 - No data corruption from data races.
 - No deadlock (no waiting)
 - Don't need to lock to read.

Transactoin disadvantaes:

 - Need to handle rollbacks / replays.
   - Side effects in transactions may happen multiple
     times.
   - Formally, side effects not allowed in transactions.
 - Slow transactions my be delayed forever by fast
   transactions that invalidate their input.






