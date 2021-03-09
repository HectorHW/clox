cmake_minimum_required(VERSION 3.17)
project(clox C)

set(CMAKE_C_STANDARD 99)

add_executable(clox memory.c value.c chunk.c debug.c main.c)