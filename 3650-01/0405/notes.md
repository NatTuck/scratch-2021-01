
# Homework Questions



# File Systems Review

Start with a hard disk or SSD or similar.

Provides an interface like this:

 - read_block(int lba) => {512 byte block}
 - write_block(int lba, {512 byte block})

We want to build:

 - A heirarchical structure of files and
   directories.
   - Files are leaves in a tree.
   - Directories are nodes that can have
     children.
 - We always have a root directory.

Another way to look at this:

 - Map<String,FsObj>
 - The string is a path 

## The FAT filesystem

Directory tree to test:

 - C:\
   - BOOK\
     - CHAPTER1.TXT (800B)
     - CHAPTER2.TXT (1600B)
   - COMMAND.COM (2800B)

It's like 1996, we've got a computer with MS-DOS
and a FAT32 filesystem.

We'd like to be able to store this tree of FS objects.

Kinds of objects:

 - Files
 - Directories

For this scenario, a directory is just a table
(array of structs) with:

 - A name (8 characters + 3 character extension)
 - Type of object (file or directory)
 - Location where the first data block for this
   object is (cluster #)
 - Size in bytes
 - Other metadata (e.g. hidden flag)

The filesystem is layed out in three parts:

 - Superblock (1 512 byte block)
   - Number of clusters
   - Cluster size
   - root directory ptr (cluster #)
 - File Allocation Table
   - Array of 32-bit numbers
   - Each entry corresponds to a data cluster
 - Data clusters
   - Blocks for storing data
   - Size is set in the superblock
   - This example: 1024 byte clusters
   - Every file is allocated at least one cluster
     for its data, can't allocate smaller chunks
     than this

Let's say we have a 101MB hard disk.

 - Cluster size = 1K = 1024B
 - How many clusters? ~100,000 clusters
 - FAT has 100,000 entries

Disk layout:

 - Superblock (512B)
 - FAT (100,000 entries * 4 bytes each) (400kB)
 - Data (100,000 blocks * 1024B each) (100MB)

Superblock:

 - Root directory is cluster #0

FAT:

 - 0: END-OF-LIST     # root dir
 - 1: END-OF-LIST     # book dir
 - 2: 3               # command.com
 - 3: 4               # command.com
 - 4: END-OF-LIST     # command.com 
 - 5: END-OF-LIST     # chapter1.txt
 - 6: 7               # chapter2.txt
 - 7: END-OF-LIST     # chapter2.txt
 - 8..100k: FREE

Cluster 0: Root Directory

   NAME    EXT   TYPE   DATA    SIZE
 - BOOK          DIR    1       < 1024B
 - COMMAND COM   FILE   2       2800B

Cluster 1: Book Directory
   
   NAME     EXT   TYPE   DATA    SIZE
 - CHAPTER1 TXT   FILE   5       800B
 - CHAPTER2 TXT   FILE   6       1600B

Cluster 2-4: Command.com

 - file data..

Cluster 5: Chapter1

 - file data
 
Cluster 6-7: Chapter2

 - file data

Fragmentation:

 - As files are added and deleted, free space
   ends up being in small non-consecutive
   blocks.
 - Large new files get spread around on the disk
 - In practice, you'd use a "defrag" tool to
   reallocate data clusters to be contiguous. 

Maximum file size:

 - Fat32 is limited to 4GB files
 - Can't specify a number bigger than that
   in 32 bit int (for size in directory)

Maximum disk size for FAT32:

 - 2TB with 32K clusters
 - That's the limit for an unsigned int storing
   a LBA for 512B blocks

## The ext filesystem

Basic layout:

 - Superblock
 - Bitmaps: Inode, Datablock
 - Inodes
 - Datablocks (4k each)

Each filesystem object (file, directory, symlink, etc)
is represented by:

 - An inode
 - Data is in 0+ data blocks

Superblock contain:

 - Size and location of all the pieces
 - Inode# of root directory

For the homework you might not need a superblock at all.

 - Think carefully for each thing that would go
   in the superblock:
   - Will this actually vary, or can I hard code it.
   - e.g. The root directory is always inode #0

Each inode contains:

 - owner user ID
 - size in bytes
 - creation, modification type
 - mode (integer, with two sub fields)
   - object type (e.g. directory, regular, symlink)
   - permissions 
     (for user, group, other: read? write? execute?)
 - number of hard links
 - array of 12 data blocks
   - 10 direct pointers (1024 * 4k = 40k)
   - 1 indirect (1 * 1024 * 4k = 4MB)
   - 1 double-indirect (... = 4GB)

Directories contain a map of:

 - file name (max length 64, stored variable length)
     to
 - inode #

Example:

 - /
   - /tmp
     - hello.txt (1k)
     - cat.jpg (17k)

Superblock:

 - Root inode is #0

Inode bitmap

 - 1110 0100 0000 0000 

Data bitmap

 - 1111 1001 0000 0000

Inodes:

 - 0: type: d, data: [0], ...
 - 1: type: d, data: [3], ...
 - 2: type: f, data: [1], ...
 - ...
 - 5: type: f, data: [2,4,7], ...

Data blocks:

Block 0: Root directory:

 - "tmp" => 1

Block 1: hello.txt

Block 3: /tmp directory

 - "hello.txt" => 2
 - "cat.jpg" => 5

Blocks 2, 4, 7: cat.jpg







