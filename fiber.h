
//
// Created by ALEKSEEV Maksim on 22.12.2024.
//

#ifndef FIBER_H
#define FIBER_H
#include "stack.h"
#endif //FIBER_H

enum fiber_state
{
    NEW = 0,
    STARTED,
    FINISHED
};

struct fiber
{
    void* sp; // fiber stack pointer
    void* ret; // fiber ret address
    int (*routine)(struct fiber*); // fiber routine
    struct stack* stack; // fiber's stack
    enum fiber_state state; // fiber's state
    void* finish_handler;
};

void clean_up_fiber(struct fiber*);
struct fiber* init_fiber(int (*routine)(struct fiber* fiber));
extern void yield(struct fiber*);
extern int run_fiber(struct fiber* fiber);
