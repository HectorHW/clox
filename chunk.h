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
    OP_RETURN,
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
