//
// Created by xinyan on 2021/8/4.
//

#include <stdlib.h>
#include "xymalloc.h"

// todo ??????




// Allocate a page
// Note: in debug mode the size includes MI_PADDING_SIZE and might have overflowed.
static xy_page_t * xy_find_page(xy_heap_t* heap, size_t size) xy_attr_noexcept {
//  // huge allocation?
//  if (xy_unlikely(size > XY_MEDIUM_OBJ_SIZE_MAX )) {
//    return mi_large_huge_page_alloc(heap,size);
//  }
//  else {
//    // otherwise find a page with free blocks in our size segregated queues
//    return mi_find_free_page(heap, size);
//  }
}

// Fast allocation in a page: just pop from the free list.
// Fall back to generic allocation only if the list is empty.
extern inline void* _xy_page_malloc(xy_page_t* page) xy_attr_noexcept {
  xy_block_t* block = page->free;
  xy_assert(block != NULL);
  page->used++;
  page->free = block->next;
  return block;
}

// A huge page is allocated directly without being in a queue
static xy_page_t* xy_huge_page_alloc(xy_heap_t* heap, size_t size) {
  size_t block_size = _xy_idx_from_size(size) * sizeof(uintptr_t);
  xy_assert(_mi_bin(block_size) == XY_BIN_HUGE);
  xy_page_queue_t* pq = xy_page_queue(heap,block_size);
  xy_page_t* page = xy_page_fresh_alloc(heap,pq,block_size);
  return page;
}

static inline void* _xy_malloc_generic(xy_heap_t* heap, size_t size) {
  // find (or allocate) a page of the right size
  xy_page_t* page  = NULL;
  if (xy_unlikely(size > XY_LARGE_SIZE_MAX)) {
    page = xy_huge_page_alloc(heap, size);
  }
  else { // first time out of memory, try to collect and retry the allocation once more
    page = xy_find_page(heap, size);
    if  (xy_unlikely(page == NULL)) {
      // xy_heap_collect(heap, true /* force */);
      page = xy_find_page(heap, size);
    }
  }

  if (xy_unlikely(page == NULL)) { // out of memory
    // printf("unable to allocate memory (%zu bytes)\n", size);
    return NULL;
  }

  // and try again, this time succeeding! (i.e. this should never recurse)
  return _xy_page_malloc(page);
}


extern inline void* xy_malloc(size_t size) xy_attr_noexcept {
  // access small cached block
  if (xy_likely(size <= XY_SMALL_SIZE_MAX)) {
    // small block allocation
    size_t idx = _xy_idx_from_size(size);
    xy_page_t * page = _xy_tl_heap.pages_cached[idx];
    if (xy_likely(page->free != NULL)) {
      return _xy_page_malloc(page);
    }
  }

  // large block allocation
  return _xy_malloc_generic(&_xy_tl_heap, size);
}