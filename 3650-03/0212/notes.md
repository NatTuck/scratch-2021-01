
# Homework Questions?

# Today: Hash Tables

Ways to build a key/value map:

 - Association list
   - Operations are O(n) 
 - Tree (e.g. balanced binary tree)
   - Operations are O(log n)
   - Keep in mind that log(n) < 45
   - log(n) is definitely less than 64
 - Hash tables
   - Operations are expected amortized O(1)

```
struct hash_table {
    pair* data;  // array of pairs
    long size;   // # of pairs in table
    long capacity; // # of slots allocated
}

struct pair {   // <- this is one table row
    int key;
    int val;
}

long
hash(int key)
{
    return 3 * key;
}
```

```
 +-------+-----+-------+
 | index | key | value |
 +-------+-----+-------+
 |   0   |     |       |
 |   1   |  3  |  30   |
 |   2   | 10  | 100   |
 |   3   |     |       |
 
 { size: 4, capacity: 8 }
 +-------+-----+-------+
 | index | key | value |
 +-------+-----+-------+
 |   0   |     |       |
 |   1   |  3  |  30   |
 |   2   | 14  | 140   |
 |   3   |     |       |
 |   4   |     |       |
 |   5   |     |       |
 |   6   |  2  |  20   |
 |   7   |     |       |
```

```
    Insert pairs:
     - 10 => 100
     -  3 =>  30
     - 14 => 140
     -  2 =>  20
     -  1 =>  10
```

Handling collisions: 

 - Chaining: Instead of storing our pair
   directly in the table, we store a linked
   list of pairs in each table slot.
 - Probing: Have a procedure for finding a new
   slot to try when the first slot is full.
   - Most common is linear probing, where we
     just try the next slot.

Load factor:

 - Load factor is (size / capacity).
 - For chaining, the normal maximum load factor
   is 1.
 - For probing, the normal maximum load factor
   is 0.5.

Deletes with probing:

 - Can mark deleted slots with a tombstone
 - Can rehash items probing forward to the
   next empty slot.

## Optimizations

Hash tables are intended to be fast.

Modulo with bitwise AND:

 - If your hash table has a power of 2 capacity.
 - You can do modulo with:
   - mask = capacity - 1
   - slot = hash(x) & mask
 - This is worthwhile because division is
   kind of slow

Embedding key / data in the table:

 - Following pointers require a memory load.
 - Memory load could be a cache miss, cache
   miss costs ~1000 cycles.
   - In comparison, an L1 cache hit can take 15
     cycles.
 - In comparison, linear scans through memory
   tend to be pretty quick.
 - So if we can cram stuff directly into the
   packed table, that's great.
   - char[4] key;

Best hash function:

 - A good hash function is fast to compute
   and avoids collisions.
 - Different functoins work better for different
   key data types.
 - It's worth googling for a well known good
   function if you really care about performance.
  
Best collision avoidance strategy:

 - There's some complicated ones.
 - Best simple one is linear probing.





  
   












