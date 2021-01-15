+++
title = "./20-os-kernels/old-notes.md"
+++

 - Questions on the Challenge?
 
 
## Start with Christo's slides

[Arch and Kernels Slides](http://www.ccs.neu.edu/home/ntuck/courses/2017/09/cs3650/notes/17-kernel-boot/3_Arch_and_Kernels.pptx)

Stop at "A Simple OS Example"

## Modern PC diagram

 - Multi-core processor
 - Dual channel RAM with two seperate dedicated busses
 - PCI-e direct to each device
 - Stuff like USB comes off a PCI-e lane.
 - Legacy FSB with maybe a serial and PS/2 keyboard port.

## Modern PC Boot

### Early Boot: Old Plan

 - Processor starts @ BIOS
 - BIOS selects boot disk
 - Reads partition table, selects boot partition
   * limit, 4 partitions
 - Reads MBR, loads 1st-stage bootloader
   * limit, 2TB disks
 - 1st stage loads second stage
 - Second stage selects and loads kernel
 - OS Kernel is in control

### Early Boot: Modern Systems

 - Management Engine starts processor
 - Processor starts @ UEFI
 - UEFI selects boot device *or* boot OS
 - Loads 1st stage bootloader, verifies signature
 - Transfers control to 1st stage.
 - 1st stage loads second stage.
 - 2nd stage loads kernel, after optionally verifying signature
 - OS Kernel is in control

### Management Engine

 - Basically all modern computers have a sketchy second processor that runs
   before the main processor boots and has full access to the hardware.
   * Intel ME, AMD PSP, Raspberry Pi GPU 
 - These processors are intended to enable "enterprise" remote administration
   regardless of installed software or even operating system.
 - The Intel implementation has had some pretty bad security issues.
 - After a major security issue last year, researchers discovered a way to
   disable the Intel ME on some machines using a mechanism that was later
   confirmed to have been required by the NSA for their machines.

### UEFI Secure Boot

 - UEFI fixes some major issues with BIOS
   * Disk size limit
   * Partition count limit
   * Being 30 years old
 - But adds a sketchy new feature: Secure Boot
 - This feature had existed previously on phones, where it’s called a "locked
   bootloader".
 - Computers with this feature can only boot signed operating system code. Other
   operating system code won’t load.

Signed by who?

 - Cryptographic keys that can be used to verify boot software are loaded by the
   device manufacturer.
 - On phones, that’s generally manufacturer keys.
 - On machines sold with Windows, that’s generally just a key from Microsoft.
 - On Chromebooks, it’s a key from Google.
 - Some systems allow the user to load their own keys and thus sign their own
   boot software.

Why?

 - Some viruses have operated by overwriting the boot loader. Secure boot
   prevents such infected systems from booting up at all.
 - Other malware like keyloggers or remote access trojans like to run early in
   the boot process. This similarly prevents them from working.
 - Allows guarantees about all the software running on a system if the OS wants
   to enforce it - can require package signatures, iOS style.
 - Allows systems to make guarantees like "no screen capture software is running
   while playing a video", for "content protection".

Why not?

 - The practicality of resisting attacks by someone with physical access to a
   computer is limited. They could always do things like installing a hardware
   keylogger.
 - The person most likely to have physical access to a device and the ability to
   attempt an OS install is the owner.
 - Being "secure against the owner" so you can prevent them from installing
   their choice of operating system sounds like an anti-feature to me.

This produces an amusing result: The bootloader on many Linux install images is
signed by Microsoft.

## The OS Kernel

 - Runs on boot and stays running while system is on
 - Provides syscalls, which means it must manage:
   - Processes
   - Virtual memory
   - File systems and file I/O
   - Network I/O
   - Other hardware access

Consider a single-core processor.

 - Usually we want a user program running.
 - We transfer control from the user program to the kernel in two cases:
   - A syscall.
   - An interrupt.
 - The kernel runs with extra priviledges compared to normal programs.
   Specifically:
   - The kernel can access all of memory - user programs can't access restricted
     memory.
   - The kernel can execute all instructions - user programs can't access
     restricted instructions.
 - We say that the kernel runs in "ring 0", while user programs run in "ring 3".
   This is determined by a bit in the page table. The priviledge level of code
   being executed depends on how this flag is set in the virtual memory page the
   code was loaded from.

## Syscalls

 - Logically, syscalls are like function calls.
 - Mechanically, they work a little differently - syscalls are identified
   by a number indexing into a syscall table (array of function pointers)
   in kernel memory.
 - When a syscall occurs, the kernel looks up the syscall # (%rax) in this
   table, and then invokes the function pointer it finds.

## Interrupts

 - Hardware events
 - Timers
 - Scheduler
 - Interrupts can also be triggered from software.

Example:

 - Press a key on the keyboard.
 - That sends a signal down a physical wire - say interrupt line #10 - into the
   processor.
 - There's an interrupt table (an array of function pointers) that the kernel has
   set up.
 - The processor stops the currently running program, switches to kernel mode,
   and runs the function in interrupt table slot #10.
 - That function will use I/O ports or device-mapped memory to read the keypress
   from the keyboard and send it on to the terminal driver (or GUI, or whoever
   needs it).
 - Once the interrupt is done, the user program is resumed - without even knowing
   that it stopped.

This is important for programming. If you're writing a normal program it *will*
be randomly stopped for interrupts. Usually instructions happen nanoseconds
apart, but you'll occasionally see long gaps. If another process gets scheduled,
the gap between your instructions may be 8 milliseconds or more.

## Finish the Slides

[Arch and Kernels Slides](http://www.ccs.neu.edu/home/ntuck/courses/2017/09/cs3650/notes/17-kernel-boot/3_Arch_and_Kernels.pptx)

