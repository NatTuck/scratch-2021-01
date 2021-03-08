---
layout: default
---

# First: Project Questions
 
## OTP: Processes & State

```
# Can compile / recompile from iex
> c("stack.ex")
```

The problem:

 - Stack of integers: push, pop, print
 - More than one stack at a time.
 - Every five seconds, If the top integer is odd, add 2.
 
## Supervisors and Supervision Trees

**stack0.ex**

 - Review manual state process.
 
**stack1.ex**

 - Use the built-in GenServer behavior.

**stack2.ex**

 - Add a Registry
 - What do we do when we want a bunch of processes?
 - We can register by arbitrary term with the Registry module.
   
**stack3.ex**

 - Add a Supervisor
 - Supervisor: Start & Monitor processes, potentially restart them when they crash.
 - In this case, DynamicSupervisor

## Supervisor Concepts

**Supervision Tree**

 - Supervisor module takes a static list of children and keeps them running.
 - DynamicSupervisor handles workers spawned at runtime.
 - Spin up hangman & show tree in observer.
   - iex -S mix phx.server
   - :observer.start()
   - install and use kmag to zoom in

**Links?**

If a child is linked to a parent, the parent gets an exit message when the
parent dies. 

**Trapping exit?**

A process that gets an exit message will exit itself unless it's configured to
trap exits. Trapping exits is what lets supervisors work.

## Multiplayer Hangman

 - Current Hangman design:
   - User connects to channel.
   - Game state is stored in channel and backed up to agent
   - When a move comes in, the next state is produced by channel process.
 - Two-player Hangman:
   - Two users each connect to the same channel.
   - They each get their own, seperate, channel process.
   - We can't store or update the state in the channels, because then
     the game states could get out of sync.
   - Solution: Add a process to manage the game state.
     - Channel processes sends move messages to game process.
     - Game process broadcasts updated state to the channel.
   - If the process crashes, the game state will be lost.
   - Could keep the backup agent as well.

Let's try building that. We could add turns, but instead we'll just allow either
player to guess whenever they want.

Process:

 - Write Hangman.Server
 - Write Hangman.GameSup
 - Add Hangman.GameSup to the application child list.
 
Test:

```
$ iex -S mix
iex> Hangman.GameServer.start("foo")
iex> Hangman.GameServer.guess("foo", "l")
iex> Hangman.GameServer.guess("foo", "z")
iex> Hangman.GameServer.guess("foo", "b")
```
