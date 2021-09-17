//
// Created by xinyan on 2021/8/5.
//
#include <atomic>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>    // sysconf
#include <features.h>
#include <sys/mman.h>  // mmap
#include <linux/mman.h> // linux mmap flags
#include <sys/syscall.h>


#include "xy_os.h"
#include "xymalloc_macro.h"

#ifndef MPOL_PREFERRED
#define MPOL_PREFERRED 1
#endif

static size_t xy_get_numa_node() {
  unsigned long node = 0;
  unsigned long ncpu = 0;
  long err = syscall(SYS_getcpu, &ncpu, &node, NULL);
  if (err != 0) return 0;
  return node;
}
/// mbind sets the NUMA memory policy for the memory range specified by start
/// and length len. The memory policy defines which node memory is allocated.
/// The policy has no effect on ranges in which memory has already been
/// allocated.  nmask is a bit field of nodes that contains upto maxnode
/// bits.  The node mask is rounded to the next multiple of sizeof (unsigned
/// long), but the kernel will only use bits upto maxnode. The flags
/// parameter is unused and only 0 should be passed in for its value.  The
/// available policies set in mode are as follows:
static long xy_mbind(void* start, unsigned long len, unsigned long mode, const unsigned long* nmask, unsigned long maxnode, unsigned flags) {
  return syscall(SYS_mbind, start, len, mode, nmask, maxnode, flags);
}

void* xy_mmap(void* addr, size_t size, bool try_large, bool* is_large) {
  int protect_flags = PROT_READ | PROT_WRITE;
  int flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE;

  void* p = mmap(addr, size, protect_flags, flags, -1, 0);
  /// Many Linux systems don't allow MAP_HUGETLB but they support instead
  /// transparent huge pages (THP). It is not required to call `madvise` with MADV_HUGE
  /// though since properly aligned allocations will already use large pages if available
  /// in that case -- in particular for our large regions (in `memory.c`).
  /// However, some systems only allow THP if called with explicit `madvise`, so
  /// when large OS pages are enabled, we call `madvise` anyways.
  if (try_large) {
    if (madvise(p, size, MADV_HUGEPAGE) == 0 && is_large) {
      *is_large = true; // possibly
    }
  }

  return p;
}

void* xy_mmap_huge(void* addr, size_t size) {
  void* p = MAP_FAILED;
  int protect_flags = PROT_READ | PROT_WRITE;
  int flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB;  // using NORESERVE on huge pages seems to fail on Linux

  static bool huge_pages_available = true;

  if ((size % GiB) == 0 && huge_pages_available) {
    flags |= MAP_HUGE_1GB;
  } else {
    flags |= MAP_HUGE_2MB;
  }

  // try large OS page allocation
  p = mmap(addr, size, protect_flags, flags, -1, 0);
  if xy_unlikely(p == MAP_FAILED && (flags & MAP_HUGE_1GB) != 0) { // failed to allocate 1GiB page
    huge_pages_available = false; // don't try huge 1GiB pages again
    flags = ((flags & ~MAP_HUGE_1GB) | MAP_HUGE_2MB);
    p = mmap(addr, size, protect_flags, flags, -1, 0);
  }

  return p;
}

void* xy_mmap_numa(void* addr, size_t size, int numa_node, bool try_large) {
  bool is_large = false;
  void* p = xy_mmap(addr, size, try_large, &is_large);
  if (p == MAP_FAILED)
    return MAP_FAILED;
  if (numa_node >= 0 && numa_node < 64) { // at most 64 nodes
    unsigned long int numa_mask = (1UL << numa_node);
    // TODO: does `mbind` work correctly for huge OS pages? should we
    // use `set_mempolicy` before calling mmap instead?
    // see: <https://lkml.org/lkml/2017/2/9/875>
    long err = xy_mbind(p, size, MPOL_PREFERRED, &numa_mask, 64, 0);
    if (err != 0) {
     printf("failed to bind huge (1gb) pages to numa node %d\n", numa_node);
    }
  }
  return p;
}


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
static inline uintptr_t _xy_thread_id(void) mi_attr_noexcept {
  return (uintptr_t)&_mi_heap_default;
}
#endif