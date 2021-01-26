/*
	$ ./compare AA BB
    if (aa > bb) {
        print "{aa} > {bb}"
    }
    else {
        print "{aa} <= {bb}"
    }
*/

    .global main
    .text
main:
	push %rbx // argv is in %rbx
    push %r12 // atol(argv[1]) is in %r12
    enter $0, $0
    mov %rsi, %rbx

    cmp $3, %rdi
    je args_good

    mov $umsg, %rdi
    call puts

    mov $1, %rax
    jmp main_done
args_good:

    mov 8(%rbx), %rdi // argv[1]
    call atol
    mov %rax, %r12

    // calculate %rbx+16, load the 64-bit
    // value at that address in memory, copy to %rdi
    mov 16(%rbx), %rdi
    call atol

    cmp %rax, %r12  // compare is backwards
    jg greater_than
    // if %r12 > %rax then goto greater_than
    // else, less than or equal

    mov $le, %rdx
    jmp print_now

greater_than:
    mov $gt, %rdx

print_now:
    mov $msg, %rdi
    mov %r12, %rsi
    mov %rax, %rcx
    mov $0, %al
    call printf

main_done:
    leave
    pop %r12
	pop %rbx
    ret

    .data
msg:    .string "%ld %s %ld\n"
gt:     .string ">"
le:     .string "<="
umsg:   .string "Usage: ./compare NN NN"
