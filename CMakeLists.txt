cmake_minimum_required(VERSION 3.28)
project(asm_procedures C ASM)

set(CMAKE_C_STANDARD 11)

add_executable(asm_procedures
        stack.c
        stack.h
        execution_context.S
        execution_context.c
        execution_context.h
        main.c
)