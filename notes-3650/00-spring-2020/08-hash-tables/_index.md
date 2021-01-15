+++
title = "./08-hash-tables/notes.md"
+++

# Computer Systems

## HW Questions?

# Hash Tables

Ways to build a key-value map:

 - Association list
   - Operations are O(n)
 - Tree (e.g. balanced binary tree)
   - Operations are O(log n)
 - Hash table
   - Operations are expected amortized O(1)

Draw hash table on board.

 - Pointer to struct.
 - Fields: {size, capacity, data}
 - Data is pointer to array of pair struct.
 - Fields: {key, value}
   - May need more fields depending on embedding (e.g. "used" field).

Key and value types depend on what key values you want
to map to what value values.

We'll use integers for our first example.

## Operations

Insert:

 - Consider growing. If size/capacity exceeds some constant load factor, double
   the capacity of the data array just like with a vector.
 - Hash the key.
   - Your hash function goes (key => int), and selects which
     slot in the data array will be used to store your pair.
   - Hash function should assign "random looking" but deterministic
     integers to keys. We want different keys to have different hashes
     so we don't have too many collisions where multiple keys map to the
     same slot.
   - Hash function should be fast.
   - Example hash function for integers: h(x) = x
 - Pick slot: h(x) % size
 - If slot is empty, put pair in slot.
 - If slot is already full, use some mechanism to handle the collision.
   - First example will use chaining.

Collision handling mechanisms:

 - Chaining: Store a linked list of pairs in each slot.
   - To insert, just cons onto list.
   - Load factor can be high: Maybe grow at LF = 1
 - Linear Probing: Scan forward in data array to find an empty slot.
   - Load factor needs to stay lower: Maybe grow at LF = 0.5

Lookup:

 - Hash the key.
 - Find slot: h(x) % size
 - If slot is full, compare key, if match return.
   - If first key doesn't match, scan possible collisions for match.
     - For chaining, scan whole list.
     - For linear probing, scan forward until empty slot.
 - If no match, return failure.

Delete:

 - Hash the key; find the slot.
 - Scan possible matches
 - If a pair matching key is found, remove.
 - Linear probing problem: Future lookups are now broken.
   - Add "tomb" flag to pair.

Iterate:

 - Scan through items in data array.
 - Return all pairs containing inserted data.

## Optimizations

Hash tables are used for performance, so they're frequently optimized.

Bit twiddling modulo:

 - This is one of the few single-instruction optimizations that's likely
   to actually be worth doing.
 - % requires divide, which takes noticibly more clock cycles than
   most arithmetic operations
 - If your table size is a power of two, modulo can be accomplished
   as follows:
    - mask = size - 1
    - slot = hash(x) & mask
   
Embedding key / data:

 - Following pointers requires a memory load, which can be slow.
 - It's likely to be faster to put small values in the data array
   directly rather than using pointers to these values.
 - 16 bytes or less is definitely "small".
 - Cache lines are 128 bytes, so the benefits get worse above that value.

Best hash function:

 - Picking a good hash function matters a lot.
 - This is worth googling if you're going to write a hash table for any
   application where performance matters.

Best collision avoidance strategy:

 - There are a bunch of other collision avoidance strategies.
 - Linear probing is probabably the fastest simple scheme.

Linear probing complications:

 - On delete, you may be orphaning later items with the same hash.
 - Plan A: tombstone flag
 - Plan B: Rehash items and move them back until next empty or correct slot.

# Building a hash table

We're going to build a (string => string) hash table.

 - See animals.c

# Overflow: Describe Balanced Binary Trees
