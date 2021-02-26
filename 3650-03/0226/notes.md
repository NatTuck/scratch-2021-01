
# Homework Questions?

Cases in evaluating a programming language:

Single term:

```
> 5
5
> x
3
```

An operator:

```
> 3 + 5
```

"3 + 5" => [3, +, 5]

Simple solution:

 - Find the operator "+"
 - Apply the operator to the stuff on the left
   and the stuff on the right.

```
> 3 + 5 + 4

Gives us AST

    "+"
    / \
  "+"   4
  / \
 3   5
```

Order of operations:

 - Explicitly not specified.
 - See what Bash does and do that.

# New Topic: Virtual Memory

Address space layout for a process running
on a 32-bit machine, simple version

```
 4G +--------------
    | stack v
    +--------------
    |
    +--------------
    | heap ^
    +--------------
    |
    +--------------
    | .text
    +--------------
    | .data
    +--------------
    |
 0  +--------------
```


```
Executable file for /usr/bin/program
 +-----------------
 | +4000: .text
 | +1000: .data
 | +0: Table of Contents 
 +-----------------
 (6000 bytes)
```

```
 4G +--------------
    | stack v
    +--------------
    |
    +--------------
    | heap ^
    +--------------
    |
    +--------------
    | .text
    |   from /usr/bin/program @+1000
    |   address 14750 - main:
    +--------------
    | .data
    |   from /usr/bin/program @+4000
    |   global var: 
    |   addr 17782 - int glob = 27;
    +--------------
    |
 0  +--------------
```

Different processes have separate memory address
spaces; user code uses *virtual* memory address.

The OS kernel and the CPU conspire to lie to your
program about what the memory addresses actually
are.

Every process has a page table, mapping the virtual
addresses that you see to the physical address that
the hardware actually uses to do real stuff.

The system manages memory in blocks called pages,
which are 4KB = 4096 bytes each.


```
Virtual address space layout for PID 10.
 4G +--------------
    | 35    stack v
    +--------------
    | 9,10  heap ^
    +--------------
    | 5     .text
    +--------------
    | 3,4   .data
    +--------------
    | 0     reserved for null
 0  +--------------
 
Page table for PID 10.

    | virtual page #   | physical page #  | flags
    +------------------+------------------+--------
    |      3           |     107          |  ro
    |      4           |     103          |  ro
    | ...    
    |     35           |      55          |  rw

Page table for PID 11.

    | virtual page #   | physical page #  | flags
    +------------------+------------------+--------
    |      3           |     107          |  ro
    |      4           |     103          |  ro
    | ...    
    |     35           |     755          |  rw

```


```
 3G +--------------
    | stack v    (backed by swap)
    +--------------
    |
    +--------------
    | heap ^     (backed by swap)
    +--------------
    |
    +--------------
    | .text
    |   from /usr/bin/program @+1000
    |   address 14750 - main:
    +--------------
    | .data
    |   from /usr/bin/program @+4000
    |   global var: 
    |   addr 17782 - int glob = 27;
    +--------------
    |
 0  +--------------
```

# Trying to play video games in ~2005.

 * Stalker, Shadow of Chernobyl

My computer:

 * AMD Athlon 64
 * 8 GB of RAM
 * Windows XP, preview 64-bit edition
 * Nvidia card with 256MB VRAM
   * 1.6 + 0.25 < 2GB

Stalker ran OK, but not great.

 * Upgraded my video card to a Nvidia card with 512MB VRAM
   * 1.6 + 0.5 > 2GB
 
Stalker crashed, out of memory.

Stalker had a recommended system requirement of
2GB of system RAM, like 128MB of VRAM.

 * Stalker shipped a 32-bit executable.
 * Stalker was running in 32-bit compatibility
   mode.

```
Address space for Stalker running in 32-bit Windows

 4G +--------------
    |  Reserved for Kernel
 2G +--------+-----
    |        | Nvida maps in VRAM here
    +--------+-----
    | 1.6 gigs used by Stalker
 0  +--------------
```

Solution:

 - 32-bit windows executables have a "3gb" flag
   that allocates 3 gigs to the user code and
   only 1gb to the kernel
 - I downloaded a sketchy executable off the web,
   hacked the binary, and it ran fine.

# When do we need to switch to a 64-bit OS?

 - 32-bit intel processors had an extension called
   PAE, which gave them 36-bit physical addresses.
   - Max memory = 2^36 bytes = 64 GB
   - Complication: Max 3GB per process
 - When we have more than 3GB of RAM, we want to use
   it all in a single process, so we want a 64-bit OS.
 - It turns out that address space gets used for other
   stuff:
   - Drivers maybe?
   - Memory mapped files
 - That leaves the cutoff around 1GB, more RAM than
   that and we want a 64-bit OS.

```
Address space for 32-bit Linux / 32-bit Windows 
with 3gb flag;

 4G +--------------
    |  Reserved for Kernel
 3G +--------------
    |
    |
    |  For user process
 0  +--------------
```

```
Address space for 64-bit Linux:

 -0    +--------------
       |  Reserved for Kernel
 -2^47 +--------------
       |
       |     not usable on most current CPUs
       |
 2^47  +--------------
       |  For user process
 0     +--------------
```

