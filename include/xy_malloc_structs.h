//
// Created by xinyan on 2021/8/4.
//
#pragma once
#ifndef XYMALLOC_XY_MALLOC_STRUCTS_H
#define XYMALLOC_XY_MALLOC_STRUCTS_H

#include <atomic>

using std::atomic;

// A page contains blocks of one specific size (`block_size`).
// Each page has three list of free blocks:
// `free` for blocks that can be allocated,
// `local_free` for freed blocks that are not yet available to `mi_malloc`
// `thread_free` for freed blocks by other threads
// The `local_free` and `thread_free` lists are migrated to the `free` list
// when it is exhausted. The separate `local_free` list is necessary to
// implement a monotonic heartbeat. The `thread_free` list is needed for
// avoiding atomic operations in the common case.
//
//
// `used - |thread_free|` == actual blocks that are in use (alive)
// `used - |thread_free| + |free| + |local_free| == capacity`
//
// We don't count `freed` (as |free|) but use `used` to reduce
// the number of memory accesses in the `mi_page_all_free` function(s).
//
// Notes:
// - Access is optimized for `mi_free` and `mi_page_alloc` (in `alloc.c`)
// - Using `uint16_t` does not seem to slow things down
// - The size is 8 words on 64-bit which helps the page index calculations
//   (and 10 words on 32-bit, and encoded free lists add 2 words. Sizes 10
//    and 12 are still good for address calculation)
// - To limit the structure size, the `xblock_size` is 32-bits only; for
//   blocks > MI_HUGE_BLOCK_SIZE the size is determined from the segment page size
// - `thread_free` uses the bottom bits as a delayed-free flags to optimize
//   concurrent frees where only the first concurrent free adds to the owning
//   heap `thread_delayed_free` list (see `alloc.c:mi_free_block_mt`).
//   The invariant is that no-delayed-free is only set if there is
//   at least one block that will be added, or as already been added, to
//   the owning heap `thread_delayed_free` list. This guarantees that pages
//   will be freed correctly even if only other threads free blocks.
struct xy_page_s;
typedef struct xy_page_s xy_page_t;
struct xy_page_s {
    // "owned" by the segment
    uint8_t                     segment_idx;       // index in the segment `pages` array, `page == &segment->pages[page->segment_idx]`
    uint8_t                     segment_in_use:1;  // `true` if the segment allocated this page
    uint8_t                     is_reset:1;        // `true` if the page memory was reset
    uint8_t                     is_committed:1;    // `true` if the page virtual memory is committed
    uint8_t                     is_zero_init:1;    // `true` if the page was zero initialized

    // layout like this to optimize access in `mi_malloc` and `mi_free`
    uint16_t                    capacity;          // number of blocks committed, must be the first field, see `segment.c:page_clear`
    uint16_t                    reserved;          // number of blocks reserved in memory
    mi_page_flags_t             flags;             // `in_full` and `has_aligned` flags (8 bits)
    uint8_t                     is_zero:1;         // `true` if the blocks in the free list are zero initialized
    uint8_t                     retire_expire:7;   // expiration count for retired blocks

    mi_block_t*                 free;              // list of available free blocks (`malloc` allocates from this list)

    uint32_t                    used;              // number of blocks in use (including blocks in `local_free` and `thread_free`)
    uint32_t                    xblock_size;       // size available in each block (always `>0`)

    mi_block_t*                 local_free;        // list of deferred free blocks by this thread (migrates to `free`)
    atomic<mi_thread_free_t>    xthread_free;      // list of deferred free blocks freed by other threads
    atomic<uintptr_t>           xheap;

    xy_page_t*                  next;              // next page owned by this thread with the same `block_size`
    xy_page_t*                  prev;              // previous page owned by this thread with the same `block_size`
};



// A heap owns a set of pages.
struct xy_heap_s;
typedef struct xy_heap_s xy_heap_t;
struct xy_heap_s  {
    xy_page_t*            pages_free_direct[MI_PAGES_DIRECT];  // optimize: array where every entry points a page with possibly free blocks in the corresponding queue for that size.
    mi_page_queue_t       pages[MI_BIN_FULL + 1];              // queue of pages for each size class (or "bin")
    atomic<mi_block_t*>   thread_delayed_free;
    uintptr_t             thread_id;                           // thread this heap belongs too
    size_t                page_count;                          // total number of pages in the `pages` queues.
    size_t                page_retired_min;                    // smallest retired index (retired pages are fully free, but still in the page queues)
    size_t                page_retired_max;                    // largest retired index into the `pages` array.
    xy_heap_t*            next;                                // list of heaps per thread
    bool                  no_reclaim;                          // `true` if this heap should not reclaim abandoned pages
} ;
#endif //XYMALLOC_XY_MALLOC_STRUCTS_H
