//
// Created by xinyan on 2021/8/13.
//

#ifndef XYMALLOC_INCLUDE_PAGE_H_
#define XYMALLOC_INCLUDE_PAGE_H_

#include "xymalloc_structs.h"

xy_page_t* xy_page_fresh_alloc(xy_heap_t* heap, xy_page_queue_t* pq, size_t block_size);

#endif //XYMALLOC_INCLUDE_PAGE_H_
