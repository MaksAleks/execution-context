# Execution Context

This is a demo app, showing a mechanism for cooperative execution context switch for learning purposes.

In [execution](./execution) folder there's an [execution.S](./execution/execution.S) asm procedure called `jump`, showing a possibility to jump to another
function, without returning to previous, i.e.:

```c
extern void jump(void* addr);

// this function never returns
int run(int (*routine)(void*, void*)) { 
    void* addr = routine;
    printf("Going to invoke a routine by address %p\n", addr);
    // we jump to `routine` function by its address, switching execution flow.
    // we don't return from jump function in a normal sense.
    // `jump` switches to `routine`, and when the `routine` returns, this `jump` makes you return from `run`.
    jump(routine);
    // never returns - meaning, we never got here
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
    // we call `run` function, passing a pointer to another function `routine`.
    int res = run(routine); 
    printf("routine was invoked\n");
    printf("res = %d\n", res);
    return 0;
}
```

This example is a usual example of a non-local exit, intended to show the possibilities of a non-local control transfer.

## Stackfull coroutine
Using such an approach, we can build what is called `stackfull coroutines`, being able to cooperatively suspend and resume its execution.

A stackfull coroutine is represented by its stack, which is [stack.h](./stack.h) and its execution - [execution_context.h](./execution_context.h).

## Execution context

Execution is represented by execution context:
- stack and CPU registers
- current stack pointer
- previous stack pointer (a point, from which the current execution were called)

The context is represented by following struct:
```c
struct execution_context_struct {
    void* cur_rsp;
    void* prev_rsp;
    stack_view stack;
};
```

Three main methods represents an API of stackfull coroutines:
- `coroutine new_coroutine(execution_routine routine)`; - created new coroutine object, that is going to execute a routine.
  For simplicity of a demo I constrain its type to a `execution_routine`.
- `void switch_to(execution_context ctx)`; - switches current execution context to another one.
- `void yield(execution_context ctx)`; - suspends specified execution ctx, and resumes the context suspended earlier.