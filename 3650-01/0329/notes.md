
# Challenge Questions?

How to initialize a shared value:

 - Lazily
 - What if two threads try to initialize
   things concurrently.
 - So you need some mechanism to make sure
   one thread does the one-time initialization
   and other threads don't.

For mutexes, we can just initialize them
staticly (see the manpage).

A place to get started:

 - Global "initialized" flag, staticly set
   to false.
 - We can safely read this without a lock.
 - If it's already been set to true, we're
   initialized.

But if it's not true, we need to figure out
how to safely do the initialization.

Can we do it with a mutex?

 - Everyone who sees false takes the mutex.
 - Once they get the mutex, check again.
   - If still false, we init.
   - If true, we can unlock.
   - If someone else has the lock, we'll
     block on the mutex.
     
.. Probably yes, think it through carefully.


100 meg sequential = 1 s

one random 4k block = 10 ms

How many 4k blocks in 100 MB?

 - 25600 ~ 25k

What's 25k * 10ms = 250s



# Storage Devices

## Hard Disks

 - Kind of slow for sequential access
 - Painfully slow for random access

## SSDs

 - Much faster for random access
 - Random still slower than sequential

## What We'd Really Like

 - We'd prefer to just have non-volatile RAM
 - We can read or write single bytes
 - No penalty for random access
 - As fast as DRAM
 - No limit on read/write cycles

There are a bunch of technologies in the lab
that do this:

 - MRAM
 - Phase Change RAM
 - ...

Intel Optane (3D cross-point)

 - Non-volatile
 - Reads of single bytes
 - Seems to allow writes of single bytes
 - An order of magnitude faster than FLASH, with
   even less random access penalty
 - An order of magnitude more expensive than
   FLASH
 - Avaliable in an SSD form factor
 - Available in a DIMM form factor
 - More read-write cycles than flash, but not
   unlimited


 

   






