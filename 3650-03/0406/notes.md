
# quetions on xv6 hw

# review: FAT and ext

LBA is Logical Block Address

Basic interface:

 - read_block(int lba) -> {512-byte block}
 - write_block(int lba, {512 byte block})

## review: FAT

Example disk contents:

 - C:\
   - BOOK\
     - CHAPTER1.TXT (800B)
     - CHAPTER2.TXT (1600B)
   - COMMAND.COM (2800B)

We're doing FAT32 on MSDOS.

The filesystem provides a mechanism to
store filesystem objects. Types:

 - Regular Files
 - Directories

On MSDOS, a directory is a table (array of struct)
of directory entries, each containing:

 - name (8-byte fixed width)
 - file extension (3 bytes)
 - type of object (file or directory)
 - size in bytes
 - first data cluser #
 - other metadata (e.g. hidden flag)

File system layout:

 - superblock (one 512-byte block)
   - metadata from format time (e.g. cluster size)
   - cluster # for root directory
 - File Allocation Table
   - Array of 32-bit integers
   - One integer per data cluster
   - Stores metadata about that cluster.
   - Possible values:
     - empty (0)
     - reserved (1)
     - cluster # of next cluster in file
     - end-of-list (-1)
  - Data clusters

Concrete example:

 - 101MB hard disk
 - 100MB of data clusters @ 1KB each
   - 100k clusters
 - How many bytes does our FAT take? 400kB

Superblock:

 - Root directory cluster # = 0

File Allocation Table:

 - 0: END-OF-LIST       # root directory (128B)
 - 1: END-OF-LIST       # CHAPTER1
 - 2: 7                 # CHAPTER2    1/2
 - 3: 5                 # COMMAND.COM 1/3
 - 4: END-OF-LIST       # BOOK directory (128B)
 - 5: 6                 # COMMAND.COM 2/3
 - 6: END-OF-LIST       # COMMAND.COM 3/3
 - 7: END-OF-LIST       # CHAPTER2    2/2
 - 8..100k: empty
 
Data Clusters:

 - 0: root directory
 - 1: ch1
 - 2: ch2
 - 3: c.com
 - 4: BOOK directory
 - 5: c.com
 - 6: c.com
 - 7: ch2
 - 8..100k: unknown, unused

directory: C:\

    NAME    EXT     TYPE    SIZE    CLUSTER#
 -  BOOK            DIR     ??      4
 -  COMMAND COM     FILE    2800    3

directory: C:\BOOK

    NAME    EXT     TYPE    SIZE    CLUSTER#
 - CHAPTER1 TXT     FILE    800     1
 - CHAPTER2 TXT     FILE    1600    2

## review: ext

Disk layout:

 - Superblock
   - Root directory inode #
   - other metadata
 - Bitmaps
   - inodes
   - data blocks
 - inodes
   - one inode per filesystem object
 - data blocks (assume 4k)

Each inode contains:

 - random metadata:
   - owner
   - timestamps
   - ...
 - size in bytes
   - 32 bit integer
 - mode (an integer, split into two parts)
   - fs object type (e.g. file, directory, symlink,etc)
   - permissions
 - # of hard links
 - array of 12 data block pointers
   - 10 direct (10 * 4k = 40kB)
   - 1 indirect (1 * 1k * 4k = 4MB)
   - 1 double-indirect (1 * 1k * 1k * 4k = 4GB)

Directories contain:

 - Map of filename (string) to inode# (32-bit integer)
 - This is simply a list of {string, int} pairs.
 - Tightly packed, not sorted, all operations are
   O(n).
 - hello.txt\0[4 bytes of int]hello2.txt\0[4B of int]

Example:

 - /
   - /tmp
     - hello.txt (1k)
     - cat.jpg (17k)

Superblock:

 - Root directory inode # = 0

Inode Bitmap:

 1011 0100 0000 0000

Data block bitmap:

 1111 1111 0000 0000 

Inodes:

 - 0: blocks: [2]   # root directory
 - 2: blocks: [0, 3, 5, 6, 7]  # cat.jpg
 - 3: blocks: [1]   # tmp directory
 - 5: blocks: [4]   # hello.txt
 
Data blocks:

 - 1: temp directory
 - 2: root directory

Directory: /

 - "tmp" => 3

Directory: /tmp

 - "hello.txt" => 5
 - "cat.jpg" => 2

# slides

# hw10 starter code



