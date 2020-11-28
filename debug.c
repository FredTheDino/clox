#include "debug.h"

static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int constantInstruction(const char* name, Chunk *chunk, int offset) {
    bool is_long = chunk->code[offset] == OP_CONSTANT_LONG;
    int addr;
    if (is_long) {
        addr = chunk->code[offset+1];
        addr |= chunk->code[offset+2] << 0x08;
        addr |= chunk->code[offset+3] << 0x10;
    } else {
        addr = chunk->code[offset+1];
    }
    printf("%-16s %4d -> ", name, addr);
    Value value =  chunk->constants.values[addr];
    printValue(value);
    printf("\n");
    if (is_long) {
        return offset + 4;
    } else {
        return offset + 2;
    }
}

int disassembleInstruction(Chunk* chunk, int offset) {
    printf(BLUE);
    printf("%04d ", offset);
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf("   | ");
    } else {
        printf("%4d ", chunk->lines[offset]);
    }
    printf(RESET);

#define SIMPLE(name) \
    case name: return simpleInstruction(STR(name), offset)

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        SIMPLE(OP_NIL);
        SIMPLE(OP_TRUE);
        SIMPLE(OP_FALSE);
        SIMPLE(OP_RETURN);
        SIMPLE(OP_ADD);
        SIMPLE(OP_SUBTRACT);
        SIMPLE(OP_MULTIPLY);
        SIMPLE(OP_DIVIDE);
        SIMPLE(OP_NEGATE);
        SIMPLE(OP_NOT);
        SIMPLE(OP_LESS);
        SIMPLE(OP_GREATER);
        SIMPLE(OP_EQUAL);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_CONSTANT_LONG:
            return constantInstruction("OP_CONSTANT_LONG", chunk, offset);
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
