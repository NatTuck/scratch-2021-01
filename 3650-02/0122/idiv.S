    .global main

    .text
main:
    enter $0, $0

    mov $40, %rax
    mov $30, %rbx
    //mov $20, %rcx
    mov $10, %rcx

    // "double quadword / quadword"
    // "word" means 16 bits
    // 128 bits / 64 bits

    // (%rdx*2^64 + %rax) / %rdx

    // problem: overflow
    // result doesn't fit in 64-bit register

    cqo // copy sign bit over %rdx
    idiv %rcx
    // actual op: [%rdx:%rax] / %rdx

    // option 1: %rax / %rdx = 4
    // option 2: small # / big # = 0

    mov $longfmt, %rdi
    mov %rax, %rsi
    mov $0, %al
    call printf

    leave
    ret

    .data
longfmt: .string "%ld\n"
