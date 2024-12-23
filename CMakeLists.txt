cmake_minimum_required(VERSION 3.28)
project(asm_procedures C ASM)

set(CMAKE_C_STANDARD 11)

add_executable(asm_procedures
        fiber.S
        stack.c
        stack.h
        fiber.h
        fiber.c
        main.c
)