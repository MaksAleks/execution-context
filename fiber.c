//
// Created by ALEKSEEV Maksim on 22.12.2024.
//
#include "fiber.h"
#include "stdio.h"
#include <stdlib.h>

extern void invoke(void* addr);
extern void* setup_fiber_ctx(struct fiber*);
extern long test2(long a);
extern long test();

void clean_up_fiber(struct fiber* f)
{
    free_stack(f->stack);
    free(f);
}

void stop_fiber()
{
    printf("finishing fiber\n");
    // f->state = FINISHED;
    // invoke(f->ret);
}

struct fiber* init_fiber(int(* routine)(struct fiber*))
{
    struct stack *stack = allocate_stack(PAGE_SIZE);
    struct fiber* f = malloc(sizeof(struct fiber));
    f->state = NEW;
    f->routine = routine;
    f->stack = stack;
    printf("fiber stack size: %ld\n", f->stack->size);
    printf("Setup fiber's context\n");
    void* sp = setup_fiber_ctx(f);
    printf("Fiber's context set up\n");
    f->sp = sp;
    f->finish_handler = stop_fiber;
    return f;
}
