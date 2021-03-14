//
// Created by hectorhw on 09.03.2021.
//

#include <stdlib.h>
#include "chunk.h"
#include "memory.h"

void initLineArray(LineArray *lineArray) {
    lineArray->capacity = 0;
    lineArray->count = 0;
    lineArray->structArray = NULL;
}

void freeLineArray(LineArray *lineArray) {
    FREE_ARRAY(LineEncoding, lineArray->structArray, lineArray->capacity);
    initLineArray(lineArray);
}

void writeLineArray(LineArray *lineArray, int line) {

    if(lineArray->capacity < lineArray->count + 1) {
        int oldCapacity = lineArray->capacity;
        lineArray->capacity = GROW_CAPACITY(oldCapacity);
        lineArray->structArray = GROW_ARRAY(LineEncoding, lineArray->structArray,
                                            oldCapacity, lineArray->capacity);
    }

    if(lineArray->count==0){
        lineArray->structArray[0].line = line;
        lineArray->structArray[0].span = 1;
        lineArray->count++;
        return;
    }

    if (lineArray->structArray[lineArray->count-1].line==line){
        lineArray->structArray[lineArray->count-1].span++;
    }else{
        lineArray->structArray[lineArray->count].line = line;
        lineArray->structArray[lineArray->count].span = 1;
        lineArray->count++;
    }
}

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;

    chunk->code = NULL;

    initLineArray(&chunk->lines);
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    freeLineArray(&chunk->lines);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line){
    if(chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
        //chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
    }
    chunk->code[chunk->count] = byte;
    //chunk->lines[chunk->count] = line;
    writeLineArray(&chunk->lines, line);
    chunk->count++;

}

void writeConstant(Chunk *chunk, Value value, int line) {

    int constant_index = addConstant(chunk, value);

    if(constant_index<=UINT8_MAX){
        writeChunk(chunk, OP_CONSTANT, line);
        writeChunk(chunk, (uint8_t)constant_index, line);
    }else{

        writeChunk(chunk, OP_CONSTANT_LONG, line);

        //write address in next 3 bytes
        writeChunk(chunk, constant_index&     0xff,line);
        writeChunk(chunk, (constant_index&  0xff00)>>8,line);
        writeChunk(chunk, (constant_index&0xff0000)>>16,line);

    }
}

int addConstant(Chunk *chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

int getLine(LineArray *lineArray, int opcode_index) {
    int acc = 0;
    for (int i=0;i<lineArray->count;i++){
        if(acc<=opcode_index && lineArray->structArray[i].span + acc >opcode_index){
            return lineArray->structArray[i].line;
        }
        acc += lineArray->structArray[i].span;
    }
    return 0;
}




