
# CS3650 Lecture 2

Course site: https://ntuck-neu.site/
 
 - Code from class is in "Nat's Scratch Repo"

# C => ASM

 - For this class, "Programming" means "Writing C code"
 - For a Unix style OS (e.g. Linux, systems following
   the "POSIX" standards), you write programs in C.
 - System calls are exposed to C programs.
 - The hardware runs machine code - which is a binary
   sequence of instructions.
 - Instructions, and thus machine code, vary across
   machines.
 - Machine code is for machines, not people.
 - Assembly code is the same sequence of instructions,
   encoded as text.
   - An assembler goes ASM => Machine Code
 - To compile a machine code, we translate C to ASM.
   - A C compiler goes C => ASM (=> Machine Code)
 
# Systems, Archaeology, and Intel-style ISAs

Intel released the 8086 processor in 1978. It was based
on the 8008 from the 1972, and that was based on the 4004.

At some point there must have been an 8 bit microprocessor:

```
RAM ============== CPU
```

8-bit microprocessor:

 - The memory bus handles 8 bits
   - Of data
   - For addresses
 - If we have a memory chip and we're going to
   differentiate bytes of memory using an 8 bit address,
   how many bytes of memory can we have?
   - 256 = 2^8
 - Registers in the processor are 8 bits, storing 8
   bit numbers (i.e. one address)
 - This processor had 5 registers:
   - a, b, c, d
   - ip
 - What a processor does:
   - read one instruction from the memory at address in %ip
   - execute that instruction
   - increment %ip to next instruction
 - What kind of instructions?
   - arithmetic instructions, e.g. add, sub, mul, div
   - Test instructions: on intel, cmp
   - conditional branch: sometimes change %ip, e.g. jge
   - Load/store instructions: on intel, mov
     - mov (%a), %b // copy from address in %a to %b
     - mov 4(%a), %b // copy from (address in %a) + 4 to %b
   - jump: change %ip
   - syscall: ask the OS to do something
 - Instructions tend to operate on at least one register
   - On a RISC machine, mostly on registers
   - Intel isn't RISC, at least externally
 - If instructions operate on memory locations, the CPU needs
   to block to do the read/write from RAM.

The 8086 was a 16-bit microprocessor:

 - 16 bit registers
 - 16 bit memory bus
 - 16 bit addresses
 - How many bytes of RAM can we address with 16 bits?
   - 2^16 = 65536 = "64K" 
 - 9-ish registers:
   - %ax, %cx, %dx, %bx
   - %si, %di, %bp, %sp
   - %ip
 - Can access low half of %ax with %al,
   high half with %ah 

The 80386 or i386 was a 32-bit microprocessor, backwards
compatible with the 8086.

 - Had a 32-bit data bus (= memory address size).
   - How much RAM can we address now?
   - 2^32 = 65536*65536 = 4.3 billion  = "4G"
 - 32 bit registers
   - Could still use the old 16-bit names (e.g. %ax)
     for the lower 16 bits.
   - %eax, %ecx, %edx, %ebx, %esi, %edi, %ebp, %esp, [%eip]
   - Registers like %al and %ah
     - %ah is upper 8 bits of lower 16 bits of %eax
   - Full 32 bit registers start with e (e.g. %eax)

AMD Athlon 64 was a 64-bit microprocessor, backwards 
compatible with the Intel 8086 and i386. This was the
first "AMD64" (= x86_64, x64, Intel 64; not ia64
even though i386 is sometimes called ia32).

 - It had a 48-bit data bus, designed to be extendable
   to 64-bits later.
   - 2^48 = 256 TB
   - 2^57 = 128 PB
   - 2^64 = 16 EB
 - 64-bit registers 
   - New names are like %rax.
   - Using the old names (e.g. %eax) gets you the least
     significant 32-bits of the 64-bit registers.
   - All the older names work too.
   - New registers: %r9, %r10, ..., %r15


```
[                %rax             ]
             [      %eax          ]
                    [    %ax      ]
                    [ %ah ] [ %al ]
```




