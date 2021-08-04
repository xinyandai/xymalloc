//
// Created by xinyan on 2021/8/4.
//

#ifndef XYMALLOC_XYMALLOC_MACRO_H
#define XYMALLOC_XYMALLOC_MACRO_H

#define XY_SMALL_SIZE_MAX   (128 * sizeof(void*))

#if defined(__GNUC__) || defined(__clang__)
#define xy_unlikely(x)     __builtin_expect(!!(x), 0)
#define xy_likely(x)       __builtin_expect(!!(x), 1)
#else
#define xy_unlikely(x)     (x)
#define xy_likely(x)       (x)
#endif


#endif //XYMALLOC_XYMALLOC_MACRO_H
