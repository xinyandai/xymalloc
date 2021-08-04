//
// Created by xinyan on 2021/8/4.
//
#pragma once
#ifndef XYMALLOC_XYMALLOC_ATTRIBUTE_H
#define XYMALLOC_XYMALLOC_ATTRIBUTE_H



#define XY_CACHE_LINE          64
#if (defined(__GNUC__) && (__GNUC__>=3))  // includes clang and icc
#define xy_decl_noinline        __attribute__((noinline))
#define xy_decl_cache_align     __attribute__((aligned(XY_CACHE_LINE)))
#else
#define xy_decl_noinline
#define xy_decl_cache_align
#endif

// ------------------------------------------------------
// warn_unused_result
// ------------------------------------------------------
#if defined(__cplusplus) && (__cplusplus >= 201703)
  #define xy_decl_warm_unused    [[nodiscard]]
#elif (__GNUC__ >= 4) || defined(__clang__)  // includes clang, icc, and clang-cl
  #define xy_decl_warm_unused    __attribute__((warn_unused_result))
#elif (_MSC_VER >= 1700)
  #define xy_decl_warm_unused    _Check_return_
#else
  #define xy_decl_warm_unused
#endif

// ------------------------------------------------------
// warn_unused_result
// ------------------------------------------------------
#ifdef __cplusplus
  #if (__cplusplus >= 201103L) || (_MSC_VER > 1900)  // C++11
    #define xy_attr_noexcept   noexcept
  #else
    #define xy_attr_noexcept   throw()
  #endif
#else
  #define xy_attr_noexcept
#endif



#if defined(__GNUC__)                 // includes clang and icc
  #define xy_decl_export                __attribute__((visibility("default")))
  #define xy_attr_malloc                __attribute__((malloc))
  #if (defined(__clang_major__) && (__clang_major__ < 4)) || (__GNUC__ < 5)
    #define xy_attr_alloc_size(s)
    #define xy_attr_alloc_size2(s1,s2)
    #define xy_attr_alloc_align(p)
  #elif defined(__INTEL_COMPILER)
    #define xy_attr_alloc_size(s)       __attribute__((alloc_size(s)))
    #define xy_attr_alloc_size2(s1,s2)  __attribute__((alloc_size(s1,s2)))
    #define xy_attr_alloc_align(p)
  #else
    #define xy_attr_alloc_size(s)       __attribute__((alloc_size(s)))
    #define xy_attr_alloc_size2(s1,s2)  __attribute__((alloc_size(s1,s2)))
    #define xy_attr_alloc_align(p)      __attribute__((alloc_align(p)))
  #endif
#else
  #define xy_decl_export
  #define xy_attr_malloc
  #define xy_attr_alloc_size(s)
  #define xy_attr_alloc_size2(s1,s2)
  #define xy_attr_alloc_align(p)
#endif
#endif //XYMALLOC_XYMALLOC_ATTRIBUTE_H
