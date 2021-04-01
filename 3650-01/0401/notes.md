
# Boot Sequence

 - Computer turns on
 - Firmware runs bootloader
   - Running the MBR boot code on the primary
     disk.
   - Bootloader loads code from elsewhere on
     disk to finish running.
 - Bootloader is configured with where OSes are,
   picks one to boot.
 - The OS kernel is stored on some partition, and
   the bootloader loads it and starts executing it.
 - The kernel is going to rescan disks
   - Find all currently available disks.
   - One partition is configured as root,
     this is mounted at / (or C:\)
 - Later, other partitions will be mounted.


# Homework Questions: xv6



# Filesystems Slides

 - Finished before "Extents and B-Trees (ext4)"
