
# Extra Credit

# CH03 / XC01 Questions?

 - CH03 disk size is still 1MB
 - Uptime timestamp with "touch"
 - There are no rules about valid values
   for offset or size.
 - read(..., offset=3291, size=8999) is valid

# Persistent Binary Search Tree

A persistent data structure:

 - Logically immutable
 - You can create a new, modified version based
   on the old version.
 - In doing that, a persistent data structure
   is efficinet in time and space due to shared
   structure.
 - When you create a new version, the old version
   is still valid (and unmodified).

Tree rotation

```
                5
             /      \
            3       (7)
          /   \     /  \
        (2)   (4)  6    8

Rotate right, left child is new root (3)

                3
              /   \
            (2)    5
                 /   \ 
               (4)   (7)

```


# Exploit Slides
