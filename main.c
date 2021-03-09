#include <stdio.h>
#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]) {

    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    writeConstant(&chunk, 1.5, 124);
    writeConstant(&chunk, 1.6, 124);
    writeConstant(&chunk, 1.7, 124);

    writeChunk(&chunk, OP_RETURN, 126);
    writeChunk(&chunk, OP_RETURN, 127);

    disassembleChunk(&chunk, "test chunk");

    freeChunk(&chunk);

    return 0;
}
