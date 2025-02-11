#include "debug.h"
#include "object.h"

static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int byteInstruction(const char* name, Chunk *chunk, int offset) {
    uint8_t slot = chunk->code[offset + 1];
    printf("%-16s %4d\n", name, slot);
    return offset + 2;
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

static int invokeInstruction(const char* name, Chunk* chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];
    uint8_t argCount = chunk->code[offset + 2];
    printf("%-16s (%d args) %4d '", name, argCount, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 3;
}

static int jumpInstruction(const char* name, int sign, Chunk *chunk, int offset) {
    uint16_t jump = (uint16_t)(chunk->code[offset + 1] << 8);
    jump |= chunk->code[offset + 2];
    printf("%-16s %4d -> %d\n", name, offset, offset + 3 + sign * jump);
    return offset + 3;
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
        SIMPLE(OP_ADD);
        SIMPLE(OP_SUBTRACT);
        SIMPLE(OP_MULTIPLY);
        SIMPLE(OP_DIVIDE);
        SIMPLE(OP_NEGATE);
        SIMPLE(OP_NOT);
        SIMPLE(OP_LESS);
        SIMPLE(OP_GREATER);
        SIMPLE(OP_EQUAL);
        SIMPLE(OP_CLOSE_UPVALUE);
        case OP_LOOP:
            return jumpInstruction("OP_LOOP", -1, chunk, offset);
        case OP_JUMP:
            return jumpInstruction("OP_JUMP", 1, chunk, offset);
        case OP_JUMP_IF_FALSE:
            return jumpInstruction("OP_JUMP_IF_FALSE", 1, chunk, offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_CONSTANT_LONG:
            return constantInstruction("OP_CONSTANT_LONG", chunk, offset);
        case OP_CLASS:
            return constantInstruction("OP_CLASS", chunk, offset);
        case OP_METHOD:
            return constantInstruction("OP_METHOD", chunk, offset);
        case OP_GET_PROPERTY:
            return constantInstruction("OP_GET_PROPERTY", chunk, offset);
        case OP_SET_PROPERTY:
            return constantInstruction("OP_SET_PROPERTY", chunk, offset);
        case OP_CALL:
            return byteInstruction("OP_CALL", chunk, offset);
        case OP_INVOKE:
            return invokeInstruction("OP_INVOKE", chunk, offset);
        case OP_DEFINE_GLOBAL:
            return constantInstruction("OP_DEFINE_GLOBAL", chunk, offset);
        case OP_CLOSURE: {
            offset++;
            uint8_t constant = chunk->code[offset++];
            printf("%-16s %4d ", "OP_CLOSURE", constant);
            printValue(chunk->constants.values[constant]);
            printf("\n");

            ObjFunction* function = AS_FUNCTION(chunk->constants.values[constant]);
            for (int j = 0; j < function->upvalueCount; j++) {
                int isLocal = chunk->code[offset++];
                int index = chunk->code[offset++];
                printf("%04d      |                     %s %d\n",
                       offset - 2, isLocal ? "local" : "upvalue", index);
            }

            return offset;
        }
        case OP_SET_LOCAL:
            return byteInstruction("OP_SET_LOCAL", chunk, offset);
        case OP_GET_LOCAL:
            return byteInstruction("OP_GET_LOCAL", chunk, offset);
        case OP_SET_GLOBAL:
            return constantInstruction("OP_SET_GLOBAL", chunk, offset);
        case OP_GET_GLOBAL:
            return constantInstruction("OP_GET_GLOBAL", chunk, offset);
        case OP_SET_UPVALUE:
            return byteInstruction("OP_SET_UPVALUE", chunk, offset);
        case OP_GET_UPVALUE:
            return byteInstruction("OP_GET_UPVALUE", chunk, offset);
        SIMPLE(OP_PRINT);
        SIMPLE(OP_RETURN);
        SIMPLE(OP_POP);
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
