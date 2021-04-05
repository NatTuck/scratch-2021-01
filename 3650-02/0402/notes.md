
# Homework Questions: xv6


# Filesystem Slides

Checksum false positives:

 - crc32 means 1 in 2^32
   - carefully crafted data could intentionally
     break checksum
 - sha256 means 1 in 2^256
   - nobody's spoofing a cryptographic checksum

A filesystem object could be:

 - A file
 - A directory
 - A symbolic link
 - A device
 - A named pipe
 - ...

Every filesystem object is an inode.

Object type is specified in the inode.

Directory:

      foo.txt => inode 4
      pictures => inode 7
      
Inode 4:

      type: regular file

Inode 7:

      type: directory


Stopped slides at: "Extents and BTrees (ext4)"





