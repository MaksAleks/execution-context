//
// Created by Максим Алексеев on 24.12.2024.
//
#include <stdlib.h>
#include <stdio.h>
#include "execution_context.h"

extern void* prepare_context(execution_context ctx, void* trampoline, void* arg);
extern void resume(void* rsp);

static execution_context new_context() {
    execution_context ctx = malloc(sizeof(struct execution_context_struct));
    return ctx;
}

static void finish_execution(execution_context ctx) {
    printf("finish execution: %p\n", ctx->cur_rsp);
    void* prev_rsp = ctx->prev_rsp;
    free_stack(ctx->stack);
    free(ctx);
    printf("resume execution: %p\n", prev_rsp);
    resume(prev_rsp);
}

static void trampoline(execution_context ctx, execution_routine routine) {
    routine(ctx);
    finish_execution(ctx);
}

static void context_trampoline(void* a, void* b, void* c, void* d, void* e, void* f, execution_context ctx, execution_routine routine) {
    trampoline(ctx, routine);
}

execution_context new_coroutine(execution_routine routine) {
    stack_view stack = allocate_stack(PAGE_SIZE * 2);
    execution_context ctx = new_context();
    ctx->stack = stack;
    ctx->cur_rsp = stack->start;
    void *rsp = prepare_context(ctx, context_trampoline, routine);
    ctx->cur_rsp = rsp;
    return ctx;
}