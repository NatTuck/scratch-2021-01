
# Homework Questions (xv6)

 - Everyone understands everything.

# Filesystem Slides

Why might a list of blocks not be the best way
to store the collection of blocks associated
with a file on disk?

 - Slow?
   - Tree might be faster
 - Need a variable size data structure.

Question: Are short files or long files more
common, why?

--

We wrote the data and then lost power before
writing the new inode or updating the data bitmap.

 - Is the filesystem consistent?
 - If not, is the file corrupted?
 - Is the filesystem in general corrupted? If futher
   writes occur to this disk, could other files be
   corrupted?

This case

 - Before the list of data blocks was: [4]
 - Now the list of data blocks is: [4]
 - The only block we wrote to was 5.

Update the inode (blocks are now [4, 5]), then crash.
Didn't update bitmap or write block 5.

 - Is the filesystem consistent?
   - Henry says no.
   - Inode disagrees with bitmap.
 - If not, is the file corrupted?
   - File is corrupted, block 5 is junk data.
 - Is the filesystem in general corrupted? If futher
   writes occur to this disk, could other files be
   corrupted?
   - Write a new file, it gets blocks: [5]
   - Then we write to the first file, overwriting
     block 5.

Update data bitmap, but not inode or data block.

 - Is the filesystem consistent?
   - Inconsistent.
 - If not, is the file corrupted?
   - The file is fine, inode didn't change nor
     did it's data block.
 - Is the filesystem in general corrupted? If futher
   writes occur to this disk, could other files be
   corrupted?
   - Everything will work fine.
   - We just lost one block.


Finished with slides at "Extents and BTrees (ext4)"
