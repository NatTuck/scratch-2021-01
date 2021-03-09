

# First: Homework Questions


# Today: Threads

Two ways to get concurrent / parallel
execution:

 - We can fork() for a new process
 - We can pthread_create() for a new thread

Key difference: with threads, all memory
is shared by default.

 - Advantage: It's possible to allocate
   (or reallocate) new shared memory.
 - Disavantage: 100% data races


# Concurrency vs Parallelism

"Parallel" is a hardware thing. Two processes
are running in parallel if they're physically
running at the same time on seperate hardware
resources (e.g. CPU cores).

"Concurrent" is an programming pattern thing.
When two things happen concurrently, that means
we can't assign an order to them. This can
be useful if we don't want one thing to wait
for another thing to happen.

# The History

## Back in the day: Minicomputers

 - We'll start with Minicomputers, possibly
   running UNIX
 - Before multi-processor systems parallelism
   didn't matter.
 - Concurrency matters a lot.
   - Two people should be able to run programs
     at the same time.
   - Implementing the shell background operator
     ("&") was a straightforward extension.
 - We can't trust users / programmers to share
   resources. The OS needs to do pre-emptive
   scheduling - if one program wants to hog
   the CPU, then the OS stops it and gives
   other programs a turn.
 - Similarly, we need memory safety, MMU
   hardware, and virtual memory.
 - The mechanism for concurrency on this
   style of system is processes.
   
## Back in the day: Early Microcomputers (PCs)

 - The thing that really pushed concurrency
   was the GUI.
 - This sort of concurrency was supported by
   cooperative threads. User programs would
   occasionally call "yield()" to give other
   programs/threads a chance to run.

## Multiprocessors

 - We got common multi-processor servers in
   the late 1990's.
 - Now we needed mechanisms for parallelism.
 - Windows and Solaris had good parallel thread
   support.
   - So threads were the default mechanism
     for parallel speedup.
 - Linux didn't get parallel thread support
   until like 2002.
   - Webservers need to be concurrent, and
     gain a lot from parallelism.
   - Parallelism meant calling fork()
   - The Apache webserver called fork()
     for every incoming web request.
   - Side effect: fork() is super-optimized
     on Linux (and other free Unixes like
     FreeBSD).
   - When threads did show up on Linux, they
     were a simple variant of fork()
 - Result:
   - Threads are fastest on Windows.
   - Threads are fast on Linux.
   - Processes are nearly as fast on Linux.
   - Processes are really slow on Windows.
 - For cross platform apps, probalby default
   to threads for parallelism/concurrency.
 - Exception: If you want memory separation.

# Today

 - Multi-core processors became standard
   around 2005.
 - You can't by a new, single core computer
   if you wanted one.
   - exception: Small embedded stuff.
   - Lamest phones / laptops are quad-core.
   - A nice workstation is 128-cores.
 - So all modern programs that need to do
   CPU heavy work must be parallel or they're
   written badly.
   

# Why so many Linuxes?

 - Anyone is allowed to take the Linux kernel,
   the GNU C library and compiler toolchain,
   the X11 or Wayland GUI libraries, the open
   source applications, etc - and their own
   operating system.










