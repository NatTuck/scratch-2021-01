

# Challenge 2 Questions



# New topic sequence: File systems

 - Today: Storage devices



# Hard Disk Speed: Random vs. Sequential Access

Average Seek + Rotational delay: 10ms

Sequential read speed: 100 MB/s

 - How long does it take to sequentially
   read 100 MB? 0.01 seek + 1 read = 1.01 s
 - How long does it take to randomly read
   100 MB in dispersed 4k blocks?
   - How many 4k blocks in 100 MB? 25000
   - 250 * 10ms = 250s
   - +1 for a meg of data read
   - = 251 seconds = more than a minute

Key idea: Random I/O is *slow* on hard disks.


# What we want: Ideal Non-Volatile RAM

 - Byte addressable
   - You can read or write any single byte.
 - Fast like DRAM
 - Non-volatile like flash or HDD (works with
   no power)
 - Unlimited writes
 - Low power consumption

There are a bunch of candidate technologies

 - MRAM
   - 512 Mb = 64 MB
   - $20
 - FeRAM
 - Phase change RAM
 - Memristors
 - ...

## Intel Optane (3D X-point)

 - Some sort of phase change RAM
 - Non-volatile
 - Appears to be byte addressable
 - Intel sells it in two forms:
   - NVME / PCIe solid state drive
   - DIMM that goes in a memory slot
 - Cheaper than DRAM per capacity 
 - Faster than NAND flash
 - Does have a erase/rewrite limit
 - More expensive than NAND
 
 
 
