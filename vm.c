#include "vm.h"
#include "debug.h"
#include "compiler.h"

VM vm;

static void resetStack() {
    vm.stackTop = vm.stack;
}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
    ASSERT(vm.stackTop - vm.stack < STACK_MAX, "Stack overflow!");
}

Value pop() {
    ASSERT(vm.stack != vm.stackTop, "No value on stack!");
    vm.stackTop--;
    return *vm.stackTop;
}

void initVM() {
    resetStack();
}

void freeVM() {
}

static InterpretResult run() {
    Value *consts = vm.chunk->constants.values;
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (consts[READ_BYTE()])
#define READ_CONSTANT_LONG() (consts[READ_BYTE() \
                              | READ_BYTE() << 0x08 \
                              | READ_BYTE() << 0x10 \
                              ])
#define BINARY_OP(oper) push(pop() oper pop())
    for (;;) {
        uint8_t instruction;
#ifdef DEBUG_TRACE_EXECUTION
        printf("         ");
        for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
        switch (instruction = READ_BYTE()) {
            case OP_ADD: BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE: BINARY_OP(/); break;
            case OP_NEGATE: push(-pop()); break;
            case OP_CONSTANT_LONG: {
                Value constant = READ_CONSTANT_LONG();
                push(constant);
                break;
            }
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_RETURN: {
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }
#undef READ_BYTE
#undef READ_CONSTANT
}

InterpretResult interpret(const char* source) {
    compile(source);
    return INTERPRET_OK;
}
