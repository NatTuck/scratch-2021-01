    .global main
    .text
/*
    long x
	scanf("%ld", &x)
	long y = 1
	for (int ii = x; ii > 0; ii--) {
        y = y * ii
	}
	print "fact({x}) = {y}"
*/

main:
    // x is 8(%rsp)
    // y is %rax
    // ii is %rcx
    enter $16, $0

    //long x
	//scanf("%ld", &x)
    mov $scanfmt, %rdi
    lea 8(%rsp), %rsi  // just calculate address
    // mov 8(%rsp), %rsi // this would be x, not &x
    //mov %rsp, %rsi
    //add $8, %rsi
	// problem: registers aren't in memory
    //          they don't have addresses.
    mov $0, %al
    call scanf

    /*
	long y = 1
	for (int ii = x; ii > 0; ii--) {
        y = y * ii
	}
	print "fact({x}) = {y}"
    */

    mov $1, %rax
    mov 8(%rsp), %rcx
loop_cond:
    cmp $0, %rcx
    jle loop_done

    imul %rcx  // %rax = %rax * %rcx

    dec %rcx
    jmp loop_cond
loop_done:

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
