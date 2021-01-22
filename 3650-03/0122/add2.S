    .global main
    .text
    // long add2(long xx):
    //    xx += 2
    //    return xx
add2:
    enter $0, $0

    // Arguments come in in the following registers:
    //  %rdi, %rsi, %rdx, %rcx, %r8, %r9
    add $2, %rdi

    // Values are returned from functions in:
    //  %rax
    mov %rdi, %rax

    leave
    ret

main:
    enter $0, $0

    mov $5, %rdi // first argument
    call add2
    // long y = add1(5)

    //printf("%ld\n", y);
    mov $longfmt, %rdi
    mov %rax, %rsi
    mov $0, %al
    call printf

    leave
    ret

    .data
longfmt: .string "%ld\n"
