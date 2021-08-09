//
// Created by xinyan on 2021/8/4.
//

#include <stdlib.h>
#include "xymalloc.h"

// todo ??????
xy_decl_cache_align const xy_heap_t _xy_heap_empty = {};
__thread xy_heap_t* _tl_heap = (xy_heap_t*)&_xy_heap_empty;


static inline void* _mi_malloc_generic(xy_heap_t* _tl_heap, size_t size) {
}


extern inline void* xy_malloc(size_t size) xy_attr_noexcept {
  if (xy_likely(size <= XY_SMALL_SIZE_MAX)) {
    size_t idx = _xy_idx_from_size(size);
    xy_page_t * page = _tl_heap->pages_free_direct[idx];
    xy_block_t* block = page->free;
    if (block == NULL) {
      return _mi_malloc_generic(_tl_heap, size);
    }
    page->used++;
    page->free = block->next;
  }
  else {
    return _mi_malloc_generic(_tl_heap, size);
  }
}