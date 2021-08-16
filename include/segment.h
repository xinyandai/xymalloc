//
// Created by xinyan on 2021/8/13.
//

#ifndef XYMALLOC_INCLUDE_SEGMENT_H_
#define XYMALLOC_INCLUDE_SEGMENT_H_
#include "xymalloc_structs.h"

xy_page_t* _xy_segment_page_alloc(size_t block_size, mi_segments_tld_t* tld, mi_os_tld_t* os_tld) {
  mi_page_t* page;
  if (block_size < MI_SMALL_PAGE_SIZE / 8)
    // smaller blocks than 8kb (assuming MI_SMALL_PAGE_SIZE == 64kb)
    page = mi_segment_small_page_alloc(tld,os_tld);
  else if (block_size < (MI_LARGE_SIZE_MAX - sizeof(mi_segment_t)))
    page = mi_segment_large_page_alloc(tld, os_tld);
  else
    page = mi_segment_huge_page_alloc(block_size,tld,os_tld);
  mi_assert_expensive(mi_segment_is_valid(_mi_page_segment(page)));
  return page;
}

#endif //XYMALLOC_INCLUDE_SEGMENT_H_
