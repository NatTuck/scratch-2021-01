+++
title = "./09-introducing-otp/notes.md"
+++

# First: Homework Questions
 
 - Memory Game with Server-Side State is due today.

## Project 1

 - Project 1: Make a multi-player game.
 - You can pick what game you want to make.
   - Turn based games should be at least as complex as Connect4.
   - Real-time games should be at least as complex as a 2-player
     breakout (think Pong with blocks in the middle).
 - You'll work with a partner.
 - Make sure you've talked to your partner for Project 1 - you want
   to be on a team by tomorrow.
 - Project 1 is due in 2 weeks.
   - If homeworks normally take 8 hours, that means your team has
     at least 36 hours to spend on this project.
   - For students who claim that homework takes 23 hours...

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

 - Manual state process.
 
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
