//
// Created by Максим Алексеев on 24.12.2024.
//

#ifndef ASM_PROCEDURES_EXECUTION_CONTEXT_H
#define ASM_PROCEDURES_EXECUTION_CONTEXT_H
#include "stack.h"
#endif //ASM_PROCEDURES_EXECUTION_CONTEXT_H

typedef struct stack* stack_view;

struct execution_context_struct {
    void* cur_rsp;
    void* prev_rsp;
    stack_view stack;
};

typedef struct execution_context_struct* execution_context;
typedef struct execution_context_struct* coroutine;
typedef void (*execution_routine)(execution_context);

extern void switch_to(execution_context ctx);
extern void yield(execution_context ctx);
coroutine new_coroutine(execution_routine routine);