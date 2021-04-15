
# Questions on Challenge 3?


# Questions on Extra Credit?


# Today: Tree Data Structure

 - Persistent Binary Tree

"Persistent" here means that:

 - The data structure is logically immutable.
 - Rather than changing it you create a new
   version.
 - Creating the new version doesn't modify
   the old version.
 - Creating a new version doesn't require a
   complete copy - we have shared structure.
   
## Rotations on a binary tree 
 
```
                5
              /   \
            3       (7)
          /   \    /   \
        (2)  (4)  6     8
     
Rotate so our left child (3) is new root.
        
                3 
              /   \
            (2)    5
                  / \
                (4)  (7)
```








