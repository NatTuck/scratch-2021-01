    .global main
    .text
    /*
if (aa > bb) {
	print "{aa} > {bb}"
}
else {
    print "{aa} <= {bb}"
}
	*/

main:
    // argc is %rdi (arguably %edi)
    // argv is %rsi => %rbx ; type is char**
    push %rbx
	push %r12 // atol(argv[1]) is %r12
    push %r13 // atol(argv[2]) is %r13
    enter $8, $0

    // to main alignment
    //  # of pushes * 8 + enter $XX must be multiple
    //  of 16

    mov %rsi, %rbx

    cmp $3, %rdi
    je main_cmp

    mov $usage, %rdi
    call puts

    mov $1, %rax
    jmp main_done

main_cmp:
    mov 8(%rbx), %rdi  // move data from %rsi+8 to %rdi
    call atol
    mov %rax, %r12

    mov 16(%rbx), %rdi
    call atol
    mov %rax, %r13

    cmp %r13, %r12  // cmp should be backwards
    jg greater_than
    mov $le, %rdx
    jmp print_msg

greater_than:
    mov $gt, %rdx

print_msg:
    mov $msg, %rdi
    mov %r12, %rsi
    mov %r13, %rcx
    mov $0, %al
    call printf

main_done:
    leave
    pop %r13
	pop %r12
    pop %rbx
    ret

    .data
msg:    .string "%d %s %d\n"
gt:     .string ">"
le:     .string "<="
usage:  .string "Usage: ./compare NN NN"
