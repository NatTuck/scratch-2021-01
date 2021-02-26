
# Homework Questions?

Abstract syntax tree:

```
    x = sqrt(7 + 3);
```

Text => Tokenize => Parse => AST

```
      assign
     /      \
   "x"      function call("sqrt")
                  | 
                  +
                 / \
                7   3
```

For the shell, this matters for complex
expressions. No nested syntax, no need for
a recursive data structure.

```
$ cat foo.txt | tac
```

```
struct cmd {
    char* left[];     // If no op, put cmd here.
    char* right[];
    char* op;
};
```

```
def really_lame_eval(line):
   if (line.contains?("<")) {
     cmd, file = line.split("<");
     ...
   }
```

# Today: Virtual Memory

Diagram of a process address space on a
32-bit machine:

```
    Process #10
3G +-------------------+
   | stack v
   +-------------------
   |
   +-------------------
   | heap ^
   +-------------------
   |
   +-------------------
   | .data
   |   0x20000: int some_global = 7;
   +-------------------
   | .text
   |   0x10000: main
   +-------------------
   |
0  +-------------------+

    Process #11
3G +-------------------+
   | stack v
   +-------------------
   |
   +-------------------
   | heap ^
   +-------------------
   |
   +-------------------
   | .data
   |   0x20000: int some_global = 9;
   +-------------------
   | .text
   |   0x10000: main
   +-------------------
   |
0  +-------------------+

```

 * The addresses that our programs see are *virtual*.
 * The OS and CPU conspire to make it look like those
   are the real addresses, even while some other addresses
   are nessisarily used on the memory bus.
 * So these process address spaces are called
   virtual address spaces.
 * What the kernel sees when the machine first boots,
   and the reality in hardware, is called the physical
   address space.
   
   
  
 
```
Load an executable:

File: /usr/bin/program:
5k +-------------
   | 0x500: .data
   +--------- 
   | 0x100: .text
   +-------
   |  table of contents
 0 +-------

Address space for PID #20,21
3G +-------------------+
   | 11 stack v (backed by swap space)
   +-------------------
   | 7,8,9,10
   +-------------------
   | 5,6 heap ^  (backed by swap space)
   +-------------------
   | 4
   +-------------------
   | 3 .data: "/usr/bin/program"+0x500
   +-------------------
   | 1,2 .text: "/usr/bin/program"+0x100 
   +-------------------
   | 0  (page #0 unused for null ptrs)
0  +-------------------+

Page table pid 20
   | Virtual    |  Physical | flags
   +------------+-----------+----------
   |    1       |   100     |  ro
   |    2       |   108     |  ro
   |    3       |   102     |  ro
   |    5       |   122     |  rw
   |    6       |   105     |  ro,cow
   |  ...       |           |
   
 Page table pid 21
   | Virtual    |  Physical | flags   |  file, offset?
   +------------+-----------+---------+------
   |    1       |   100     |  ro     | ...
   |    2       |   108     |  ro
   |    3       |   102     |  ro
   |    5       |   121     |  rw
   |    6       |   105     |  ro,cow
   |  ...       |           |
  
```

 * Memory is managed in 4k pages.
 * Every process has a page table, mapping
   virtual address to physical addresses.
 * This table has, conceptually, one entry per 
   page.

Can we max out on physical addresses?

 * Yes, if we have a 32-bit system, the maximum
   possible memory is 2^32 bytes = 4GB
 * We *can* allocate more virtual memory than
   physical memory.
 * We just can't have all that virtual memory 
   mapped to unique physical memory.
 * But we can point it elsewhere: like disk.
   - Storing stuff that appears to be in
     memory on disk is called swapping.

Fork is a cheap operation:

 * Conceptually it copies.
 * In practice it does not.
 * Instead: 
   - read only pages are shared
   - read/write pages are handled copy-on-write
 * Copy-on-write requires hardware support
 * Processors without a MMU can't validate
   writes, so they need to just copy the whole
   address space.

# Stalker: Shadow of Chernobyl

My new computer:

 - AMD Athlon 64 processor
 - Microsoft Windows XP, 64-bit preview edition 
 - 8GB of RAM
 - Stalker shipped a 32-bit executable
   - So 64 bit windows ran in 32-bit compat mode
   - So it gets 2GB
 - Stalker recommended system reqs was 2GB of RAM.
 - I started with a Nvidia GeForce ?? with 256MB VRAM.
   - Stalker ran kind of slow.
   - 1.6 for stalker + 0.25 for Nvida < 2GB
 - Upgraded to GeForce with 512MB of VRAM.
   - Stalker crashed, out of memory.
   - 1.6 for stalker + 0.5 for Nvida > 2GB

```
4G  +-------------------+
    |  Dedicated to Kernel
    | 
2G  +-----------+-------
    |           | Nvidia driver VMEM
1.6 +-----------+-------
    | stalker
 0  +-------------------
```

 - 32 bit windows executables have a "3GB mode"
   flag you can set, which makes the OS split
   memory 3GB / 1GB
 - Once I downloaded a sketchy tool and hacked
   the executable, it worked fine.

# Argument from the turn of the century:

 - How much memory does your system need to have
   before you really *need* to switch to a 64-bit OS.
 - Conceptually ia32 systems with PAE could do
   36-bit physical address = Max 64 GB
   - Problem: User address space limit is still 3GB.
 - If you want to assign all the RAM to one process,
   you need to do 64-bit for >3GB of RAM.
 - Things other than RAM can take virtual address
   space.
   - We saw drivers.
   - Also memory mapped files.
 - Probably the point to switch to 64 bit is 
   greater than 1GB of RAM.

# Memory Mapped Files

This is a way to do I/O without using the
read / write syscalls.

Instead, we map a file into memory by adjusting
the page tables.

 - When a store happens in this range, it implies a
   write to disk
 - When a load happens, it implies a read.
 - The kernel does the work.
 - The kernel caches everything efficiently.







