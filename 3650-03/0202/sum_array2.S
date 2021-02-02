    .global main
/*
long
sum_array(long* xs, long nn)
{
    long yy = 0;
    for (long ii = 0; ii < nn; ++ii) {
        yy += xs[ii];
    }
    return yy;
}
Variables:
	- xs is %rdi
	- nn is %rsi
	- ii is %rdx
	- yy is %rax
*/
sum_array:
    enter $0, $0
    mov $0, %rax
    mov $0, %rdx
sum_array_cond:
    cmp %rsi, %rdx   // while(ii < nn)
    jge sum_array_done

    add (%rdi,%rdx,8), %rax   // %rdi + 8*%rdx (8 is sizeof(xs[ii]))

    inc %rdx
    jmp sum_array_cond
sum_array_done:
    leave
    ret

/*
int
main(int argc, char* argv[])
Vars:
	- argc is %rdi
	- argv is %rsi
	- nn is %r12
	- xs is %rbx
	- ii is %r13
	- 8  is %r8
{
    */
main:
    push %r12
    push %rbx
    push %r13
    enter $8, $0

    /*
    if (argc != 2) {
        puts("Wrong");
        return 1;
    }
	*/
    cmp $2, %rdi
    je main_args_ok

    mov $wrong, %rdi
    call puts
    mov $1, %rax
    jmp main_done

main_args_ok:
    //long nn = atol(argv[1]);
    mov 8(%rsi), %rdi
    call atol
    mov %rax, %r12

    mov $8, %r8
    imul %r8
    mov %rax, %rdi
    call malloc

    mov %rax, %rbx

    /*
    for (long ii = 0; ii < nn; ++ii) {
        long count = scanf("%ld", &(xs[ii]));
        if (count != 1) { abort(); }
    }
	*/

    mov $0, %r13   // ii = 0
main_loop_cond:
    cmp %r12, %r13
    jge main_loop_done

    mov $sfmt, %rdi
    //mov (%rbx,%r13,8), %rsi
    lea (%rbx,%r13,8), %rsi
    mov $0, %al
    call scanf

    cmp $1, %rax
    jne do_abort

    inc %r13
    jmp main_loop_cond
do_abort:
    call abort
main_loop_done:

    //printf("Sum = %ld\n", sum_array(xs, nn));
    mov %rbx, %rdi
    mov %r12, %rsi
    call sum_array

    mov $pfmt, %rdi
    mov %rax, %rsi
    mov $0, %al
    call printf

    mov %rbx, %rdi
    call free

    //return 0;
    mov $0, %rax
main_done:
    leave
    pop %r13
    pop %rbx
    pop %r12
    ret

    .data
wrong:  .string "Wrong!"
sfmt:   .string "%ld"
pfmt:   .string "Sum = %ld\n"

/*
    long nn = atol(argv[1]);

    //long xs[nn]; // This is a "new" C feature, introduced
                   // in the 1999 C standard.
                   // This allocates the array on the stack.
    long* xs = alloca(nn * sizeof(long));

    for (long ii = 0; ii < nn; ++ii) {
        long count = scanf("%ld", &(xs[ii]));
        if (count != 1) { abort(); }
    }

    printf("Sum = %ld\n", sum_array(xs, nn));

    return 0;
}
*/
