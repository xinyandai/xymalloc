#include "xy_malloc.h"
#include <memory>

extern inline void *xy_malloc(size_t size) xy_attr_noexcept {
  return malloc(size);
}

extern inline void *xy_calloc(size_t count, size_t size) xy_attr_noexcept {
  return calloc(count, size);
}

extern inline void *xy_realloc(void *p, size_t new_size) xy_attr_noexcept {
  return realloc(p, new_size);
}

extern inline void xy_free(void *p) xy_attr_noexcept { free(p); }