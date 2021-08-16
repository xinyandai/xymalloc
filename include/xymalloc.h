//
// Created by xinyan on 2021/8/4.
//
#pragma once
#ifndef XYMALLOC_XYMALLOC_H
#define XYMALLOC_XYMALLOC_H
#include "os.h"
#include "init.h"
#include "page.h"
#include "xymalloc_macro.h"
#include "xymalloc_internal.h"
#include "xymalloc_structs.h"
#include "xymalloc_attribute.h"


// ------------------------------------------------------
// Standard malloc interface
// ------------------------------------------------------

xy_decl_warm_unused xy_decl_export void* xy_malloc(size_t size) xy_attr_noexcept xy_attr_malloc xy_attr_alloc_size(1);
xy_decl_warm_unused xy_decl_export void* xy_calloc(size_t count, size_t size) xy_attr_noexcept xy_attr_malloc xy_attr_alloc_size2(1, 2);
xy_decl_warm_unused xy_decl_export void* xy_realloc(void* p, size_t newsize) xy_attr_noexcept xy_attr_alloc_size(2);
xy_decl_export void* xy_expand(void* p, size_t new_size) xy_attr_noexcept xy_attr_alloc_size(2);

xy_decl_export void xy_free(void* p) xy_attr_noexcept;

#endif //XYMALLOC_XYMALLOC_H
