
# First: Homework Questions

# Today: Hash Tables

Ways to build a key-value map.

 - Association list
   - Operations are O(n)
 - Tree (e.g. balanced binary tree)
   - Operations are O(log n)
 - Hash Tables
   - Operations are expected, amortized O(1)

Hash Table Structure

 - Table is implemented as array of struct

```
struct hash_table {
   row* data;
   long size;
   long cap;
};

struct row {
    T key;
    T val;
};

long
hash(long x)
{
    return 5 * x;
}

// hash(key) % cap is index
```


```
 { size: 3, cap: 8 }
 +-------+------+-------+---------+
 | index | key  | value | empty?  |
 +-------+------+-------+
 |  0    |      |       | 
 |  1    |      |       | 
 |  2    |      |       |
 |  3    |      |       | 
 |  4    |   4  |  40   | 
 |  5    |      |       | 
 |  6    |      |       | 
 |  7    |  11  |  110  | 
```

Insert pairs into table:

 * 10 => 100
 * 11 => 110
 * 12 => 120
 * 13 => 130
 * 14 => 140 
 *  4 =>  40 

What do we do when we get a collision:

 * Plan A: Chaining
   * Rather than storing k/v pairs directly in
     the table, we store a linked list of k/v pairs.
 * Plan B: Probing
   * If our first choice slot is full, have a plan
     for what slot to try next.
   * Simplest is linear probing, where we try the
     next slot next.

Load factor:

 * If we let the hash table get full, things may break.
 * If we let the table get near full, operations start
   to slow down.
 * In order to avoid that, we grow our table early.
 * Load factor is (size / capacity).
   * Max load factor for chaining is usually 1.
   * Max load factor probing is usually 0.5.

Deletes:

 * Can add a tomb flag to each slot
 * Can move back subsequent slots until blank

## Optimizations

Hash tables are at least somewhat about performance.

Bitwise operation modulo:

 - If our table has a power of 2 capacity.
 - Modulo can be:
   - mask = cap - 1
   - slot = hash(x) & mask
 - Normal modulo requires divide, which is the
   slowest arithmetic instruciton by a lot.

Embedding key/data:

 - Following pointers requires memory loads.
 - Memory loads are slow, because they're sometimes
   cache misses.
 - Data structures with data packed all in one block
   tend to be faster than ones with pointers.

Best hash function:

 - Better hash functions mean less collisions.
 - It's worth googling for good hash functions,
   at least for produciton code.

Best collision avoidance scheme:

 - There are a ton of options.
 - Linear probing is the best simple option.


 









