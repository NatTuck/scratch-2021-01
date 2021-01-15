+++
title = "./99-lectures/18-malloc-pt2.md"
+++

# Lecture #18: Allocators Part 2

## Core Lecture Content

Please review this material before the scheduled lecture period. Our lecture
meeting on Teams will be a Q&A / discussion on this material and the current
assignment.

 - [Intro Video](https://youtu.be/IV3c8jQF0k0)
 - [Challenge 2](https://youtu.be/55cIcBKgE-4)

## Secondary Lecture Content

This material is also part of this lecture, but not essential for the Q&A. Please
review this content when convenient.

### Allocator Slides
 
 - [Allocator Slides Stolen from Christo](https://youtu.be/W8sHP6owR_E)
   {[slides](http://www.ccs.neu.edu/home/ntuck/courses/2017/09/cs3650/notes/15-alloc-slides/8_Free_Space_and_GC.pptx)}

### Real World Allocators

 * The allocator that inspired the slides is ptmalloc, or the stock allocator on
   Linux (provided by GNU libc). It's reasonably fast and very mature, but
   compared to other modern allocators is optimized more for consistency than
   speed.
 * The allocator that is considered the fastest today is jemalloc, which was
   heavily funded by Facebook.
 * The other major allocator worth mentioning is tcmalloc, which was heavily
   funded by Google.

### jemalloc Video

 * Watch: [An ACM Presentation on
   jemalloc](https://www.youtube.com/watch?v=RcWp5vwGlYU)
 * Keep in mind that this is a video about further optimizing an already
   optimized allocator.
 * Keep an eye out for the design elements of jemalloc that the speaker *isn't*
   excited about in this talk - some of those (like shared chunk metadata) are
   the secret sauce that makes jemalloc fast.

