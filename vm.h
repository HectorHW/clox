//
// Created by hectorhw on 10.03.2021.
//

#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "chunk.h"
#include "table.h"
#include "value.h"
#include "object.h"

#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX*UINT8_COUNT)

typedef struct {
    ObjFunction* function;
    uint8_t* ip; // function stores its own ip
    Value* slots;
} CallFrame;


typedef struct {
    CallFrame frames[FRAMES_MAX];
    int frameCount;

    Value stack[STACK_MAX];
    Value* stackTop;
    Obj* objects; //list of all allocated objects
    int stack_size;

    Table globals;
    Table strings;
} VM;

typedef enum {
    INTERPRET_OK=0,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

extern VM vm;

void initVM();
void freeVM();
InterpretResult interpret(const char* source);

void push(Value value);
void uncheckedPush(Value value);
Value pop();

#endif //CLOX_VM_H
