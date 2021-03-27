
# Look at CH02 starter code


# CH02 questions?

## What are arenas?

An arena is the data structure which is nessiary
to implement a memory allocator. Or an arena
is a complete memory allocator.

## Best strategy?

The best strategy is whatever you can benchmark
fastest.

You'll probably want something fancier than a
singly linked O(n)-for-everything freelist.

## Fragmentation Test

 - If you allocate a lot of memory at one size and
   then free it.
 - You must be able to reuse that memory for an allocation
   of a different size.

# OS Kernels: Intro, Boot Process

Who can sign an OS to let it run on a smartphone?

 - Smartphone manufacturers
 - Operating system vendors

 - Sometimes these are the same: e.g. Apple
 - Sometimes there's two companies: e.g. Samsung, Google

Who can sign an OS to let it run on a desktop or laptop?

 - Hardware manufacturers
 - Operating system vendors
 
 - Sometimes they're the same:
   - In the case of Apple, only Apple. 
   - Microsoft surface? Microsoft.
 - Sometimes different:
   - Dell, Microsoft
   - Lenovo, Microsoft

 - Depends how you have "secure boot" configured in your
   UEFI firmware.
   - Anyone?

 - How do you run Linux?
   - Disable secure boot.
   - Get Microsoft to sign your bootloader.



# XV6 UNIX & HW09
