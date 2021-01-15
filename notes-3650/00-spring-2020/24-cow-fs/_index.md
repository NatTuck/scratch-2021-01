+++
title = "./24-cow-fs/notes.md"
+++

# Copy-on-Write Filesystem

 - Slides: LFS
 - History / description of modern COW Filesystems
 - Technical description of persistent data structures and
   the COW FS plan
 - Some examples with the solution code

## History of COW

### ZFS

Sun Microsystems introduced ZFS in 2005 for their Solaris operating system.

New features:

 - Very large fixed size limits
 - Copy on write
 - Snapshots
 - RAID
   - Built in striping and mirroring over pools
   - RAID-Z
 - Checksums
 - Snapshot send / recieve

Sun made Solaris open source in 2005, so ZFS was open source too. But it was
released under a license specifically chosen so that Solaris code couldn't be
used in the Linux kernel.

Today ZFS is primarily used on Solaris and various BSD Unixes, but can be run
both Linux and MacOS. Using it on Linux might be legal, and definitely has some
technical gotchas due to the legal issues.

### BTRFS

Oracle funded development of BTRFS as a clone of ZFS for Linux starting in 2007.
This was nessisary due to the licensing issue.

Today, BTRFS has basically the same feature set with a different set of quirks.
BTRFS is probalby a bit more suitable for smaller deployments (e.g. a desktop
with a mirrored pair of disks) than ZFS, while ZFS is probalby more mature for
larger deployments (e.g. a 20 disk RAID array).

Amusingly, Oracle bought out Sun in 2010. At that point Oracle had the ability
to relicense ZFS to run on Linux, and has chosen not to for the past ten years
in favor of continuing to develop both filesystems.

### Apple AFS

Apple's AFS provides a subset of the ZFS features: snapshots and metadata
checksums. It doesn't provide any native multi-disk support. This is standard on
new Apple devices.
   
### Microsoft's ReFS 

This modern FS is available on Windows Server, but not on consumer desktop
versions of Windows. It has multi-disk support, data and metadata checksums, and
snapshots (but not the ability to write to C-O-W copies of snapshots).

