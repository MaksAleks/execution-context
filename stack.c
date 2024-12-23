//
// Created by ALEKSEEV Maksim on 22.12.2024.
//
#include "stack.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

int protect_guard_page(const struct stack* stack);

struct stack * allocate_stack(const size_t at_least)
{
    size_t pages = at_least / PAGE_SIZE;
    if (at_least % PAGE_SIZE != 0)
    {
        ++pages;
    }
    size_t size = pages * PAGE_SIZE;
    pages++; // for guard page
    void* start = mmap(/*addr=*/NULL, /*length=*/size + PAGE_SIZE,
                                /*prot=*/PROT_READ | PROT_WRITE,
                                /*flags=*/MAP_PRIVATE | MAP_ANONYMOUS,
                                /*fd=*/-1, /*offset=*/0);
    if (start == MAP_FAILED)
    {
        perror("mmap failed");
        return NULL;
    }
    struct stack* stack = malloc(sizeof(struct stack));

    stack->start = start + PAGE_SIZE;
    stack->pages = pages;
    stack->size = size;
    const int res = protect_guard_page(stack);
    if (res != 0)
    {
        perror("protect_guard_page failed");
        return NULL;
    }
    return stack;
}

int free_stack(struct stack* stack)
{
    if (stack == NULL)
    {
        return 0;
    }
    const int result = munmap(stack->start, stack->size);
    if (result != 0)
    {
        return -1;
    }
    free(stack);
    return 1;
}

size_t pages_to_bytes(size_t count) {
    return count * PAGE_SIZE;
}

int protect_guard_page(const struct stack* stack)
{
    return mprotect(/*addr=*/stack->start,
                   /*len=*/1,
                   /*prot=*/PROT_NONE);
}
