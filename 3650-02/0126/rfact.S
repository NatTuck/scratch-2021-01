    .global main
    .text
    /*
    fact(x):
	   if (x <= 1): return 1
	   return x * fact(x-1)
    */
fact:
    // x is 0(%rsp)
    enter $16, $0
    mov %rdi, 0(%rsp)

    mov $1, %rax
    cmp $1, %rdi
    jle fact_done

    dec %rdi
    call fact

    imulq 0(%rsp)

fact_done:
    leave
    ret

main:
    // x is 8(%rsp)
    // y is %rax
    // ii is %rcx
    enter $16, $0

    //long x
	//scanf("%ld", &x)
    mov $scanfmt, %rdi
    //mov 8(%rsp), %rsi  // scanf(..., x)
    lea 8(%rsp), %rsi  // scanf(..., &x)
    mov $0, %al
    call scanf

    mov 8(%rsp), %rdi
    call fact

    mov $printfmt, %rdi
    mov 8(%rsp), %rsi
    mov %rax, %rdx
    mov $0, %al
    call printf

    mov $0, %rax
    leave
    ret

    .data
scanfmt:    .string "%ld"
printfmt:   .string "fact(%ld) = %ld\n"
