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
        return 3*x + 1;
    }
}
Variable mapping:
	- x is %rdi
	- (x%2) is %rdx
	- (3*x) is %rax
	- temp is %r10
    */
iterate:
    enter $0, $0

    mov %rdi, %rax
    mov $2, %r10
    cqo // sign extend %rax into %rdx, since
        // idiv takes %rdx as the top 64-bits of input
    idiv %r10

    cmp $0, %rdx
    je even
odd: // else
    mov $3, %rax
    imul %rdi
    inc %rax

    jmp iterate_done
even: // then
    // do nothing, we already divided by 2 into %rax

iterate_done:
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

    printf("ii = %ld\n", ii);

    return 0;
}
Variable mappings:
	- argc in %rdi
	- argv in %rsi
	- xx in %r12
	- ii in %r13
    */

main:
    push %r12
    push %r13
    enter $0, $0

    //long xx = atol(argv[1]);
    mov 8(%rsi), %rdi
    call atol
    mov %rax, %r12

    // long ii = 0;
    mov $0, %r13

    /*
    while (xx > 1) {
        printf("%ld\n", xx);
        xx = iterate(xx);
        ii++;
    }
	*/
main_loop_cond:
    cmp $1, %r12
    jle main_loop_done

    mov $longfmt, %rdi
    mov %r12, %rsi
    mov $0, %al
    call printf

    mov %r12, %rdi
    call iterate
    mov %rax, %r12

    inc %r13
    jmp main_loop_cond
main_loop_done:

    //printf("%ld\n", ii);
    mov $longfmt, %rdi
    mov %r13, %rsi
    mov $0, %al
    call printf

    leave
    pop %r13
    pop %r12
    ret

    .data
longfmt:    .string "%ld\n"

    /*
	Caller save pattern:
    push %r10
    sub $8, %rsp

    call foo
    add $8, %rsp
    pop %r10
    */

/*
func:
    push %rbx
    enter $40, $0 // added 8 here
*/
