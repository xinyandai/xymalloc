//
// Created by xinyan on 2021/8/11.
//
#pragma once
#ifndef XYMALLOC_INCLUDE_INIT_H_
#define XYMALLOC_INCLUDE_INIT_H_
#include "xymalloc_macro.h"
#include "xymalloc_structs.h"
#include "xymalloc_attribute.h"

extern __thread xy_decl_cache_align xy_heap_t _xy_tl_heap;

#endif //XYMALLOC_INCLUDE_INIT_H_
