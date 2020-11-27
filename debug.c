#include "debug.h"

static int simpleInstruction(const char*name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

int disassembleInstruction(Chunk* chunk, int offset) {
    printf(BLUE "%04d " RESET, offset);

#define SIMPLE(name) \
    case name: return simpleInstruction(STR(name), offset)

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        SIMPLE(OP_RETURN);
        default:
            printf("%d - Unknown opcode\n", instruction);
            return offset + 1;
    }

#undef SIMPLE
}

void disassembleChunk(Chunk* chunk, const char* name) {
    printf(BLUE "==" RESET " %s " BLUE "==" RESET "\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}
