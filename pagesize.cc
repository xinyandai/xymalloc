#include <unistd.h>
#include <stdio.h>

int main(void) {
  printf("%dKB\n", getpagesize() >> 10);
  return 0;
}