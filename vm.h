//
// Created by hectorhw on 10.03.2021.
//

#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "chunk.h"
#include "value.h"


typedef struct {
    Chunk* chunk;
    uint8_t* ip; //instruction pointer, points to instruction that will be executed next
    Value* stack;
    Value* stackTop;
    int stack_size;
} VM;

typedef enum {
    INTERPRET_OK=0,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(const char* source);

void push(Value value);
void uncheckedPush(Value value);
Value pop();

#endif //CLOX_VM_H
