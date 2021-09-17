#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>

#include "xy_os.h"

#define LENGTH (1024UL*1024*1024)
#define STEP (4096)


static void write_bytes(char *addr) {
  unsigned long i;

  for (i = 0; i < LENGTH; i += STEP)
    *(addr + i) = (char)i;
}

static int read_bytes(char *addr) {
  unsigned long i;

  for (i = 0; i < LENGTH; i += STEP)
    if (*(addr + i) != (char)i) {
      printf("Mismatch at %lu\n", i);
      return 1;
    }
  return 0;
}

void __test_address(char* addr) {
  printf("Returned address is %p\n", addr);
  struct timeval starttv, endtv;
  gettimeofday(&starttv, NULL);
  write_bytes(addr);
  read_bytes(addr);
  gettimeofday(&endtv, NULL);
  printf("time cost : %ld us\n", (endtv.tv_sec - starttv.tv_sec) * 1000000 + endtv.tv_usec - starttv.tv_usec);
  /* munmap() length of MAP_HUGETLB memory must be hugepage aligned */
  if (munmap(addr, LENGTH)) {
    printf("Error at munmap\n");
    exit(1);
  }
  printf("\n");
}

#define _test_address(addr) { \
  printf("testing %s \n", __PRETTY_FUNCTION__); \
  if (addr == MAP_FAILED) { \
    printf("Error at mmap %s\n\n", __PRETTY_FUNCTION__); \
    return; \
  }         \
  __test_address(addr); \
 }

void test_normal() {
  bool is_large;
  char *addr = (char*) xy_mmap(NULL, LENGTH, false, &is_large);
  _test_address(addr);
}

void test_mavise() {
  bool is_large;
  char *addr = (char*) xy_mmap(NULL, LENGTH, true, &is_large);

  if (is_large)
    printf("large page is used\n");
  else
    printf("large page is not used\n");

  _test_address(addr);
}

void test_mmap_huge() {
  char *addr = (char*) xy_mmap_huge(NULL, LENGTH);
  _test_address(addr);
}

void test_mmap_at_numa() {
  char *addr = (char*) xy_mmap_numa(NULL, LENGTH, 0, false);
  _test_address(addr);
}

void test_mmap_large_at_numa() {
  char *addr = (char*) xy_mmap_numa(NULL, LENGTH, 0, true);
  _test_address(addr);
}


int main() {
  test_normal();
  test_mavise();
  test_mmap_huge();
  test_mmap_at_numa();
  test_mmap_large_at_numa();
}
