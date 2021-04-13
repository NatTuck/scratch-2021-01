
# HW Questions: Filesystem


# Copy on Write Filesystems

Slides covered through ext4

 - Journaling filesystem

New Thing: Copy on Write Filesystem

First example: ZFS

 - Developed at Sun Microsystems for Solaris
 - Initial release with OpenSolaris in 2005.
   - The OpenSolaris license was open source,
     but explicitly incompatible with Linux.
 - Core idea: Make the filesystem an immutable
   data structure; all writes went to a copy
   of the modified block, and a shared structure
   tree was updated to allow the newest versoin
   of stuff to be found.
 - Every change generates a new tree root.
 - We store a list of all old roots.
 - Old roots and unreferenced nodes get
   garbage collected.
 

```
             root
            /    \
           A      B
          / \    / \
         W   X  Y   Z

    Write to Z, producing new block Z'

    Z' <- B' <- root'
    
           root'
          /    \
         A      B'
        / \    / \
       W   X  Y   Z'
```

Direct features:

 - Corruption resistent: The old tree root isn't
   removed, nor is any part of the old tree modified.
 - Snapshots: We can mark specific roots as stuff we
   want to keep and give them names.

Other Features in ZFS:

 - Built in RAID: We can do replication, including
   parity-based replicaiton on a per-file level.
   ZFS is used on pool of disks, managed by the
   filesystem driver.
 - Checksums for each block: Corruption can be 
   detected or repaired.

Solaris used ZFS to enable a neat feature: snapshots
before and after every OS update.

Issues:

 - It's hard to show disk usage.
 - Everything breaks if the disk fills up.
 - ZFS can't be shipped as part of the Linux kernel.

Licensing nonsense:

Sometime around 2010, Oracle bought Sun Microsystems.

Slightly before that, developers at Oracle started
development of BTRFS, effectively a ZFS clone for
Linux.

Compared to ZFS, BTFS:

 - Has basically the same features
 - The simulated RAID5/6 modes are not 100% stable.
 - Uses less RAM 
 - BTRFS has, at least historically, been better
   about letting you remove disks from pools

You can do ZFS on Linux - it's distributed as an
external kernel module.

Apple and Microsoft have both released modern CoW
filesystems.

Apple's AFS provides a subset of the ZFS features
useful for "client" devices:

 - Snapshots
 - Metadata checksums

Not included:

 - Any sort of RAID

Microsoft's ReFS is on-par with ZFS/BTRFS.

 - Only on Windows Server

# GC Slides

C++ smart pointers:

 - shared_ptr is a reference count
 - unique_ptr is a type system hack

https://www.microsoft.com/en-us/research/uploads/prod/2020/11/perceus-tr-v1.pdf 

https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-908.pdf whe

## Manual Allocators

 - The thing in GNU Libc
 - tcmalloc
 - jemalloc
 - mimalloc

# Short lecture ends early



