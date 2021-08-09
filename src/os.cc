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

#include "os.h"
#include "xymalloc_macro.h"

#ifndef MPOL_PREFERRED
#define MPOL_PREFERRED 1
#endif


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

  *is_large = false;
  void* p = mmap(addr, size, protect_flags, flags, -1, 0);
  // Many Linux systems don't allow MAP_HUGETLB but they support instead
  // transparent huge pages (THP). It is not required to call `madvise` with MADV_HUGE
  // though since properly aligned allocations will already use large pages if available
  // in that case -- in particular for our large regions (in `memory.c`).
  // However, some systems only allow THP if called with explicit `madvise`, so
  // when large OS pages are enabled for mimalloc, we call `madvise` anyways.
  if (try_large) {
    if (madvise(p, size, MADV_HUGEPAGE) == 0) {
      *is_large = true; // possibly
    }
  }

  return p;
}

void* xy_mmap_huge(void* addr, size_t size) {
  void* p = MAP_FAILED;
  int protect_flags = PROT_READ | PROT_WRITE;
  int flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB;  // using NORESERVE on huge pages seems to fail on Linux
  int fd = -1;

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

  if ((size % GiB) == 0 && mi_huge_pages_available) {
    flags |= MAP_HUGE_1GB;
  } else {
    flags |= MAP_HUGE_2MB;
  }

  // try large OS page allocation
  p = mmap(addr, size, protect_flags, flags, fd, 0);
  if (p == MAP_FAILED && (flags & MAP_HUGE_1GB) != 0) { // failed to allocate 1GiB page
    mi_huge_pages_available = false; // don't try huge 1GiB pages again
    flags = ((flags & ~MAP_HUGE_1GB) | MAP_HUGE_2MB);
    p = mmap(addr, size, protect_flags, flags, fd, 0);
  }

  if (p == MAP_FAILED) {
    large_page_try_ok.store(uintptr_t(10));  // on error, don't try again for the next N allocations
  }
  return p;
}

void* xy_mmap_numa(void* addr, size_t size, int numa_node, bool try_large) {
  bool is_large = true;
  void* p = xy_mmap(addr, size, try_large, &is_large);
  if (p == MAP_FAILED)
    return MAP_FAILED;
  if (numa_node >= 0 && numa_node < 64) { // at most 64 nodes
    uintptr_t numa_mask = (1UL << numa_node);
    // TODO: does `mbind` work correctly for huge OS pages? should we
    // use `set_mempolicy` before calling mmap instead?
    // see: <https://lkml.org/lkml/2017/2/9/875>
    long err = xy_mbind(p, size, MPOL_PREFERRED, &numa_mask, 64, 0);
    if (err != 0) {
     printf("failed to bind huge (1gb) pages to numa node %d: %s\n", numa_node);
    }
  }
  return p;
}