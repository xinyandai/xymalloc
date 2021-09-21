//
// Created by xydai on 2021/9/20.
//

#ifndef XYMALLOC_INCLUDE_XY_SEGMENT_H_
#define XYMALLOC_INCLUDE_XY_SEGMENT_H_
#include <sys/mman.h>

#include "xy_list.h"
#include "xy_os.h"
#include "xy_structs.h"

xy_segment_t* malloc_segment(void* addr) {
  int numa_node = xy_get_numa_node();
  return (xy_segment_t*)xy_mmap_numa(addr, XY_SEGMENT_SIZE, numa_node, true);
}


static inline xy_segment_t* _xy_free_segment(xy_heap_t* heap) {
  int queue_empty = xy_list_is_empty(&heap->free_segment_queue);
  auto segment = (xy_segment_t*)heap->free_segment_queue.next;

  if xy_unlikely (queue_empty || segment->free == 0) {
    if (!queue_empty) {  /// <=> segment->free == 0
      xy_list_del((xy_list_node*)segment);
      xy_list_add_head(&heap->full_segment_queue, (xy_list_node*)segment);
    }
    segment = malloc_segment(NULL);
    /// TODO
    /// init segment
    xy_list_add_head(&heap->free_segment_queue, (xy_list_node*)segment);
  }

  return segment;
}

/**
 *
 * \param segment
 * \return
 */
xy_page_t* xy_segment_malloc_page(xy_segment_t* segment) {
  xy_page_t* page = segment->free;
  segment->used++;
  segment->free = (xy_page_t*)segment->free->page_queue.next;
  return page;
}

/**
 *
 * \param page
 * \return
 */
int xy_segment_free_page(xy_page_t* page) {
  xy_segment_t* segment = xy_page_to_segment(page);
  page->page_queue.next = (xy_list_node*)segment->free;
  segment->free = page;

  if xy_unlikely (--page->used == 0) {
    xy_list_del((xy_list_node*)segment);
    xy_munmap(segment, segment->segment_size);
  }
  return 0;
}

#endif  // XYMALLOC_INCLUDE_XY_SEGMENT_H_
