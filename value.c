#include <stdlib.h>
#include <string.h>

#include "value.h"
#include "memory.h"
#include "object.h"

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
        case VAL_OBJ: {
            ObjString* aStr = AS_STRING(a);
            ObjString* bStr = AS_STRING(b);
            return aStr->length == bStr->length &&
                    memcmp(aStr->chars, bStr->chars, aStr->length) == 0;
        }
        default:
            ASSERT(false, "UNREACHABLE");
            return false;
    }
}

void printValue(Value value) {

    switch(value.type) {
        case VAL_NIL: printf("nil"); break;
        case VAL_BOOL: printf(AS_BOOL(value) ? "true" : "false"); break;
        case VAL_NUMBER: printf("%g", AS_NUMBER(value)); break;
        case VAL_OBJ: printObject(value); break;
    }
}
