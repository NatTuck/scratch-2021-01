+++
title = "./21-disks/notes.md"
+++

# Next four lectures

 - Disks; RAID
 - [Cristo's
 Slides](http://ccs.neu.edu/home/ntuck/courses/2015/01/cs5600/slides/10_File_Systems.pptx);
 stop before LFS
 - ext review; FUSE
 - Modern Filesystems; LFS -> ZFS -> BTRFS

# Today: Disks and RAID

## Hard Disks

 - Draw a disk
 - Tracks
 - Blocks (traditionally 512b, sometimes now 4k)
 - R/W Head
 - Platters
 - Logical Block Addresses

Latency for reads / writes:

 - Head latency to move to correct track
   - 1-10 ms 
 - Rotation latency
   - 7200 RPM, math it out, 4ms
 - Max transfer is maybe 150 MBPS

 - So 150 MB of sequential 4k blocks takes about a second
 - How long does it take to transfer 150 MB of random 4k blocks?
   * 6ms to move read head + 4 for disk = 10
   * 150M / 4k = 37.5k
   * So 375 seconds just for seeks.

Optimizing for HDDs:

 - Avoid seeks by avoiding random access

## Solid State Drives

 - Simulate HDDs using NAND Flash
 - Reads generally fast
   - maybe 25 microseconds to read a 4k block
 - Writes are pretty fast too
   - maybe 100 microseconds for a 4k block
 - But... writes are complicated

The problem: Erases
  
 - You can only write to Flash if it's empty.
 - Flash with data on it can't be overwritten, instead
   it first must be erased and then the newly empty
   space can be written to again.
 - Erases happen in large chunks at once - say 16k as
   an example.
 - Erases are pretty slow - more than 1 ms per chunk.
 - The number of times a chunk can be erased is limited. For the new low price
   QLC flash, that's like 1k erases per block. Once the erases run out that
   chunk can't be erased anymore.

This combination means that the obvious way to use flash in an SSD doesn't
really work. Some files on disk get rewritten a lot, others almost never. If
logical blocks mapped to physical blocks of flash, SSDs would be slow due to all
the erases and they'd die pretty quick - a QLC drive might die in a few seconds.

This is solved with a virtualization layer. Kind of like with virtual memory,
SSDs store a table that translates logical block #'s into physical block
numbers. When a write happens to a logical block, the drive picks a physical
block to use that happens to be empty and then changes the mapping. 

This allows the drive firmware to optimize for:

 - Speed, by always keeping a stash of empty blocks ready for writes.
 - Endurance, by spreading erases around over all the physical chunks on the
   disk. 1000 erases per chunk isn't a lot, but with 100k+ chunks in an SSD,
   millions of erases total ends up being pretty usable for many use cases


## RAID

Redundant Array of Inexpensive Disks

 - Disks are unreliable
 - Disks are kind of slow
 - We can try to solve both problems by adding more disks.

RAID-0: Stripes

 - 2 disks is double speed on large reads

RAID-1: Mirroring

 - 2 disks means one can fail

RAID-10: Stripe of mirrors

 - Four disks gets you double performance and lets one disk fail.

RAID-4: Parity Disk

 - Any one disk can fail

RAID-5: Striped Parity

 - (n-1)x performance, any one disk can fail

RAID-6: Multiple parity

 - Two disks can fail



