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
