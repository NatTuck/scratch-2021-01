
# xv6 hw questions

# file system review

Disk provides the following interface:

(LBA is logical block address)

 - read_block(int lba) -> {512 byte block}
 - write_block(int lba, {512 byte block})

To build:

 - A traditoinal filesystem structure
 - Files
 - Directories
 - Nested directories
 - Metadata

## FAT Filesystem

Our directory tree:

 - C:\
   - BOOK\
     - CHAPTER1.TXT (800B)
     - CHAPTER2.TXT (1600B)
   - COMMAND.COM (2800B)

Traditional FAT32, as if for MSDOS.

Kinds of filesystem objects:

 - Directories
 - Regular Files

In MSDOS, a directory just contains a
table (array of struct) of directory entries,
each with:

 - name (fixed 8 bytes)
 - file extension  (fixed 3 bytes)
 - type of object (file or directory)
 - data size
 - starting data cluster
 - other metadata (not much, like "hidden")

The file system is layed out in three sections:

 - The superblock (1 512-byte block)
   - number of clusters
   - cluster size
   - cluster # for the root directory
 - File Allocation Table
   - array of 32-bit integers
   - each integer corresponds to a data
     cluster
   - can be one of:
     - 0  = empty
     - 1  = reserved
     - other = next data cluster # in sequence
     - -1 = end of list
 - Data Clusters
   - Where file data is stored, or directory
     data for directories.

Example: 101MB hard disk

 - 100MB of data clusters
 - clusters are 1KB each
 - How many clusters? 100K
 - How many bytes of FAT? 400K

Superblock:

 - Cluster # of root: 0

File Allocation Table:

 - 0: END-OF-LIST     # root directory 
 - 1: END-OF-LIST     # book directory
 - 2: 5               # command.com 1/3
 - 3: 7               # chapter2    1/2
 - 4: END-OF-LIST     # chapter1
 - 5: 6               # command.com 2/3
 - 6: END-OF-LIST     # command.com 3/3
 - 7: END-OF-LIST     # chapter2    2/2
 - 8..100K unused

Data Clusters

 - 0: root directory
 - 1: book directory
 - 2: command.com
 - 3: chapter2
 - 4: chapter1
 - 5: command.com
 - 6: command.com
 - 7: chapter2

Directory: C:\

   NAME     EXT     TYPE    SIZE  CLUSTER#
 - BOOK             DIR     64B     1
 - COMMAND  COM     FILE    2800B   2

Directory: \BOOK

   NAME     EXT     TYPE    SIZE  CLUSTER#
 - CHAPTER1 TXT     FILE    800B    4
 - CHAPTER2 TXT     FILE   1600B    3

## ext file system

Basic disk layout:

 - Superblock
   - format-time metdata
   - inode# for root directory
 - Bitmaps
   - inodes
   - data blocks
 - Inodes
 - Data blocks (4k each)

Each filesystem object is represented by

 - 1 inode
 - 0+ data blocks

Inode contains:

 - metadata
   - owner user ID
   - timestamps
   - ...
 - size in bytes
   - in ext1, this is a 32-bit integer
 - mode (one int, split into two fields)
   - object type (e.g. file, directory, symlink)
   - permissions (ugo X rwx)
 - # of hard links
 - array of 12 data block indexes
   - 10 direct pointers  (10 * 4k = 40k bytes)
   - 1 indirect pointer  (1 \* 1k \* 4k = 4M bytes)
   - 1 double-indirect pointer (1k \* 1k \* 4k = 4G bytes)

Directory is:

 - A map from filename (string) to inode # (32-bit int)
 - Filenames are variable length, so this can't be a
   simple array-of-struct
 - This may not be in sorted order, and the structure
   may not allow efficient random access.

Example:

 - /
   - /tmp
     - hello.txt (1k)
     - cat.jpg (17k)

Superblock:

 - root directory inode # is 0

Inode bitmap:

 1110 0100 0000 0000

Data block bitmap:
 
 1111 1111 0000 0000

Inodes:

 - 0: data blocks: [3] # root directory
 - 1: data blocks: [0] # hello.txt
 - 2: data blocks: [1] # tmp directory
 - 5: data blocks: [2,4,5,6,7]

Data blocks:
 
 - 0: /tmp/hello.txt: "hello, there"
 - 1: tmp directory
 - 2: cat
 - 3: root directory
 - 4..7: cat

Directory: /

 - "tmp" => 2

Directory: /tmp

 - "hello.txt" => 1
 - "cat.jpg" => 5

# finish the fs slides
