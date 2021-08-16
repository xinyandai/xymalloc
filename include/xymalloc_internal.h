//
// Created by xinyan on 2021/8/4.
//
#pragma once
#ifndef XYMALLOC_XYMALLOC_INTERNAL_H
#define XYMALLOC_XYMALLOC_INTERNAL_H
// Align a byte size to a size in _machine words_,
// i.e. byte size == `wsize*sizeof(void*)`.
inline size_t _xy_idx_from_size(size_t size) {
    return (size + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);
}

static inline uint8_t mi_bsr32(uint32_t x) {
  return (31 - __builtin_clz(x));
}
#define MI_LARGE_PAGE_SIZE                (4 * MiB)
#define MI_LARGE_SIZE_MAX                 (MI_LARGE_PAGE_SIZE/8)   // 512kb on 64-bit
#define MI_LARGE_WSIZE_MAX                (MI_LARGE_SIZE_MAX >> sizeof(void*) )
// Return the bin for a given field size.
// Returns MI_BIN_HUGE if the size is too large.
// We use `wsize` for the size in "machine word sizes",
// i.e. byte size == `wsize*sizeof(void*)`.
inline uint8_t _mi_bin(size_t size) {
  size_t word_size = _xy_idx_from_size(size);
  uint8_t bin;
  if (word_size <= 1) {
    bin = 1;
  }
  else if (word_size <= 8) {
    bin = (uint8_t) word_size;
  }
  else if (word_size > MI_LARGE_WSIZE_MAX) {
    bin = XY_BIN_HUGE;
  }
  else {
    word_size--;
    // find the highest bit
    uint8_t b = mi_bsr32((uint32_t)word_size);
    // and use the top 3 bits to determine the bin (~16% worst internal fragmentation).
    // - adjust with 3 because we use do not round the first 8 sizes
    //   which each get an exact bin
    bin = ((b << 2) + (uint8_t)((word_size >> (b - 2)) & 0x03)) - 3;
  }
  xy_assert(bin > 0 && bin <= XY_BIN_HUGE);
  return bin;
}

static inline xy_page_queue_t* xy_page_queue(const xy_heap_t* heap, size_t size) {
  return &((xy_heap_t*)heap)->pages[_mi_bin(size)];
}
#endif //XYMALLOC_XYMALLOC_INTERNAL_H
