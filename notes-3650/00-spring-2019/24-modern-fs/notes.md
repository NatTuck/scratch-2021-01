---
layout: default
---

# Computer Systems

## Homework Questions?

## Slides

[Christo's FS Slides](http://www.ccs.neu.edu/home/ntuck/courses/2018/09/cs3650/notes/00-Spring/20-file-systems/10_File_Systems.pptx)

Start @ ext3


## RAID

We can get better performance and/or reliability by using several drives instead
of only one.

 - Raid 0: Striping. Two drives can go twice as fast as one. Put even blocks on one
   drive and odd blocks on the other.
 - Raid 1: Mirroring. Two drives can be twice as reliabile as one. Just put the same
   data on both drives. If one fails, you still have the other. Can also speed up
   reads by a factor of two.
 - Raid 5/6: Stripes + Parity. With data striped across N drives you can add one
   additional drive to make space for parity information:
    - Reads and writes are sped up by N.
    - One drive can fail without data loss.
    - Adding extra parity drives is possible, but gets tricky (Raid 6).

## Copy-on-Write Filesystems

Modern systems are moving to "copy on write" filesystems.

The first example of this class of filesystem was ZFS.

 - Developed at Sun Microsystems for Solaris.
 - Initially available for OpenSolaris in 2005.
 - Core idea: Make the filesystem an immutable data structure, with
   writes to blocks going to a copy, and a shared structure tree to
   find things. All blocks are copied on write.
 - (draw a persistent binary tree, show an update with a new root)
 - Old roots are garbage collected.

Features:
 
 - Corruption resistant: The old tree root is always available as the last
   valid version of the file system.
 - Snapshots: You can pin old roots and access old versions of the complete FS
   at any time.
 - Built-in RAID: Support for multiple disks and data replication is built-in,
   either acting like RAID-1 or RAID 5/6.
 - Checksums: A checksum is stored for each block. If the data doesn't match,
   it either gives a read error or uses an uncorrupted replica of that block if
   available.
 - Solaris did something neat with this: All OS updates created a snapshot,
   which allowed the update to be rolled back if there were problems.

Issues:

 - This kind of design has issues with showing disk usage and recovering when
   disks get full. Multiple snapshots can share blocks, and no operation (other
   than "delete snapshot") actually frees disk space.

ZFS can't be included in the Linux kernel due to license issues. ZFS-on-Linux is
available as an external module, but it's not 100% clear that it's legal.

Due to the legal issues, Oracle started another similar FS project called BTRFS.

Compared to ZFS, BTFS:

 - Has similar features.
 - Block mirroring works, but RAID5/6 still isn't stable.
 - Uses less RAM - ZFS likes you to give it 8+ gigs for cache.

Apple and Microsoft have both released their own modern filesystems:

 - Apple's AFS provides a subset of the ZFS features: snapshots and metadata
   checksums. It doesn't provide any native multi-disk support. This is standard
   on new Apple devices.
 - Microsoft's ReFS is available on Windows Server, but not on consumer desktop
   versions of Windows. It has multi-disk support, data and metadata checksums,
   and snapshots (but not the ability to write to C-O-W copies of snapshots).

