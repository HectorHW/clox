//
// Created by hectorhw on 10.03.2021.
//

#include <stdio.h>
#include "common.h"
#include "vm.h"
#include "compiler.h"
#include "debug.h"
#include "memory.h"
#include "stdlib.h"

VM vm;

static void initStack(){
    vm.stack = GROW_ARRAY(Value, vm.stack, 0, 8);
    vm.stack_size = 8;
    vm.stackTop = vm.stack;
}

static void freeStack(){
    free(vm.stack);
}


void initVM(){
initStack();
}

void freeVM(){
    freeStack();
}

static InterpretResult run(){
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_CONSTANT_LONG(constant_variable) \
(constant_variable = vm.chunk->constants.values[*vm.ip + (*(vm.ip+1)<<8) + (*(vm.ip+2)<<16)], vm.ip+=3)

#define BINARY_OP(op) \
    do {\
        *(vm.stackTop-2) = *(vm.stackTop-2) op *(vm.stackTop-1); \
        vm.stackTop--;\
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
                *(vm.stackTop-1) *= -1;  //inplace
                break;
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

InterpretResult interpret(const char* source) {
    Chunk chunk;
    initChunk(&chunk);
    if(compile(source, &chunk)!=0){
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    InterpretResult result = run();

    freeChunk(&chunk);

    return result;
}

void push(Value value) {

    if(vm.stackTop==vm.stack_size+vm.stack){
        //grow stack, repoint
        vm.stack = GROW_ARRAY(Value, vm.stack, vm.stack_size, GROW_CAPACITY(vm.stack_size));
        vm.stackTop = vm.stack_size + vm.stack;
        vm.stack_size = GROW_CAPACITY(vm.stack_size);
    }

    *vm.stackTop = value;
    vm.stackTop++;
}

void uncheckedPush(Value value){
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}


