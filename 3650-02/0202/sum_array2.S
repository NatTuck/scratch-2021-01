    .global main
    .text
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
Variable mappings:
	- xs is %rdi
	- nn is %rsi
	- yy is %rax
    - ii is %r9
	- xs[ii] ?
    */
sum_array:
    enter $0, $0

    mov $0, %rax
    mov $0, %r9
sum_loop_cond:
    cmp %rsi, %r9      // if (ii >= nn)
    jge sum_loop_done

    // aa[bb] is (aa,bb,sizeof(aa[bb]))
    add (%rdi,%r9,8), %rax

    inc %r9
    jmp sum_loop_cond
sum_loop_done:
    leave
    ret

/*
int
main(int argc, char* argv[])
{
    if (argc != 2) {
        puts("Usage: ./sum_array NN");
        return 1;
    }
Variables:
	- argc is %rdi
	- argv is %rsi
	- nn is %r12
	- xs is %r13
	- ii is %r14
	- 8  is %r10
	- count is %rax
    */
main:
    push %r12
    push %r13
    push %r14
    enter $8, $0

    cmp $2, %rdi
    je arg_count_good

    mov $umsg, %rdi
    call puts
    mov $1, %rax
    jmp main_done

arg_count_good:

    // long nn = atol(argv[1]); // Number of items.
    mov 8(%rsi), %rdi
    call atol
    mov %rax, %r12

    // long* xs = malloc(nn * sizeof(long));
    //mov $8, %r10
    //imul %r10
    //mov %rax, %rdi
    mov $0, %rax
    lea (%rax,%r12,8), %rdi
    call malloc
    mov %rax, %r13

    /*
    for (long ii = 0; ii < nn; ++ii) {
        int count = scanf("%ld", &(xs[ii]));
        if (count != 1) { abort(); }
    }
    */
    mov $0, %r14
main_for_cond:
    cmp %r12, %r14
    jge main_for_done

    mov $sfmt, %rdi
    lea (%r13,%r14,8), %rsi
    mov $0, %al // calling convention for varargs
    call scanf

    cmp $1, %rax
    jne do_abort

    inc %r14
    jmp main_for_cond

do_abort:
    call abort

main_for_done:

    // printf("Sum = %ld\n", sum_array(xs, nn));
    mov %r13, %rdi
    mov %r12, %rsi
    call sum_array

    mov $pfmt, %rdi
    mov %rax, %rsi
    mov $0, %al
    call printf

    mov %r13, %rdi
    call free

    // return 0;
    mov $0, %rax
main_done:
    leave
    pop %r14
    pop %r13
    pop %r12
    ret


    .data
umsg:   .string "wrong!"
sfmt:   .string "%ld"
pfmt:   .string "Sum = %ld\n"
