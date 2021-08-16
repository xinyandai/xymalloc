//
// Created by xinyan on 2021/8/4.
//

#ifndef XYMALLOC_XYMALLOC_MACRO_H
#define XYMALLOC_XYMALLOC_MACRO_H
#include <stdint.h>

#define KiB     ((size_t)1024)
#define MiB     (KiB*KiB)
#define GiB     (MiB*KiB)

#define XY_SMALL_SIZE_MAX   (1024)
#define XY_LARGE_SIZE_MAX   (512 * KiB)

#define XY_MEDIUM_OBJ_SIZE_MAX  (128 * KiB)
#define XY_LARGE_OBJ_SIZE_MAX   (2 * MiB)

#define XY_PAGES_DIRECT   ((XY_SMALL_SIZE_MAX + sizeof(uintptr_t) - 1) / sizeof(uintptr_t))

#if defined(__GNUC__) || defined(__clang__)
#define xy_unlikely(x)     __builtin_expect(!!(x), 0)
#define xy_likely(x)       __builtin_expect(!!(x), 1)
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

#endif //XYMALLOC_XYMALLOC_MACRO_H
