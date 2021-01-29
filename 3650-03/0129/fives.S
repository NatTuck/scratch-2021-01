/*
int
all_match(char c, char* text)
{
    if (*text == 0) {
        return 1;
    }

    if (*text == c) {
        return all_match(c, text + 1);
    }

    return 0;
}
Variable mappings:
	- c is %dil
	- text is %rsi
    - *text is %r10b
    */

    .global main
    .text
all_match:
    enter $0, $0

    mov (%rsi), %r10b // dest reg is 1b, so copy 1b
    cmp $0, %r10b
    je done_true

    cmp %dil, %r10b
    jne done_false

    inc %rsi
    call all_match
    jmp done

done_false:
    mov $0, %eax
    jmp done

done_true:
    mov $1, %eax

done:
    leave
    ret

/*
int
main(int argc, char* argv)
{
    if (all_match('5', argv[1])) {
        puts("all fives");
    }
    else {
        puts("not all fives");
    }
    return 0;
}
Variable mapping:
	- this is an empty list
    */
main:
    enter $0, $0

    mov $'5, %dil
    mov 8(%rsi), %rsi
    call all_match

    cmp $0, %eax
    je nope
yup:
    mov $yesall, %rdi
    call puts
    jmp main_done

nope:
    mov $notall, %rdi
    call puts

main_done:
    mov $0, %eax // return an int, so %eax
    leave
    ret


    .data
yesall: .string "all fives"
notall: .string "not all fives"
