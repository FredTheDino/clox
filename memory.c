#include "memory.h"

#include "stdlib.h"
#include "assert.h"

void* reallocate(void* ptr, size_t olds, size_t news) {
    if (news == 0) {
        ASSERT(ptr, "Trying to free NULL!");
        free(ptr);
        return NULL;
    }
    void* result = realloc(ptr, news);
    ASSERT(result, "Invalid memory allocation!");
    return result;
}

