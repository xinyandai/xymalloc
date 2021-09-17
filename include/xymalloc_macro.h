//
// Created by xinyan on 2021/8/4.
//

#ifndef XYMALLOC_XYMALLOC_MACRO_H
#define XYMALLOC_XYMALLOC_MACRO_H
#include <stdint.h>
#include <stddef.h>

#define KiB     ((size_t)1024)
#define MiB     (KiB*KiB)
#define GiB     (MiB*KiB)


#define XY_PAGES_DIRECT   ((XY_SMALL_SIZE_MAX + sizeof(uintptr_t) - 1) / sizeof(uintptr_t))

#if defined(__GNUC__) || defined(__clang__)
#define xy_unlikely(x)     (__builtin_expect(!!(x), 0))
#define xy_likely(x)       (__builtin_expect(!!(x), 1))
#else
#define xy_unlikely(x)     (x)
#define xy_likely(x)       (x)
#endif

#define XY_DEBUG 1
#if (XY_DEBUG)
// use our own assertion to print without memory allocation
void _xy_assert_fail(const char* assertion, const char* fname, unsigned int line, const char* func );
#define xy_assert(expr)     ((expr) ? (void)0 : _xy_assert_fail(#expr, __FILE__, __LINE__, __func__))
#else
#define xy_assert(x)
#endif

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

#endif //XYMALLOC_XYMALLOC_MACRO_H
