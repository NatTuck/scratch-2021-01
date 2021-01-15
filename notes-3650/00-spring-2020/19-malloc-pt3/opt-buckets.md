+++
title = "./19-malloc-pt3/opt-buckets.md"
+++

# Optimizing a Bucket Allocator

## General Idea

 - A bucket allocator is like a buddy system allocator but with
   a couple of simplifications that change which problems we have
 - One big problem with a buddy system allocator is that it only handles
   power of two sizes. And that's power of two sizes *including* any headers.
   - That means power of two sizes are our worst case, since adding the header
     bumps us up to the next power of two.
   - Programmers like to allocate power of two size blocks, so that's a problem.
   - A bucket allocator handles this by allowing other sizes as well. The sizes
     can be close to arbitrary as long as they don't break alignment.
   - Commonly, the sizes are powers of two plus some extra sizes between the
     powers of two.
 - Another issue is that coalescing different sizes adds complexity and causes
   issues. Bucket allocators tend not to allow direct coalescing between sizes -
   instead entire chunks are allocated for a specific size.

## Draw stuff

 - Draw a picture with sizes {8, 16, 24, 32, 48, 64, 80, 96, ...}
 - malloc(20)
   - add header to 28
   - round up to 32
   - allocate a whole page of 32's
 - This is great if we have lots of allocations of the same size
 - Seems inefficent for a bunch of different sizes.
 - But even if we have 100 small sizes and waste a whole page for each, that's
   only 400k.
 - For larger sizes (e.g. >= 512), we probably need to allocate multiple pages
   at a time to avoid external fragmentation. (e.g. size 3084 wastes 25% if we
   allocate only one page for it each time)
 - For very large sizes we want to just call mmap directly.
 - One important trick from jemalloc: If we request multiple pages but don't
   touch most of them, they don't really get allocated. We probably can't take
   advantage of this much in this assignment due to the VM limit in the frag
   test, but it's a valuable trick in real allocators.

## Optimizations

 - Pick a constant chunk size (e.g. 64k)
 - Allocate chunks aligned at chunk size (so 64k aligned at 64k). This may
   require allocating twice the size, doing pointer arithmetic to find the
   aligned piece, and returning the unused portion of the allocation.
 - Put metadata at the beginning:
   - Size
   - Used bitmap
   - Prev / next ptr. "free list" becomes a list of chunks
 - Return memory to OS: When the used bitmap shows nothing used, munmap the chunk
