//
// Created by hectorhw on 09.03.2021.
//
#include <stdio.h>
#include "debug.h"
#include "value.h"

void disassembleChunk(Chunk* chunk, const char* name){
    printf("== %s ==\n", name);
    for(int offset = 0; offset<chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

static int simpleInstruction(const char* name, int offset){
    printf("%s\n", name);
    return offset+1;
}

static int constantInstruction(const char* name, Chunk* chunk, int offset){
    uint8_t constant = chunk->code[offset+1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset+2;
}

static int constantLongInstruction(const char* name, Chunk* chunk, int offset){
    // chunk holds 1 byte of opcode + 3 bytes of address;

    int constant_idx;

    constant_idx = chunk->code[offset+1] + (chunk->code[offset+2]<<8) + (chunk->code[offset+3]<<16);

    printf("%-16s %4d '", name, constant_idx);
    printValue(chunk->constants.values[constant_idx]);
    printf("'\n");
    return offset+4;
}


int disassembleInstruction(Chunk* chunk, int offset){
    printf("%04d ", offset);

    int currentLine = getLine(&chunk->lines, offset);


    if (offset>0 && currentLine == getLine(&chunk->lines, offset-1)){
        printf("   | ");
    }else{
        printf("%4d ", currentLine);
    }

    uint8_t instruction = chunk->code[offset];

    switch(instruction){

        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_CONSTANT_LONG:
            return constantLongInstruction("OP_CONSTANT_LONG", chunk, offset);

        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset+1;
    }
}

