#include <stdlib.h>

#include "value.h"
#include "memory.h"

void initValueArray(ValueArray* arr) {
    arr->count = 0;
    arr->capacity = 0;
    arr->values = 0;
}

void freeValueArray(ValueArray* arr) {
    FREE_ARRAY(uint8_t, arr->values, arr->capacity);
    initValueArray(arr);
}

void writeValueArray(ValueArray* arr, Value value) {
    if (arr->capacity <= arr->count) {
        uint32_t newCapacity = GROW_CAPACITY(arr->capacity);
        arr->values = GROW_ARRAY(Value, arr->values, arr->capacity, newCapacity);
        arr->capacity = newCapacity;
    }

    arr->values[arr->count++] = value;
}

bool valuesEqual(Value a, Value b) {
    if (a.type != b.type) return false;

    switch (a.type) {
        case VAL_BOOL:    return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NIL:     return true;
        case VAL_NUMBER:  return AS_NUMBER(a) == AS_NUMBER(b);
        default:
            ASSERT(false, "UNREACHABLE");
            return false;
    }
}

void printValue(Value value) {
    if (IS_NUMBER(value)) {
        printf("%g", AS_NUMBER(value));
    } else if (IS_BOOL(value)) {
        if (AS_BOOL(value)) {
            printf("true");
        } else {
            printf("false");
        }
    } else if (IS_NIL(value)) {
        printf("nil");
    }
}
