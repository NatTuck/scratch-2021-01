
# Challenge 2 Questions



# Storage Devices

Total average seek + rotation delay: 10ms

Transfer rate: 100 MB/s

## Sequential read of 100MBs

 - 10 ms + 1 s = 1.01 s

## Read of 100MBs in randomly distrubted 4k blocks

 - How many 4k blocks in 100MBs? 25,000
 - Total latency: 250s
 - Transfer time: 1s
 - Total: 251s

Conclusion: Random acess is *sloooooooow* on HDDs.


I hear RAID-61 is pretty good.

sizeof(void) == 1


## Ideal NVRAM

 - Can address individual bytes for read/write.
 - As fast as DRAM.
 - Non-volatile like a hard disk.
 - No rewrite/erase limit.
 - Relatively low power.
 - Cheap

Candidate technologies:

 - MRAM
   - 512 Mb = 64 MB
   - $20
 - FeRAM
 - Phase-Change RAM
 - Memristors
 - etc

## Intel Optane (3D X-point; also maybe from Micron)

 - Some sort of Phase-Change RAM
 - Byte addressable
 - Slower than DRAM but faster than FLASH
 - Non-volatile
 - Cheaper than DRAM but more expensive than FLASH
 - Does have a write limit, but way higher than SLC FLASH
 - Provided in two form factors:
   - As NVMe/PCIe SSD
   - DIMM to go in a main memory slot


