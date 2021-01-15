+++
title = "./12-mmap-io/notes.md"
+++

# Computer Systems

## First: Challenge Questions

## Introducing Virtual Memory

 - Draw the simple address space diagram.
 - Complications:
   - Two programs may want to use the same numerical address.
   - When we fork, the programs are guaranteed to have the same addresses.
   - So the addresses can't be physical locations on the RAM chips.
   - Instead, the CPU and OS conspire to give every process its own
     *virtual* address space.
 - Discuss swapping
   - Everything is backed by something on disk.
   - For text and rodata, that's the binary.
   - For stack and heap, that's swap.
   - For rodata, that's the binary COW to swap.
 - Granularity: 4k pages
 - On fork, we COW
   - Not true without MMU - then we have to copy.
   - Discuss fork vs. vfork
 - We can cram other stuff in here.
 - Stalker on 32-bit Windows story.
   - Stalker used about 1.6 GB of RAM
   - By default, windows assigns 2gb to user, 2gb to kernel
   - 512 meg video card, mapped into userspace
   - 3gb flag
   - Draw a diagram.
 - Mmmaped files
   - Map a file into memory, backed by itself.
   - Can do I/O directly with memory accesses.
   - Kernel handles reading.
   - Can be more efficient than normal read/write, sometimes.
   - Definitely easier for manipulating structs on disk, as long as you don't care 
     about files being portable across systems.

## Memory Mappings, mmap I/O

 - show save-array
 - show print-array
 
## Shared Memory

 - Show shared1 / shared2.
 - Show barrier/shared

