---
layout: default
---

# Computer Systems

## First: Homework Questions

## Second: Challenge 01: Shell

# Part 1: Solving HW04

## Linked List

 - Linked lists allow for a neat optimization: shared structure.
   - Freeing one list freed the elements from the other list.
 - Solutions:
   - Copy the entire second list.
   - Reference counting.
   - Garbage collection.

## Sequence Collections

We now have three data structures for sequences:

 - Arrays
   - Problem: Doesn't know size
   - Problem: Changing size is hard
 - Vectors / Dynamic Arrays
   - Solves the problem with arrays
   - Slightly more complex
 - Linked list
   - Prepend is easy
   - Pointers everywhere
   - Random access is hard
   - Shared structure is tempting, but hard

# Part 2: Shell Concepts

## File I/O

Progress from fgets /  to pure syscall I/O.

## Fork & Exec

Cover simple example.

 - A running instance of a program is a process.
 - Fork *copies* a process.
 - Exec loads a binary and replaces the contents of a 
   running process, like some sort of horror movie.


## Overflow: Calc Eval

 - Code from last week.
 - Build an AST + eval func with tagged struct nodes.

