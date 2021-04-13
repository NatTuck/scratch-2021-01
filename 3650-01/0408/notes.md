
# Questions on HW10?

# Modern Copy-on-Write Filesystems

First example was ZFS.

 - Developed by Sun Microsystems for Solaris.
 - Intitially released for OpenSolaris in 2005.
   - Open Solaris was open source, but explicitly
     under a license not compatible with Linux.
 - Core idea: Make the filesystem an immutable
   tree, with writes to blocks going to a copy,
   and a shared structure tree to find stuff.
 - All blocks are copy-on-write.

```
              root
              /   \
             A     B
            / \   / \
           W   X  Y  Z 
           
           
           W'-A'-root'
           
              root' 
              /   \
             A'    B
            / \   / \
           W'  X Y   Z
```

Features Innate to CoW:

 - Corruption resistant: The old tree is always
   available as a valid version of the filesystem.
 - Snapshots: By pinning (marking to exclude from
   GC) / naming old roots, we can keep around specific
   old versions forver.

ZFS Also Has:

 - Built-in RAID 
   - You can give ZFS a "pool" of multiple disks.
   - Conceptually you can specify RAID policy per file:
     - Mirror?
     - Checksum redundancy like RAID-5 or RAID-6
 - Checksums
   - A checksum is stored for each block
   - Default was a cryptograhic hash, so we expect zero
     false-validity.
   - This allows error correction on read if there's
     redundancy for the file.

Solaris used this for a neat feature:

 - Every OS update created a before and after snapshot.
 - You could roll back a whole OS update if it broke stuff.

Issues:

 - It's hard to calculate disk usage and predict when
   the disk is going to get full.
   - When the disk does fill up, even deleting stuff
     can be impossible - that'd create a new version.
   - You're stuck deleting snapshots and having special
     error recovery mechanisms.

ZFS can't be included in the Linux kernel.

Now, ZFS for Linux is distributed as a seperate module. 
It's not clear that distributing this is legal.

In 2012 or so, Sun Microsystems got bought out by Oracle.

Around 2011, developers at Oracle released BTRFS, a CoW
filesystem for Linux that could be included in the Linux
kernel.

BTRFS and ZFS are almost identical.

 - The RAID5/6 functionality isn't quite 
   stable on BTRFS.
 - Removing disks from a pool works worse on ZFS.
 - ZFS takes a lot of RAM by default.

Apple and Microsoft have both released CoW filesystems.

 - Apple AFS provides a subset of ZFS features:
   - Snapshots
   - Metadata Checksums
   - No RAID functionality
 - Microsoft's ReFS
   - Windows Server only
   - Basically on par with ZFS / BTRFS
   - Can't write to CoW snapshots.

# Garbage Collection Slides



