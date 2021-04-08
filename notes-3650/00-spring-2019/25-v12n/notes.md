---
layout: default
---

# Computer Systems

Only three classes left.

 - Today: Bonus topics.
 - Friday: Strategies for Parallelism and Concurrency
 - Last Class: Wrap Up
 - Do Your TRACE evals - the administration wants me to have a 100% response rate

Schedule questions?

Homework 10 Extension

Challenge 3 is due next Monday.

 - Several students still don't have teams.
 - Everyone: Double check that your team is correct, and if not contact the
   TAs to have it fixed. 

Challenge 3 questions?


## Topic 1: Auth and Access Control

[Christo's Auth & Access Slides](http://www.ccs.neu.edu/home/ntuck/courses/2015/01/cs5600/slides/12_Auth_and_Access.pptx)


## Topic 2: Virtualization

Early computers ran one program at a time.

To run multiple programs, the OS and hardware conspire to simulate each program
getting its own computer by virtualizing memory and I/O devices.

But some programs were later written to assume that they get a whole modern
computer to themselves. As machines - especially servers - got drastically more
capacious than was needed for a single program, a demand grew to consolate
multiple machiens into one. Or maybe administrators want to separate physical
machines into multiple logical machines for security or relability.

This lead to "virtual machine managers", which let multiple operating system
instances be installed and run in parallel on the same hardware.

It's worth contrasting virtual machines with containers. This is the idea -
first popularized in FreeBSD as Jails - that the OS kernel should prevent some
processes from having any access to the rest of the machine while still
appearing to see a complete system. Containerized processes are still running
normally under the same kernel as any other processes on the system, they just
get restricted responses to many syscalls.

Another thing to consider with this stuff is the Spectre vulnerabilities.

The basic idea of Spectre is that programs can access data that they shouldn't
through speculative execution. Modern processors - especially Intel processors -
have a bunch of problems where they will execute code *before* verifying that
the process has permission to run that code. Permission is checked afterwards,
and the processor undoes the actions and throws away the result if it's not
allowed. Unforunately, memory loads into cache can't be undone, nor can resource
utilization detectable by sibling hyper-threaded virtual cores.

These issues are now more than a year old, and still not fully fixed in hardware
for any high performance processor. Until these issues are resolved, any attempt
to set up security barriers between concurrently executing processes on the same
hardware is suspect.

[Christo's Virtual Machine Slides](http://www.ccs.neu.edu/home/ntuck/courses/2015/01/cs5600/slides/11_Virtual_Machines.pptx)




