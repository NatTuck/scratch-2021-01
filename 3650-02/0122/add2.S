    .global main

    .text

    // long add2(long x): 
    //     x += 2
    //     return x
    //
    //     y = x
    //     y += 2
    //     return y
add2:
    enter $0, $0

    // Arguments to a function come in in:
    //  %rdi, %rsi, %rdx, %rcx, %r8, %r9
    add $2, %rdi

    // The return value for a function goes in:
    //  %rax
    mov %rdi, %rax

    leave
    ret

main:
    enter $0, $0

    // long y = add2(5);
    mov $5, %rdi 
    call add2
    // by convention:
    //  - result in %rax
    //  - value in %rdi is "gone"
   
    // printf("%ld\n", y);
    mov $long_fmt, %rdi
    mov %rax, %rsi
    mov $0, %al
    call printf

    leave
    ret

    .data
long_fmt: .string "%ld\n"
