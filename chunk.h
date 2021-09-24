//
// Created by hectorhw on 09.03.2021.
//

#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_CONSTANT_LONG,

    OP_NIL,
    OP_TRUE,
    OP_FALSE,

    OP_NOT,

    OP_EQUAL, OP_GREATER, OP_LESS, // != ~~~ not (==); may change

    OP_NEGATE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,

    OP_PRINT,
    OP_JUMP_IF_FALSE,
    OP_JUMP,
    OP_LOOP,
    OP_POP,
    OP_DEFINE_GLOBAL,
    OP_GET_GLOBAL,
    OP_SET_GLOBAL,
    OP_GET_LOCAL,
    OP_SET_LOCAL,
    OP_GET_UPVALUE,
    OP_SET_UPVALUE,
    OP_CLOSE_UPVALUE,
    OP_GET_PROPERTY,
    OP_SET_PROPERTY,

    OP_CLOSURE,
    OP_RETURN,
    OP_CALL,
    OP_CLASS,
    OP_METHOD,
    OP_INVOKE,
    OP_INHERIT,
    OP_GET_SUPER,
    OP_SUPER_INVOKE
} OpCode;

typedef struct {
    int line;
    int span;
} LineEncoding;

typedef struct{
    int count;
    int capacity;
    LineEncoding* structArray;
} LineArray;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;

    LineArray lines;

    ValueArray constants;
} Chunk;

void initLineArray(LineArray* lineArray);
void freeLineArray(LineArray* lineArray);
void writeLineArray(LineArray* lineArray, int line);
int getLine(LineArray* lineArray, int opcode_index);


void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
void writeConstant(Chunk* chunk, Value value, int line);

int addConstant(Chunk* chunk, Value value);

#endif //CLOX_CHUNK_H
