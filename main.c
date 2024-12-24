//
// Created by Максим Алексеев on 24.12.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include "execution_context.h"

void nested(coroutine ctx) {
    printf("nested: %d\n", 1);
    yield(ctx);
    printf("nested: %d\n", 2);
}

void first(coroutine ctx) {
    printf("first: %d\n", 1);
    coroutine nested_coro = new_coroutine(nested);
    switch_to(nested_coro);
    printf("first: %d\n", 2);
    yield(ctx);
    printf("first: %d\n", 3);
    switch_to(nested_coro);
    printf("first: %d\n", 4);
}

int main(void) {
    coroutine first_coro = new_coroutine(first);
    switch_to(first_coro);
    printf("first ok: %d\n", 1);
    switch_to(first_coro);
    printf("first ok: %d\n", 2);
    return 0;
}