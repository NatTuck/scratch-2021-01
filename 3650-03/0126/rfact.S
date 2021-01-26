    .global main
    .text
/*
	fact(x):
	    if x <= 1: return 1
	    return x * fact(x-1)
*/
fact:
    push %rbx
    enter $8, $0
    mov %rdi, %rbx

    mov $1, %rax
    cmp $1, %rdi
    jle fact_done

    //push %rdi

    dec %rdi // %rdi = x - 1
    call fact

    //pop %rdi
    //imul %rdi
    imul %rbx

fact_done:
    leave
    pop %rbx
    ret

main:
    // x is 8(%rsp)
    enter $16, $0

    mov $scanfmt, %rdi
    lea 8(%rsp), %rsi
    mov $0, %al
    call scanf

    mov 8(%rsp), %rdi
    call fact

    mov $printfmt, %rdi
    mov 8(%rsp), %rsi
    mov %rax, %rdx
    mov $0, %al
    call printf

    leave
    ret

    .data
scanfmt:    .string "%ld"
printfmt:   .string "fact(%ld) = %ld\n"
