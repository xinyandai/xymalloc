#include "xymalloc.h"

// allocate a fresh page from a segment
xy_page_t* xy_page_fresh_alloc(xy_heap_t* heap, xy_page_queue_t* pq, size_t block_size) {
  xy_page_t* page = _xy_segment_page_alloc(block_size, &heap->tld->segments, &heap->tld->os);
  xy_page_init(heap, page, block_size, &heap->tld->stats);
  xy_page_queue_push(heap, pq, page);
  return page;
}