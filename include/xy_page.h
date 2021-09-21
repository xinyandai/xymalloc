//
// Created by xydai on 2021/9/20.
//

#ifndef XYMALLOC_INCLUDE_XY_PAGE_H_
#define XYMALLOC_INCLUDE_XY_PAGE_H_
#include <thread>
#include "xy_macro.h"
#include "xy_segment.h"
#include "xy_structs.h"

/**
 * called by local thread only.
 * \param page
 * \return
 */
static inline void* xy_page_malloc_block(xy_page_t* page) xy_attr_noexcept {
  xy_block_t* block = page->free;
  page->used++;
  page->free = page->free->next;
  return block;
}

/**
 * called by local thread only.
 * \param ptr
 * \return
 */
static inline int xy_page_free_block(xy_block_t* block) xy_attr_noexcept {
  auto page = xy_block_to_page(block);
  auto segment = xy_page_to_segment(page);
  if xy_likely (_xy_thread_id() == segment->thread_id) {
    block->next = page->free;
    page->free = block;

    if xy_unlikely (--page->used == 0) {
      xy_list_del((xy_list_node*)page);
      xy_segment_free_page(page);
    }
  } else {
    uintptr_t expected = page->other_free.load();
    uintptr_t desired = (uintptr_t) block;
    do {
      block->next = (xy_block_t*)expected;
    } while(!page->other_free.compare_exchange_strong(expected, desired));
    /// TODO
  }

  return 0;
}

#endif  // XYMALLOC_INCLUDE_XY_PAGE_H_
