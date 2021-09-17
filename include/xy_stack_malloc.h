//
// Created by xydai on 2021/9/16.
//
#pragma once
#ifndef XYMALLOC_INCLUDE_XY_STACK_MALLOC_H_
#define XYMALLOC_INCLUDE_XY_STACK_MALLOC_H_
#include "xymalloc_macro.h"

class stack_allocator {
 private:
  char* buffer_;
 public:
  void *malloc(size_t size) {};
  void *realloc(void *p, size_t new_size) {};
  void free(void *p) {};
};

#endif // XYMALLOC_INCLUDE_XY_STACK_MALLOC_H_
