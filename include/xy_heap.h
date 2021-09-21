//
// Created by xydai on 2021/9/20.
//

#ifndef XYMALLOC_INCLUDE_XY_HEAP_H_
#define XYMALLOC_INCLUDE_XY_HEAP_H_
#include "xy_page.h"

static inline size_t _xy_idx_from_size(size_t size) {
  return (size + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);
}

static inline xy_page_t* find_page(xy_page_t* head) {
  xy_page_t* page = head;
  do {
    page = (xy_page_t*)page->page_queue.next;
  } while (page != head && !page->free);
  return page == head ? NULL : page;
}

void* xy_heap_malloc(size_t size, xy_heap_t* heap) {
  size_t idx = _xy_idx_from_size(size);
  xy_page_t* page = heap->cache[idx];
  if xy_unlikely (page == NULL || page->free == NULL) {
    heap->cache[idx] = page = find_page((xy_page_t*)&heap->pages[idx]);
  }

  /// page is not set or page is used up
  if xy_unlikely (page == NULL || page->free == NULL) {
    xy_segment_t* segment = _xy_free_segment(heap);
    xy_page_t* new_page = xy_segment_malloc_page(segment);
    xy_list_add_head((xy_list_node*)&heap->pages[idx], (xy_list_node*)new_page);

    /// TODO
    /// init new_page
    page = heap->cache[idx] = new_page;
  }

  return xy_page_malloc_block(page);
}

int xy_heap_free(void* block) {
  /// TODO
  return xy_page_free_block((xy_block_t*)block);
}
#endif  // XYMALLOC_INCLUDE_XY_HEAP_H_
