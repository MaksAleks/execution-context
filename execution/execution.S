.global jump

# address in %rdi
jump:
    # %rbp - base pointer that points to the previous stack frame base (run's function frame in that case)
    # so, (%rbp + 8) = return address of previous call.
    movq %rbp, %rsp # skip current stack frame and get to run's frame

    # run function was called in C. by convention, gcc pushes %rbp on the stack at the beginning.
    # So %rsp now points not to return address, but to prev %rbp
    # ans we need to restore it
    popq %rbp

    # now %rsp point's to return run's return address (an instruction in 'main' function, that follows 'run()' call)
    movq %rsp, %rsi

    # by pushin %rdi address onto the stack, that address is now considered as return address
    pushq %rdi
    ret # pop the %rdi return address and jump to it. And previous return address becomes current return address, but for a function in %rdi