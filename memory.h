#pragma once

#include "common.h"

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define FREE_ARRAY(type, ptr, cap) \
    reallocate(ptr, sizeof(type) * (cap), 0)

#define GROW_ARRAY(type, ptr, oldCap, newCap) \
    (type*)reallocate(ptr, sizeof(type) * (oldCap), sizeof(type) * (newCap))

void* reallocate(void* ptr, size_t olds, size_t news);
