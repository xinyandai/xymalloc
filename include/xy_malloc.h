//
// Created by xydai on 2021/9/16.
//
#pragma once
#ifndef XYMALLOC_INCLUDE_XY_MALLOC_H_
#define XYMALLOC_INCLUDE_XY_MALLOC_H_
#include "xy_macro.h"

xy_decl_warm_unused xy_decl_export void * xy_malloc(size_t size) xy_attr_noexcept xy_attr_malloc xy_attr_alloc_size(1);
xy_decl_warm_unused xy_decl_export void * xy_calloc(size_t count, size_t size) xy_attr_noexcept xy_attr_malloc xy_attr_alloc_size2(1, 2);
xy_decl_warm_unused xy_decl_export void * xy_realloc(void *p, size_t new_size) xy_attr_noexcept xy_attr_alloc_size(2);
xy_decl_export void xy_free(void *p) xy_attr_noexcept;

#endif //XYMALLOC_INCLUDE_XY_MALLOC_H_
