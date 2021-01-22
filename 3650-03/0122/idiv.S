    .globl main
    .text
main:
    enter $0, $0

    mov $40, %rax
    //mov $30, %rbx
    //mov $20, %rcx
    //mov $10, %rdx
    mov $10, %rcx

    // A "word" is a 16-bit number.
    // A doubleworld is 32-bit
    // A quadword is 64 bit
    // A double quadword is 128 bit

    // We're doing 128b / 64b
    // %rdx is sign-extended from %rax
    // (%rdx*2^64 + %rax) / %rdx


    // a = 0010 = 2
    // Store a in an 8 bit register.
    // ????0010 => 00000010
    // a = 1110 = -2
    // ????1110 => 11111110 = -2

    //cqo is:
    //if %rax is negative, fill %rdx with 1 bits
    //if %rax is non-neg, fill %rdx with 0 bits


    /*
    %rdx = 10
    %rax = 40

    [%rdx:%rax] = 10*2^64 + 40
    %rax = (10*2^64 + 40) / 10
    %rax = 1*2^64 + 4  (that's too big by 5)
     */

    cqo
    idiv %rcx
    // Dividing by zero? yes
    // Option 1: %rax / %rdx = 4
    // Option 2: 3 or 2
    // Option 3: %rdx / %rax = 0
    // Option 4: %rdx / %rdx = 1

    mov $longfmt, %rdi
    mov %rax, %rsi
    mov $0, %al
    call printf

    leave
    ret

    .data
longfmt: .string "%ld\n"
