//
// Created by xinyan on 2021/8/4.
//
#pragma once
#ifndef XYMALLOC_XY_MALLOC_STRUCTS_H
#define XYMALLOC_XY_MALLOC_STRUCTS_H

#include <stddef.h>
#include <stdint.h>
#include <atomic>
#include "xy_list.h"

using std::atomic;

typedef struct xy_block_s {
  struct xy_block_s * next;
} xy_block_t;



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
// - To limit the structure size, the `x_block_size` is 32-bits only; for
//   blocks > MI_HUGE_BLOCK_SIZE the size is determined from the segment page size
// - `thread_free` uses the bottom bits as a delayed-free flags to optimize
//   concurrent frees where only the first concurrent free adds to the owning
//   heap `thread_delayed_free` list (see `alloc.c:mi_free_block_mt`).
//   The invariant is that no-delayed-free is only set if there is
//   at least one block that will be added, or as already been added, to
//   the owning heap `thread_delayed_free` list. This guarantees that pages
//   will be freed correctly even if only other threads free blocks.
typedef struct xy_page_s {
  // "owned" by the segment
  xy_list_node          page_queue;
  // layout like this to optimize access in `mi_malloc` and `mi_free`
  uint32_t              used;              // number of blocks in use (including blocks in `local_free` and `thread_free`)
  uint32_t              x_block_size;      // size available in each block (always `>0`)

  xy_block_t*           free;              // list of available free blocks (`malloc` allocates from this list)
  atomic<uintptr_t>     other_free;              // list of deferred free blocks freed by other threads
  atomic<uintptr_t>     x_heap;

} xy_page_t;



// Segments are large allocated memory blocks (2mb on 64 bit) from
// the OS. Inside segments we allocated fixed size _pages_ that
// contain blocks.
typedef struct xy_segment_s {
  xy_list_node         segment_queue;
  xy_page_t*           free;
  size_t               used;             // count of pages in use (`used <= capacity`)
  size_t               capacity;         // count of available pages (`#free + used`)
  size_t               segment_size;     // for huge pages this may be different from `MI_SEGMENT_SIZE`
  atomic<uintptr_t>    thread_id;        // unique id of the thread owning this segment
  xy_page_t            pages[1];         // up to `MI_SMALL_PAGES_PER_SEGMENT` pages
} xy_segment_t;

// ------------------------------------------------------
// Heaps
// Provide first-class heaps to allocate from.
// A heap just owns a set of pages for allocation and
// can only be allocate/reallocate from the thread that created it.
// Freeing blocks can be done from any thread though.
// Per thread, the segments are shared among its heaps.
// Per thread, there is always a default heap that is
// used for allocation; it is initialized to statically
// point to an empty heap to avoid initialization checks
// in the fast path.
// ------------------------------------------------------



#define XY_SMALL_WSIZE_MAX  (128)
#define XY_SMALL_SIZE_MAX   (XY_SMALL_WSIZE_MAX*sizeof(void*))
#define XY_PAGES_DIRECT   ((XY_SMALL_SIZE_MAX + sizeof(uintptr_t) - 1) / sizeof(uintptr_t))
// A heap owns a set of pages.
typedef struct xy_heap_s {
  xy_list_node          pages[XY_PAGES_DIRECT];
  xy_page_t*            cache[XY_PAGES_DIRECT];
  atomic<xy_block_t*>   thread_delayed_free;
  uintptr_t             thread_id;                           // thread this heap belongs too
  xy_list_node          free_segment_queue;
  xy_list_node          full_segment_queue;

} xy_heap_t;

#endif //XYMALLOC_XY_MALLOC_STRUCTS_H
