.global setup_fiber_ctx
.global run_fiber
.global yield
.global test
.global test2
.global finish_fiber
.global invoke

# invoke a function by address
# function address in %rdi
invoke:
    jmp %rdi

# fiber on the stack
finish_fiber:
    pushq %rbp
    movq %rsp, %rbp
    movq 16(%rbp), %rcx # rcx := fiber
    movq $3, 32(%rcx) # fiber.state = FINISHED [3]
    jmp 8(%rcx)

test2:
    pushq %rbp
    movq %rsp, %rbp
    movq 16(%rbp), %rax
    addq $2, %rax
    popq %rbp
    ret

test:
    mov $40, %rcx
    pushq %rcx
    call test2
    popq %rcx
    ret

# struct fiber {
#   void* sp; // fiber stack pointer -> 8(%rdi)
#   void* ret; // fiber ret address -> 16(%rdi)
#   int (*routine)(void); // fiber routine -> 36(%rdi)
#   struct stack* stack; // fiber's stack
# }
#
# struct stack {
#     void* start;
#     size_t pages;
#     size_t size;
# };
#
# setup_fiber_ctx(struct fiber*)
# fiber in %rdi
setup_fiber_ctx:
    # fiber's state
    movq 8(%rdi), %r13 # r13 = fiber.ret
    movq 16(%rdi), %r14 # r14 = fiber.routine
    movq 24(%rdi), %r15 # r15 = fiber.stack

    # Switch to stack
    movq %rsp, %r11 # tmp := rsp
    movq (%r15), %rsp # rsp := fiber->stack->start

    # Just to be safe
    subq $64, %rsp

    # stack must be 16-byte aligned
    andq $-16, %rsp

    # Prepare for context switch

    # 0) Fiber's finish routine
    pushq 40(%rdi)

    # 1) Return address for fiber's routine
    pushq %r14

    # 2) Fiber to pass to fiber's routine
    pushq %rdi

    # 3) Callee-saved registers
    pushq $0 # r15
    pushq $0 # r14
    pushq $0 # r13
    pushq $0 # r12
    pushq $0 # rbx
    pushq $0 # rbp

    # Return value for setup_fiber_ctx
    movq %rsp, %rax

    # Restore callstack
    movq %r11, %rsp # rsp := tmp

    ret

# struct fiber {
#   void* sp; // fiber stack pointer -> 8(%rdi)
#   void* ret; // fiber ret address -> 16(%rdi)
#   int (*routine)(void); // fiber routine -> 36(%rdi)
#   struct stack* stack; // fiber's stack
# }

# run_fiber(struct fiber*)
run_fiber:
    #0. Change fiber's state
    movq $2, 32(%rdi)

    # 1. Save callee-saved registers on top of the current stack
    pushq %r15
    pushq %r14
    pushq %r13
    pushq %r12

    pushq %rbx
    pushq %rbp

    # 2. switch to fiber's stack
    mov %rsp, 8(%rdi) # fiber->ret := rsp
    mov (%rdi), %rsp # rsp := fiber->sp

    # 3. Restore and pop registers saved on target stack
    popq %rbp
    popq %rbx

    popq %r12
    popq %r13
    popq %r14
    popq %r15

    # 4. in setup_fiber_ctx we pushed a fiber's address onto the stack to pass to fiber's routine.
    # A fiber's routine gets only one argument, that is passed via %rdi.
    # So we restoring the fiber's address from its stack and pass to %rdi
    popq %rdi

    # 5. in setup_fiber_ctx we installed return address to fiber->routine.
    # so after returning, we'll jump to fiber's routine
    ret

# yield a fiber
# fiber in %rdi
yield:
    pushq %rdi

    # 1. Save callee-saved registers on top of the current stack
    pushq %r15
    pushq %r14
    pushq %r13
    pushq %r12

    pushq %rbx
    pushq %rbp

    # 2. switch to caller's stack
    mov %rsp, (%rdi) # fiber->sp := rsp
    mov 8(%rdi), %rsp # rsp := fiber->ret

    # 3. Restore and pop registers saved on target stack
    popq %rbp
    popq %rbx

    popq %r12
    popq %r13
    popq %r14
    popq %r15

    ret