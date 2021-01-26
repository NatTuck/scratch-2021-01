    .global main
    .text
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

    /*
	long y = 1
	for (int ii = x; ii > 0; ii--) {
        y = y * ii
	}
    printf("fact({x}) = {y}")
	*/

    mov $1, %rax
    mov 8(%rsp), %rcx
loop_cond:
    cmp $0, %rcx
    jle loop_done

    imul %rcx  // %rax *= %rcx ; y = y * ii

    dec %rcx
    jmp loop_cond
loop_done:

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
