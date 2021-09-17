//
// Created by xinyan on 2021/8/6.
//
#pragma once
#ifndef XYMALLOC_INCLUDE_XY_OS_H_
#define XYMALLOC_INCLUDE_XY_OS_H_

void* xy_mmap(void* addr, size_t size, bool try_large, bool* is_large);
void* xy_mmap_huge(void* addr, size_t size);
void* xy_mmap_numa(void* addr, size_t size, int numa_node, bool try_large);

#endif //XYMALLOC_INCLUDE_XY_OS_H_
