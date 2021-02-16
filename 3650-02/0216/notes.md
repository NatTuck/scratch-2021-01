
# Homework questions?

```
char*
stdup(char* xx)
{
    long nn = strlen(xx);
    char* yy = malloc(nn + 1); <- this is malloc
    memcpy(yy, xx, nn + 1);
    return yy;
}
```

# Calloc vs Malloc

What is calloc?

```void* calloc(size_t count, size_t item_size);```

Basically the same as:

  - ```void* tmp = malloc(count * item_size)```
  - ```memzero(tmp, count*item_size)```
  - But handles integer overflow better if
    count * item size is a problem.
  - Zeroing memory makes some bugs easier
    to understand.

Only downside to calloc is the cost of zeroing, which
doesn't matter on large allocations which is the only
place it might matter.

```C
struct goat {
   ...
};

goat* goats = calloc(35, sizeof(goat));
goat** goats = malloc(35 * sizeof(goat*));
```

# Tree Maps

Last time we talked about hash maps. They have
some properties:

 - Expected amortized O(1) operations.
 - Some operations require an O(n) copy.
 - Dense memory layout; fast and space efficient.
 - No fast in-order traversal.

Today, we're looking at the second most common
structure: Tree Maps.

Common structure is a balanced binary search tree.

 - Tree nodes have key, value, and two children.
 - For each node, left child orders <, right node orders >
 - Operations are log2(n) *if* tree is balanced.

BST tree map does:

 - Guaranteed log(n) ops
 - Fast in-order traversal
 - Easy to implement as a persistent data structure
   for functional programing.
 - Lots of pointers, so caches hate them.

# Balancing a Binary Tree

 - Inserts and deletes can unbalance tree.
 - Plan: Fix it with "rotations"

```
           B
          / \
         A   C

rotate right to get

      A
       \
        B
         \
          C
```

 - If after an operation there are more descendants
   on the left, we rotate right.
 - If more on right, rotate left.
 - There are a bunch of ways to do this:
   - Red-black trees are probably most popular in
     practice.
   - There are at least 5 other reasonable
     strategies.
 - This tends to involve case analysis with lots
   of cases. I think red-black trees have like
   12 cases on delete.

# BTreeMap

 - log2(n) is good, but log64(n) is better
 - We can get this by having 64 children per node
 - This results in better cache behavior - a linear
   scan of an array of children is better than
   following pointers.
 - Scanning an array of 32 in linear time sounds worse
   than following log2(32) = 5 pointers. But pointers
   might be cache misses.
 - For example, Rust uses this as the default map
   in it's standard library.

# Balancing our binary tree

 - We're going to try to create a bad version
   of an "AVL Tree".
 - The plan with an AVL tree is that each node
   has at most one more child on one subtree than
   the other.




   
   















