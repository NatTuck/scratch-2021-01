    .global main
    .text
/*
long
iterate(long x)
{
    if (x % 2 == 0) {
        return x / 2;
    }
    else {
        return x*3 + 1;
    }
}
Variable mappings:
	- x is %rdi
	- (x%2) is %rdx
	- (x*3) is %rax
	- (x/2) is %rax
	- $2 is %r10
*/

iterate:
    enter $0, $0
    mov %rdi, %rax

    mov $2, %r10
    cqo
    idiv %r10         // overwrites %rax, %rdx

    cmp $0, %rdx
    je even
odd: // else
    mov $3, %r10
    mov %rdi, %rax
    imul %r10         // overwrites %rax, %rdx
    add $1, %rax

even: // then
    // didn't we just divide %rax by 2?
    // don't need to repeat the same idiv

    leave
    ret


/*
int
main(int argc, char* argv[])
{
    long xx = atol(argv[1]);
    long ii = 0;

    while (xx > 1) {
        printf("%ld\n", xx);
        xx = iterate(xx);
        ii++;
    }

    printf("%ld\n", ii);
    return 0;
}
Variable mappings:
	- xx is %rbx
	- ii is %r12
    */
main:
    push %rbx
    push %r12
    // no stack allocations & no alignment fix
    enter $0, $0

    //long xx = atol(argv[1]);
    //    why 8? sizeof(argv[0]) = 8, so that's the
    //    offset to get from argv[0] to argv[1]
    mov 8(%rsi), %rdi
    call atol
    mov %rax, %rbx

    //long ii = 0;
    mov $0, %r12

    /*
    while (xx > 1) {
        printf("%ld\n", xx);
        xx = iterate(xx);
        ii++;
    }
    */

main_loop_cond:
    cmp $1, %rbx
    jle main_loop_done

    mov $longfmt, %rdi
    mov %rbx, %rsi
    mov $0, %al
    call printf

    mov %rbx, %rdi
    call iterate
    mov %rax, %rbx

    inc %r12
    jmp main_loop_cond
main_loop_done:

    // printf("%ld\n", ii);
    mov $longfmt, %rdi
    mov %r12, %rsi
    mov $0, %al   // calling convention thing
                  // for vararg functions
    call printf


    //return 0
    mov $0, %rax
    leave
    pop %r12
    pop %rbx
    ret

    .data
longfmt:    .string "%ld\n"
