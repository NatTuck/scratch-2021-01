
# First: Homework Questions

 - HW04

# Tree Maps

Last time we did the most common map data structure: hash maps.

Hash Map properties:

 - Expected O(1) operations.
 - Some operations require an O(n) copy.
 - Dense memory layout; this is fast and space efficient.
 - No fast in-order traversal.

This time we'll look at the next most common: tree maps.

Common structure is a binary search tree:

 - Tree nodes have key, value, and two children.
 - for each node, left child orders <, right node orders >
 - Operations are log2(n) *if* tree is balanced.

BST Tree Map properties:

 - Guaranteed O(log n) operations.
 - Fast in-order traversal.
 - Easy to implement as a persistent data structure for functional-style
   programming.
 - Not great cache behavior.

# Build the Example

 - animals.c again

# Balancing a Binary Tree

 - Inserts and deletes can unbalance a tree.
 - General strategy is to fix it afterwards via "rotations".
 - This tends to result in a ton of cases and to require extra information
   in each node (like number of descendents).

# BTreeMap

 - log2(n) isn't bad, but log32(n) is even better
 - We can get that by having 32 children per node
 - This also results in better cache behavior - linear scan of 16 items tends
   to be faster than following log2(16) = 4 pointers.

# Balancing our Binary Tree

 - We're going to build a bad version of an AVL tree.
 - The common balanced tree is a bit more complex: a Red-Black tree.
 - First, we add a "size" field to our nodes, tracking how many k/v pairs
   are in that subtree.
 - So: Whenever we insert or delete into the child of a node, afterwards
   we check for balance.
 - A node is unbalanced if abs(left.size - right.size) > 1.
 - If we're unbalanced, we rotate to fix the unbalance.
 - To rotate if left child is larger:
   - Old left child (L) is new root.
   - Old root (O) is L's new right child.
   - L's old right child is O's new left child.
 - To rotate if right child is larger:
   - Same, except swap L for R and R for L.
 - This takes O(1) operations per node, and happens to
   O(log n) nodes as we return from our recursive operation.






