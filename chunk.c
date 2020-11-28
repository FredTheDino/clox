#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = 0;
    chunk->lines = 0;
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    if (chunk->constants.capacity) {
        freeValueArray(&chunk->constants);
    }
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity <= chunk->count) {
        uint32_t newCapacity = GROW_CAPACITY(chunk->capacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, chunk->capacity, newCapacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, chunk->capacity, newCapacity);
        chunk->capacity = newCapacity;
    }

    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

void writeConstant(Chunk* chunk, Value value, int line) {
    int addr = addConstant(chunk, value);
    if (addr > 0xFF) {
        writeChunk(chunk, OP_CONSTANT_LONG, line);
        writeChunk(chunk, 0xFF & (addr >> 0x00), line);
        writeChunk(chunk, 0xFF & (addr >> 0x08), line);
        writeChunk(chunk, 0xFF & (addr >> 0x10), line);
    } else {
        writeChunk(chunk, OP_CONSTANT, line);
        writeChunk(chunk, addr, line);
    }
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}
