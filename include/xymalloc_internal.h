//
// Created by xinyan on 2021/8/4.
//
#pragma once
#ifndef XYMALLOC_XYMALLOC_INTERNAL_H
#define XYMALLOC_XYMALLOC_INTERNAL_H
// Align a byte size to a size in _machine words_,
// i.e. byte size == `wsize*sizeof(void*)`.
static inline size_t _xy_idx_from_size(size_t size) {
    return (size + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);
}
#endif //XYMALLOC_XYMALLOC_INTERNAL_H
