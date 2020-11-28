#include "memory.h"

#include "stdlib.h"
#include "assert.h"
#include "object.h"
#include "vm.h"

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

void freeObject(Obj* object) {
    switch (object->type) {
        case OBJ_STRING: {
            ObjString* string = (ObjString*)object;
            FREE_ARRAY(char, string->chars, string->length + 1);
            FREE(ObjString, object);
            break;
        }
    }
}

void freeObjects() {
    Obj* object = vm.objects;
    while (object) {
        Obj* next = object->next;
        freeObject(object);
        object = next;
    }
}

