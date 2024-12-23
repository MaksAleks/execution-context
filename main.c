#include <stdio.h>
#include "fiber.h"

#define PAGE_SIZE 4096

int first(struct fiber* fiber);
int second(struct fiber* fiber);

long test();

int main(void)
{
    printf("Start fibers!\n");
    struct fiber* first_fiber = init_fiber(first);
    struct fiber* second_fiber = init_fiber(second);
    run_fiber(first_fiber);
    printf("First fiber exited!\n");
    run_fiber(second_fiber);
    printf("Second fiber exited!\n");
    run_fiber(first_fiber);
    printf("First fiber finished!\n");
    run_fiber(second_fiber);
    printf("Second fiber finished!\n");
}


int first(struct fiber* fiber)
{
    printf("Fiber[1]: started!\n");
    printf("Fiber[1]: stack_size = %ld\n", fiber->stack->size);
    printf("Fiber[1]: state = %d\n", fiber->state);
    yield(fiber);
    printf("Fiber[1]: continued\n");
    printf("Fiber[1]: stack_size = %ld\n", fiber->stack->size);
    printf("Fiber[1]: state = %d\n", fiber->state);
    return 0;
}

int second(struct fiber* fiber)
{
    printf("Fiber[2]: started!\n");
    printf("Fiber[2]: stack_size = %ld\n", fiber->stack->size);
    printf("Fiber[2]: state = %d\n", fiber->state);
    yield(fiber);
    printf("Fiber[2]: continued\n");
    printf("Fiber[2]: stack_size = %ld\n", fiber->stack->size);
    printf("Fiber[2]: state = %d\n", fiber->state);
    return 0;
}
