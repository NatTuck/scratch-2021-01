

# useState

 - useState is a thing you do in a React
   component to manage state.
 - If the primary source of state is external,
   e.g. in Elixir, we can still use React
   state to trigger re-rendering when an
   external state update occurs.

# HW06: Multi-Player Game

 - Request a team by this Friday.
 - No team assigned by course staff, 
   no submitting HW06.

## Example: A Stack

 - Stack of integers
 - Operations are: push, pop, print
 - We can have more than one stack at a time
 - Every five seconds, if the top int on the
   stack is odd, add 2 to it.

## Problem: Naming processes

 - {:ok, pid} = Stack.start_link
 - What if we want to references processes
   by name?
 - Stdlib solution: Registry








