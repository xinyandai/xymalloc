//
// Created by xinyan on 2021/8/6.
//
#pragma once
#ifndef XYMALLOC_INCLUDE_XY_OS_H_
#define XYMALLOC_INCLUDE_XY_OS_H_
#include "xy_macro.h"
size_t xy_get_numa_node();

int xy_munmap(void *addr, size_t length);

void* xy_mmap(void* addr, size_t size, bool try_large, bool* is_large);
void* xy_mmap_huge(void* addr, size_t size);
void* xy_mmap_numa(void* addr, size_t size, int numa_node, bool try_large);

// -------------------------------------------------------------------
// Getting the thread id should be performant
// as it is called in the fast path of `_mi_free`,
// so we specialize for various platforms.
// -------------------------------------------------------------------
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
static inline uintptr_t _xy_thread_id(void) mi_attr_noexcept {
  // Windows: works on Intel and ARM in both 32- and 64-bit
  return (uintptr_t)NtCurrentTeb();
}
#elif (defined(__GNUC__) || defined(__clang__)) && \
(defined(__x86_64__) || defined(__i386__) || defined(__arm__) || defined(__aarch64__))
// TLS register on x86 is in the FS or GS register
// see: https://akkadia.org/drepper/tls.pdf
inline uintptr_t _xy_thread_id(void) xy_attr_noexcept {
uintptr_t tid;
#if defined(__i386__)
__asm__("movl %%gs:0, %0" : "=r" (tid) : : );  // 32-bit always uses GS
#elif defined(__MACH__)
__asm__("movq %%gs:0, %0" : "=r" (tid) : : );  // x86_64 macOS uses GS
#elif defined(__x86_64__)
__asm__("movq %%fs:0, %0" : "=r" (tid) : : );  // x86_64 Linux, BSD uses FS
#elif defined(__arm__)
asm volatile ("mrc p15, 0, %0, c13, c0, 3" : "=r" (tid));
  #elif defined(__aarch64__)
  asm volatile ("mrs %0, tpidr_el0" : "=r" (tid));
#endif
return tid;
}
#else
// otherwise use standard C
static inline uintptr_t _xy_thread_id(void) xy_attr_noexcept {
  static_assert(0);
}
#endif

#endif //XYMALLOC_INCLUDE_XY_OS_H_
