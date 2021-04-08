---
layout: default
---

# Computer Systems

## Homework Questions?

## Ext File System

The Extended File System (ext) was the standard filesystem for early Linux. It
was based on the traditional UNIX filesystem, UFS.

Basic Layout:

 - Superblock
 - inodes
 - Data Blocks (4k each)

Looks a lot like FAT. 

More detail: Two bitmaps between superblock and inodex:

 - Free inodes
 - Free data blocks
 
Each file system object is represented by:

 - 1 inode (metadata)
 - 1 or more data blocks (data)

Superblock contains:

 - Size and location of all these pieces.
 - inode # of root directory

inodes contain:

 - owner user id
 - size in bytes
 - creation time
 - modification time
 - mode
   - object type (d / f / symlink / etc)
   - permissions (rwx / ugo)
 - number of hard links
 - array of 12 data blocks
   - 10 direct (10 * 4k = 40k)
   - 1 indirect (1024 * 4k = 4M)
   - 1 double indirect (1024 * 1024 * 4k = 4G)

directories contain a table of:

 - file name
 - inode #
 
Example:

 - /
   - /tmp
     - hello.txt (1k)
     - cat.jpg (17k)


## FUSE API / Starter Code

Pull down and unpack the starter code.

The idea:

 - Normally file systems are kernel code.
 - FUSE is an interface that allows file systems to be implemented
   as user programs.

What happens:

 - You run a FUSE program, mounting its file system on some path.
 - Future FS operations on filesystem objects in that subtree
   are handled by the FUSE program.
 - This continues until the filesystem is unmounted.
 - Each operation can be implemented pretty much any way, although
   some approximation of POSIX filesystem behavior will result in
   less buggy behavior.

## Hints

 - Look at hints folder.
 - Talk about how the pieces map to an ext-style FS.

## Slides

[Christo's FS Slides](http://www.ccs.neu.edu/home/ntuck/courses/2018/09/cs3650/notes/00-Spring/20-file-systems/10_File_Systems.pptx)


