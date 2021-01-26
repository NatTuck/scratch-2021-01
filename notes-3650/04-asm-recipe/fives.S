.global main
.data
all_fives: .string "all fives"
not_all: .string "not all fives"
intfmt: .string "%d\n"

/*
    - take a string as argv[1]
    - determine if all characters are 5's
    - print "all fives" if so
    - print "not all fives" if not
*/

.text

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
*/

/* variable mapping:
   - c: %rdi
   - text: %rsi
*/

all_match:
    enter $0, $0

    push %rdi
    push %rsi
    mov %rsi, %rdi
    call puts
    pop %rsi
    pop %rdi

    cmpb $0, (%rsi)
    jne am1_done
am1_then:
    mov $1, %rax
    jmp all_match_done 
am1_done:

    cmpb %dil, (%rsi)
    jne am2_done
am2_then:
    // correct value in %rdi 
    add $1, %rsi
    call all_match
    // mov %rax, %rax
    jmp all_match_done

am2_done:
    mov $0, %rax

all_match_done:

    push %rax
    push %rax
    mov $intfmt, %rdi
    mov %rax, %rsi
    mov $0, %al
    call printf
    pop %rax
    pop %rax

    leave
    ret


/*
int
all_match(char c, char* text)
{
    for (long ii = 0; text[ii] != 0; ++ii) {
        if (text[ii] != c) {
            return 0;
        }
    }
    return 1;
}
*/

/*
int 
main(int argc, char* argv[])
{
    if (all_match('5', argv[1])) {
        say "all fives";
    }
    else {
        say "not all fives";
    }
    return 0;
}
*/

/* variable mapping for main:
   - %r12 is the result of all_match
*/

main:
    push %r12
    enter $8, $0

    mov $'5, %rdi 
    mov 8(%rsi), %rsi
    call all_match
    mov %rax, %r12

    cmp $0, %r12
    je main_else
main_then:
    mov $all_fives, %rdi
    jmp main_done
main_else:
    mov $not_all, %rdi
main_done:     
    call puts

    mov $0, %rax
    leave
    pop %r12
    ret

