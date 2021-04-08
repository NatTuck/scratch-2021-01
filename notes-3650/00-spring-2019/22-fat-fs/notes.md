---
layout: default
---

# Computer Systems

## Disk Hardware

Three kinds of HW to consider:

 - Hard Drives
 - Flash SSDs
 - Ideal NVRAM

### Hard Drives

Draw diagram

 - Platter (can have several)
 - Track
 - Sector (e.g. an octant)
 - Read Head

Old Addressing mode: {Platter, Track, Sector, Block}

New addressing mode: LBA (blocks simply numbered 0-N)

Time for a request:

 - Seek time: Time to move the read head. 5-10 ms
 - Rotational latency: Time to rotate to the right block.
   * 7200 RPM = 4ms average (do the arithmetic)
   * 15k RPM  = 2ms average
 - Max transfer is maybe 150 MBPS
 - How long does it take to transfer 150 MB of random 4k blocks?
   * 6ms to move read head + 4 for disk = 10
   * 150M / 4k = 37.5k
   * So 375 seconds just for seeks.

Blocks:

 - Traditionally 512 bytes
 - A more reasonable value would be 4096 bytes
 - Both of these values are common today.
 - sudo blockdev --getbsz /dev/sda1 (or /dev/nvme0n1p1)

Block reads / writes are atomic:

 - Guaranteed to either happen fully or not.
 - If the system loses power, the platters are used as flywheels to run the
   motor as a generator to finish out the write (or there are capacitors, which
   is less awesome)

Optimizing for hard disks:

 - Reads and writes are about the same speed.
 - We really want sequential access. Seeks *kill* performance.
 
Hard disk scheduling:

 - If we have multiple requests in the queue, reordering them can improve performance.
 - Example:
   * Head is at track 0.
   * We have read requests in queue for [10, 5, 8].
   * Sorting the reads by track minimizes seek time.
 - Hard disks and disk drivers do this. This will mess us up later - we can't rely on
   writes occuring in a specific order.


### Flash SSDs

Modern SSDs are based on NAND flash. It's quite a bit faster than spinning hard disks,
but it's got some weirdness.

 - Written in 16k "pages". Page must be clean to write.
 - Erased (dirty -> clean) in 256k "blocks".
 - Each block can only be erased a limited number of times.

Speed: As high as 2+ GBPS read, 1+ GBPS write

Latency: as low as 50 microseconds, but can spike to 10 ms

SSDs simulate a spinning disk. That means LBAs pointing to 512B / 
4K disk blocks. 

Draw:

 - 4k OS pages in 8k physical pages in 16k physical blocks
   * Draw 4 physical blocks = 16 OS pages
 - Consider a write pattern of writing pages and then updating an index for
   each: 1,2,1,3,1,4,1,5,1,6,1,7,1,8
 - Read / Modify / Write happens a lot for block 1.
 - Now do it again with virtualized pages. (draw a virtual <-> physical page map)
 - Consider a DRAM cache - that'll eliminate the repeated writes of page 1.

Garbage collection:

 - Eventually we end up with a bunch of blocks that are half-used, with the other
   pages superceded by more recent mappings.
 - This isn't efficient use of space, so periodically the SSD is garbage collected.
 - This copies live data from half-used pages to new blocks and erases the resulting
   unused pages.

Wear leveling:

 - We have per-block write limits.
 - Some LBAs get rewritten frequently (browser history index)
 - Other pages are rewitten rarely (the background pictures included with your OS)
 - We want to spread the writes around. To do this, sometimes the garbage collector
   will move rarely-written pages to heavily used blocks, in the hope that they'll
   continue to not be written. This serves to distribute writes over the blocks
   on the disk.

Optimizing for SSDs:

 - There's still seek time, so sequential reads are faster than random reads.
 - The same is true for writes, but...
   * Pages are virtualized and we want to limit read/modify/write cycles, so all
     writes end up being seqential anyway.
 - We want to write entire pages at once. Writes below the physical page size are
   waste write cycles and cause write amplification (they're 16k anyway).

### Ideal NVRAM

Perfect storage would be more like RAM than like HDDs or SSDs.

 - Byte addressable for both reads and writes.
 - Near-zero latency, minimal penalty for random access.
 - No write / erase limits.
 - Really fast.

There are a variety of technologies being worked on that either have or
approximate these properties.

Recently, Intel and Micron have released "3D XPoint" memory:

 - Based on "phase change RAM"
 - Byte addressable
 - Very low latency
 - More rewrites than flash, but still not unlimited
 - Still tries to simulate a hard disk, so not actually byte addressable
   in default configs.

Once we have more RAM-like NVRAM, filesystem designs will want to change.

 - Small, random writes become OK.
 - No longer need or want fixed-size blocks. 
 - Instead, we want memory allocators, and possibly GC.
 - Intel is failing to ship 3D XPoint DIMMs in DDR4 form factor right now.

## FAT Filesystem

Draw a directory tree:

 - C:\
   - BOOK\
     - CHAPTER1.TXT (800B)
     - CHAPTER2.TXT (1600B)
   - COMMAND.COM (2800B)

The year is 1994 and we've got a computer running 32-bit MS DOS.

The filesystem it uses is called FAT. Specifically, FAT-32.

We want to store a tree of filesystem objects.

Main kinds:

 - Files
 - Directories

For MSDOS, a directory is just a table with 

 - A Name (8 + 3 characters)
 - Type of object (file or directory)
 - Location of where the FS object starts (cluster #)
 - file size
 - other metadata (not much)

The file system is layed out in three sections:

 - Superblock (1 block)
   - number of clusters
   - cluster size
   - root directory ptr
- File Allocation Table
   - An array of 32-bit numbers, one per cluster (only 28 bits actually used)
 - Data clusters
   - Where the data actually goes

Data clusters are the minimum allocation size of disk space. They're
usually larger than one block.

Let's say we have a 101MB hard disk. That would have had a cluster
size of 1K, so we'd have 100K clusters.

Disk layout:
 - Superblock
 - FAT: 100K 32-bit integers = ~400KB
 - data: 100K 1k clusters = 100MB
 
Superblock: Root Dir @ 1

Root:

- BOOK (D) @ 2
 - COMMAND.COM (F) @ 3

Book:

 - CHAPTER1.TXT (F) @ 6
 - CHAPTER2.TXT (F) @ 7

FAT works like a linked list with two special values:

 - FREE
 - END-OF-LIST

Free clusters are found by scanning the FAT for FREE entries. This
could be kind of slow. Not too bad if you use a persistent pointer
and next-free though.

Fragmentation:

 - As files are added and deleted, the free space ends up being
   in non-consecutive clusters.
 - New large files fit, but are spread around the disk.
 - Traversing the FAT for non-adjacent large files is worst case
   for HDDs.
 - So "defrag" tools would move blocks around to make files
   consecutive again.

Maximum File Size:

 - FAT32 can't handle files over 4GB
 - Because it uses 32-bit int for file size in directories

Large Disks:

Maximum size for FAT32 was 2TB with 32K clusters.

 - That means 250MB of FAT
 - 2TB limit was due to not being able to store larger LBAs in 32-bit numbers.

## Overflow

Start the slides: [Christo's FS Slides](http://www.ccs.neu.edu/home/ntuck/courses/2018/09/cs3650/notes/00-Spring/20-file-systems/10_File_Systems.pptx)

## Plan for Filesystems

 - 4 lectures
   - Hardware and FAT
   - CH03 starter code, EXT
   - FS slides (EXT optimizations)
   - logs and COWs
 - core topics:
   - hard disks vs. SSDs vs. ideal NVRAM
   - FAT
   - ext (basic)
   - FUSE
 - secondary topics:
   - ext2/3/4 optimizations
   - COW filessytems
   - log-based FSes



