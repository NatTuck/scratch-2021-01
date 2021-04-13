
# Homework Questions: File System

Mounting with the homework:

 - You run "make mount"
 - That calls your "nufs", passing in "mnt"
   as the mount point.
 - The fuse libary + driver will cause the kernel
   to mount your filesystem at that path.
 - What that means is that filesystem requests
   to that path will be delegated to your code
   for handling.

Filesystem image:

 - You're supposed to mount an image file called
   "data.nufs"
 - That file contains the structure of your file
   system.
 - If you unmount the filesystem and then remount
   it, any stuff in the filesystem survives.
 - If you unmount the filesystem, remove the image
   file, and then remount, you get an empty
   filesystem.

Steps to take for HW:

 - Step #1: Make sure getattr returns the correct
   error for a missing file, so you can make progress
   on writing to "one.txt".

# Copy on Write Filesystems

In the slides, we covered through ext4 filesystems.

 - Journaling filesystem
 - Some complexities like: block groups, btree directires

That's pretty much the norm for a modern desktop
filesystem. Desktop Windows uses NTFS.

The first copy-on-write filesystem in broad use
was ZFS.

 - Developed at Sun Microsystems for Solaris
 - Initially released for OpenSolaris in 2005.
   - The OpenSolaris license was Open Source (tm)
     but was explicitly not license-compatible
     with Linux
 - Core Idea:
   - Make the filesystem an immutable data structure
   - Any update to a data block would be to a copy,
     and then would be written to a new location
     on disk.
   - A shared strucutre tree was used to track
     where things were. 
   - Old roots are kept around.
   - Eventually old roots are deleted and unreferenced
     blocks are garbage collected.
  
```
            root
           /    \
          A      B
         / \    / \
        W   X  Y   Z
  
Update Z, that produces a new Z'
       
       Z' <- B' <- root' 
       
            root'
           /    \
          A      B'
         / \    / \
        W   X  Y   Z'
      
```

Direct benefits:

 - Corruption resistent: We didn't overwrite the
   old tree; the change hasn't actually happened
   until we write a new valid root pointer.
 - Snapshots: We can label old roots and mark
   them to not be deleted.

More ZFS features:

 - Built-in software RAID: Per-file replication,
   including RAID5/6 style parity.
   - ZFS is set up on a pool of multiple disks. 
 - Checksums: Each block has a checksum which
   can be used to validate (or repair!) that
   block on read.

Solaris used this to allow OS updates to be
rolled back: snapshot before the upgrade.

Issues:

 - It's hard to show disk usage.
 - If your disk gets full, you can't possibly
   recover because you can't delete stuff.

ZFS can't be included in the Linux kernel.

In ~2010, Sun Microsystems was bought by Oracle.

A couple years earlier, Oracle developers had
started BTRFS (effectively a ZFS rewrite for Linux).

You can now use ZFS on Linux, but it's an external
loadable kernel module.

Nowadays, BTRFS and ZFS are pretty similar.

 - Block mirroring works on BTRFS, but the
   RAID5/6 modes are still beta-quality.
 - BTRFS uses less RAM.

Default COW filesystems:

 - Linux: BTRFS
 - FreeBSD: ZFS

Apple and Microsoft have both released CoW filesystems.

Apple's AFS is a CoW filesystem focused on client-type
usage.

Features it has:

 - Snapshots
 - Metadata checksums

Features it doesn't have:

 - Any RAID stuff

Microsoft ReFS: Basically on par with BTRFS / ZFS.

 - Only available on Windows Server


# Garbage Collection Slides

Smart Pointers in C++:

 - shared_ptr <= atomic reference counting
 - unique_ptr <= type system guarantee one ref

# Short lecture ends early
