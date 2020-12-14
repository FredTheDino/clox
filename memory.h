#pragma once

#include "common.h"
#include "value.h"

#define ALLOCATE(type, count) \
    (type*)reallocate(NULL, 0, sizeof(type) * (count))

#define FREE(type, pointer) \
    reallocate(pointer, sizeof(type), 0)

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define FREE_ARRAY(type, ptr, cap) \
    reallocate(ptr, sizeof(type) * (cap), 0)

#define GROW_ARRAY(type, ptr, oldCap, newCap) \
    (type*)reallocate(ptr, sizeof(type) * (oldCap), sizeof(type) * (newCap))

void* reallocate(void* ptr, size_t olds, size_t news);
void collectGarbage();
void markObject(Obj* object);
void markValue(Value value);
void freeObjects();
