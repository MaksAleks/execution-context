.global prepare_context
.global switch_to
.global yield
.global resume

# 1) %rdi - execution_context (and 7-th arg for trampoline)
# 2) %rsi - trampoline
# 3) %rdx - 8-th arg for trampoline
prepare_context:
    # Switch stack
    movq %rsp, %r11 # tmp := rsp
    movq (%rdi), %rsp # rsp := stack

    # Just to be safe
    subq $64, %rsp

    # Stack must be 16-byte aligned
    andq $-16, %rsp
    addq $8, %rsp

    # Prepare for context switch

    # 7-th argument for trampoline
    movq %rdi, 8(%rsp)

    # 8-th argument for trampoline
    movq %rdx, 16(%rsp)

    # 1) set new execution's stack return address to trampoline
    pushq %rsi

    # 2) Callee-saved registers
    pushq $0 # r15
    pushq $0 # r14
    pushq $0 # r13
    pushq $0 # r12
    pushq $0 # rbx
    pushq $0 # rbp

    # 3) Return value for setup_coro_ctx
    movq %rsp, %rax

    # 4) Restore callstack
    movq %r11, %rsp # rsp := tmp
    ret

# stack pointer in %rdi
resume:
    # Move to target stack
    movq %rdi, %rsp

    # Restore and pop registers saved on target stack
    popq %rbp
    popq %rbx

    popq %r12
    popq %r13
    popq %r14
    popq %r15

    ret;

# ctx in %rdi
switch_to:
    # 1. Save current execution state

    # 1.1 Save callee-saved registers on top of the current stack
    pushq %r15
    pushq %r14
    pushq %r13
    pushq %r12

    pushq %rbx
    pushq %rbp

    # Switch stacks

    # 1.2 Save current stack to ctx.prev_stack
    movq %rsp, 8(%rdi) # prev_rsp := rsp

    # 2. Resume provided context
    movq (%rdi), %rdi
    jmp resume

# ctx in %rdi
yield:
    # 1.1 Save callee-saved registers on top of the current stack
    pushq %r15
    pushq %r14
    pushq %r13
    pushq %r12

    pushq %rbx
    pushq %rbp

    # 2. Save current context stack
    mov %rsp, (%rdi) # ctx->cur_rsp := rsp

    # 3. Resume previous execution
    mov 8(%rdi), %rdi # rdi := ctx->prev_rsp
    jmp resume

