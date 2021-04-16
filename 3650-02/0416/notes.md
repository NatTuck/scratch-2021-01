
# Challenge 3 Questions


# Extra Credit


# Persistent Tree Example

"Persistent" data structures

 - Logically immutable
 - If you want to update the structure, you
   create a new version based on the old one
 - The process of making a new version doesn't
   change the old version.
 - Making a new version is efficient in time
   and space because the unchanged portions of
   the structure can be shared.

We're going to build a binary search tree
like this.

Rotations in binary trees:

```
                5
              /   \
             3     (7)
           /  \    /  \
         (2) (4)   6   8

Rotate right, new root is left child (3)

                3
               / \
             (2)  5
                 / \
               (4)  (7)
```


# Exploit Slides
