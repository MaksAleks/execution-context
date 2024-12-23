//
// Created by ALEKSEEV Maksim on 22.12.2024.
//

#ifndef STACK_H
#define STACK_H
#include <stddef.h>
#define PAGE_SIZE 4096
#endif //STACK_H

struct stack
{
    void* start;
    size_t pages;
    size_t size;
};
struct stack * allocate_stack(size_t at_least);

int free_stack(struct stack * stack);