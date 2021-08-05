//
// Created by xinyan on 2021/8/5.
//
#include <atomic>
#include <stddef.h>
#include <unistd.h>    // sysconf
#include <features.h>
#include <sys/mman.h>  // mmap
#include <linux/mman.h> // linux mmap flags
#include <sys/syscall.h>

#include "xymalloc.h"

#ifndef MPOL_PREFERRED
#define MPOL_PREFERRED 1
#endif


static long mi_os_mbind(void* start, unsigned long len, unsigned long mode, const unsigned long* nmask, unsigned long maxnode, unsigned flags) {
  return syscall(SYS_mbind, start, len, mode, nmask, maxnode, flags);
}

static void* xy_mmap(void* addr, size_t size, int protect_flags, bool large_only, bool allow_large, bool* is_large) {
  void* p = NULL;

  int flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE;
  int fd = -1;

  if (large_only) {
    static std::atomic<uintptr_t> large_page_try_ok; // = 0;
    static bool mi_huge_pages_available = true;

    uintptr_t try_ok = large_page_try_ok.load(std::memory_order_acquire);
    if (try_ok > 0) {
      // If the OS is not configured for large OS pages, or the user does not have
      // enough permission, the `mmap` will always fail (but it might also fail for other reasons).
      // Therefore, once a large page allocation failed, we don't try again for `large_page_try_ok` times
      // to avoid too many failing calls to mmap.
      while(!large_page_try_ok.compare_exchange_strong(try_ok, try_ok - 1));
    }

    int large_flags = flags & ~MAP_NORESERVE;  // using NORESERVE on huge pages seems to fail on Linux
    large_flags |= MAP_HUGETLB;

    if ((size % GiB) == 0 && mi_huge_pages_available) {
      large_flags |= MAP_HUGE_1GB;
    } else {
      large_flags |= MAP_HUGE_2MB;
    }

    // try large OS page allocation
    *is_large = true;
    p = mmap(addr, size, protect_flags, large_flags, fd, 0);
    if (p == NULL && (large_flags & MAP_HUGE_1GB) != 0) { // failed to allocate 1GiB page
      mi_huge_pages_available = false; // don't try huge 1GiB pages again
      large_flags = ((large_flags & ~MAP_HUGE_1GB) | MAP_HUGE_2MB);
      p = mmap(addr, size, protect_flags, large_flags, fd, 0);
    }

    if (p == NULL) {
      large_page_try_ok.store(uintptr_t(10));  // on error, don't try again for the next N allocations
    }
    return p;
  }
  if (p == NULL) {
    *is_large = false;
    p = mmap(addr, size, protect_flags, flags, fd, 0);
    // Many Linux systems don't allow MAP_HUGETLB but they support instead
    // transparent huge pages (THP). It is not required to call `madvise` with MADV_HUGE
    // though since properly aligned allocations will already use large pages if available
    // in that case -- in particular for our large regions (in `memory.c`).
    // However, some systems only allow THP if called with explicit `madvise`, so
    // when large OS pages are enabled for mimalloc, we call `madvise` anyways.
    if (allow_large) {
      if (madvise(p, size, MADV_HUGEPAGE) == 0) {
        *is_large = true; // possibly
      }
    }
  }
  return p;
}


static void* mi_os_alloc_huge_os_pagesx(void* addr, size_t size, int numa_node) {
  bool is_large = true;
  void* p = xy_mmap(addr, size, PROT_READ | PROT_WRITE, true, true, &is_large);
  if (p == NULL) return NULL;
  if (numa_node >= 0 && numa_node < 64) { // at most 64 nodes
    uintptr_t numa_mask = (1UL << numa_node);
    // TODO: does `mbind` work correctly for huge OS pages? should we
    // use `set_mempolicy` before calling mmap instead?
    // see: <https://lkml.org/lkml/2017/2/9/875>
    long err = mi_os_mbind(p, size, MPOL_PREFERRED, &numa_mask, 64, 0);
    if (err != 0) {
     // todo error("failed to bind huge (1gb) pages to numa node %d: %s\n", numa_node);
    }
  }
  return p;
}