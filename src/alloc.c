//
// Created by xinyan on 2021/8/4.
//

#include <stdlib.h>
#include <stdint.h>
#include "xymalloc.h"
#include "xy_malloc_structs.h"

xy_decl_cache_align const xy_heap_t _xy_heap_empty;
__thread xy_heap_t* _tl_heap = (xy_heap_t*)&_xy_heap_empty;

extern inline void* xy_malloc(size_t size) xy_attr_noexcept {
    if (xy_likely(size <= XY_SMALL_SIZE_MAX)) {
        size_t idx = _xy_idx_from_size(size);
        xy_page_t * page = _tl_heap->pages_free_direct[idx];
    }
    else {

    }
}