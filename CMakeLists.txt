cmake_minimum_required(VERSION 3.13)
project(clox C)

set(CMAKE_C_STANDARD 99)

add_executable(clox memory.c value.c chunk.c vm.c debug.c main.c compiler.h scanner.h compiler.c scanner.c object.h object.c)