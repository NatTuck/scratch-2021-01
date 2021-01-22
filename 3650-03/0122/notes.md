
# Course Website

http://ntuck-neu.site/

# HW01: Next Tuesday

# C -> ASM

To call C from Java, Bing "JNI". To call C from most
HLL "scripting" languages, look up "SWIG".

 - For this class, "Programming" means "Writing C Code".
 - On Linux (and UNIX / "POSIX" systems) the OS
   API is primarily exposed as a C interface to
   the system calls.
 - The hardware doesn't run C, it runs machine
   code - a series of machine instructions encoded
   efficiently in binary.
 - Machine code is for machines, not humans. 
 - Assembly is the same sequence of machine instructions
   encoded as readable text.
 - An assembler translates ASM => Machine Code
 - To run a C program, you run it through a compiler
   which translates C => ASM (=> Machine Code)
 - Can still translate C => ASM if you ask for it.

# History of Intel ISAs

Systems = Archaeology > Engineering

Intel released the 8086 processor in 1978.

The 8086 was based on the 8008, which was was based
on the 4004.

At some point there must have been a simple, 8 bit
microprocessor.

```
CPU =============== RAM
```

This 8-bit microprocessor:

 - 8-bit data bus connecting it to memory.
   - This allows 8 bit values to be sent in one action.
   - That includes 8 bit memory addresses.
 - An 8 bit memory address, assuming each address refers
   to one byte, allows for at most how much memory?
   - 2^8 = 256 bytes
 - In addition to RAM, we have another place to store
   values: Registers
 - Registers are 8 bits 
 - This processor had 5 registers:
   - a, b, c, d
   - ip, the instruction pointer
 - What a processor does:
   - Reads one instruction from memory at the address in ip.
   - Execute that instruction
   - Increase ip to point to the next instruction
   - repeat
 - What instructions?
   - Arithmetic instructions: add, sub, mul, div,
     bit shifts, etc...
   - Load/store instructions: on Intel, mov
   - Comparison instruction: on Intel, cmp
   - Conditional branch: e.g. jg to jump if greater
   - Jump: jump unconditionally
   - Syscall: Ask the OS to do stuff
 - Instructions tend to operate on at least one register
 - Instructions can operate directly on memory; this
   means waiting until a memory read/write completes.

Intel released the 8086 processor in 1978. It was a 16-bit
microprocessor.

 - It had a 16-bit data bus (and 16-bit memory addresses).
   - How many bytes can we reference with a 16-bit
     memory address?
   - 2^16 = 65536 = "64K"
 - It had 16-bit registers.
   - %ax, %cx, %dx, %bx, %si, %di
   - %bp, %sp
   - ip
 - Can access low half of regisers with, e.g., %al
   - High half with %ah

Intel released the 80386 processor around 1990, which ran the
i386 instruction set. It was a 32-bit processor, backwards
compatible with the 8086.

 - It had a 32-bit data bus (= 32-bit memory address)
   - 2^32 = 4.2 billion = "4G"
 - It had 32-bit registers
   - New names: %eax, %ecx, %edx, %esi, %esp, ...
   - Using the old names (%ax) gets you the low 16
     bits of the 32-bit registers.
   - %al, %ah are still low and high of low 16 bits.
 
The AMD Athlon 64 was released in ~2002. It was the 
first AMD64 archetecture processor, backwards 
compatible with the i386 and 8086 archetectures.

AMD64 is also called x86_64, x64, and Intel 64, but not
ia64 even though ia32=i386.

 - 48-bit data bus, designed to expand to 64-bit later
 - 64 bit addresses, only 48 bits actually used
   - 48 bit address max memory = 2^48 = 256 TB
   - 57 bit data bus / 57 addr = 2^57 = 128 PB
   - 64 bit address max memory = 2^64 = 16 EB
 - It has 64-bit registers
   - Named %rax, %rcx, %rdx, %rdi, etc
   - All the old names still work for parts of the
     lower 32-bits.
 - Added 8 new registers: %r9, %r10, ... %r15

