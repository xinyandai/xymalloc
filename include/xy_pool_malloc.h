//
// Created by xydai on 2021/9/16.
//
#pragma once
#ifndef XYMALLOC_INCLUDE_XY_POOL_MALLOC_H_
#define XYMALLOC_INCLUDE_XY_POOL_MALLOC_H_

#include "xy_macro.h"
#include "xymalloc_attribute.h"

class pool_allocator {
 private:
  size_t size_;
 public:
  pool_allocator(size_t size): size_(size) {}

  void *malloc() {};
  void free(void *p) {};
};

#endif //XYMALLOC_INCLUDE_XY_POOL_MALLOC_H_
