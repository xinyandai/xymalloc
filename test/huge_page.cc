#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "os.h"

#define LENGTH (256UL*1024*1024)



static void check_bytes(char *addr) {
  printf("First hex is %x\n", *((unsigned int *)addr));
}

static void write_bytes(char *addr) {
  unsigned long i;

  for (i = 0; i < LENGTH; i++)
    *(addr + i) = (char)i;
}

static int read_bytes(char *addr) {
  unsigned long i;

  check_bytes(addr);
  for (i = 0; i < LENGTH; i++)
    if (*(addr + i) != (char)i) {
      printf("Mismatch at %lu\n", i);
      return 1;
    }
  return 0;
}

void __test_address(char* addr) {
  printf("Returned address is %p\n", addr);
  check_bytes(addr);
  write_bytes(addr);
  read_bytes(addr);
  sleep(10);
  /* munmap() length of MAP_HUGETLB memory must be hugepage aligned */
  if (munmap(addr, LENGTH)) {
    printf("Error at munmap\n");
    exit(1);
  }
  printf("\n");
}

#define _test_address(addr) { \
  if (addr == MAP_FAILED) { \
    printf("Error at mmap %s\n\n", __PRETTY_FUNCTION__); \
    return; \
  }         \
  __test_address(addr); \
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
  test_mavise();
  test_mmap_huge();
  test_mmap_at_numa();
  test_mmap_large_at_numa();
}
