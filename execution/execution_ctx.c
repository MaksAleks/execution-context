//
// Created by Максим Алексеев on 24.12.2024.
//
#include <stdio.h>
#include <stdint.h>

extern void jump(void* addr);

int run(int (*routine)(void*, void*)) { // never returns
    void* addr = routine;
    printf("Going to invoke a routine by address %p\n", addr);
    jump(routine);
    // never got here
    printf("Never got here");
    return 42;
}

int routine(void* addr, void* prev_rea_addr) {
    printf("Successfully jumped to routine\n");
    printf("In routine by address %p\n", addr);
    printf("Previous ret address %p\n", prev_rea_addr);
    return 42;
}

int main(void) {
    printf("main started:\n");
    int res = run(routine);
    printf("routine was invoked\n");
    printf("res = %d\n", res);
    return 0;
}
