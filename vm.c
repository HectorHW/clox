//
// Created by hectorhw on 10.03.2021.
//

#include <stdio.h>
#include "common.h"
#include "vm.h"
#include "debug.h"

VM vm;

static void resetStack(){
    vm.stackTop = vm.stack;
}

void initVM(){
resetStack();
}

void freeVM(){

}

static InterpretResult run(){
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_CONSTANT_LONG(constant_variable) \
(constant_variable = vm.chunk->constants.values[*vm.ip + (*(vm.ip+1)<<8) + (*(vm.ip+2)<<16)], vm.ip+=3)

#define BINARY_OP(op) \
    do {\
        double b = pop(); \
        double a = pop(); \
        push(a op b); \
    }while(false) \


    for(;;){

#ifdef DEBUG_TRACE_EXECUTION
        printf("        ");
        for(Value* slot = vm.stack; slot<vm.stackTop;slot++){
            printf("[ ");
            printValue(*slot);
            printf("]");
        }
        printf("\n");


        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif

        uint8_t instruction;
        switch (instruction=READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                //printValue(constant);
                //printf("\n");
                break;
            }
            case OP_CONSTANT_LONG: {
                Value constant;
                READ_CONSTANT_LONG(constant);
                push(constant);
                //printValue(constant);
                //printf("\n");
                break;
            }

            case OP_NEGATE: {
                push(-pop()); break;
            }

            case OP_ADD: BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE: BINARY_OP(/); break;

            case OP_RETURN: {
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }
#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_CONSTANT_LONG
#undef BINARY_OP
}

InterpretResult interpret(Chunk *chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}


