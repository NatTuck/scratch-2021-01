
# Homework questions?

 - The marginal hash function is sum
   the characters in the string.

# Today: Tree Map

Hash Map properties:

 - Expected amortized O(1) operations.
 - Some operations require O(n) copy.
 - Dense memory layout; fast and space efficient
 - No fast in-order traversal

Today, we'll look at next most common map
structure: Tree Maps.

Common implementation is a binary search tree:

 - Tree nodes have key, value, two children
 - For each node: left child is <, right child is >
 - Operations are O(log n) if the tree is balanced.

BST Tree Map properties:

 - Guarantee of O(log n) operations.
 - Fast in-order traversal
 - Easy to implement as a persistent data structure
   for functional-style programming
 - Not great cache behavior. 

# Balancing a Binary Tree

Different plans for balancing a binary tree.

 - General strategy: Do change, then fix with
   "rotations".
 - Most popular: Red-Black Trees
   - E.g. ```<map>``` in C++ is usually an R-B Tree.
 - Balancing tends to require extra info in each node.
 - There tend to be a lot of cases for rotations
   e.g. red black trees have 11 cases for delete

```
       C
      /
     B
    /
   A

rotate right to get

      B
     / \
    A   C
```

# BTree Map

 - log2(n) isn't bad, but log64(n) is better
 - Having an array of children can get us that
 - We also get better cache behavior - scanning through
   64 children linearly is better than traversing 6 pointers

Result: Rust doesn't have BinaryTree maps, just BTreeMap






 
 


