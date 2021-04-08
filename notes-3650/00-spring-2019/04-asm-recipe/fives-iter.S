.global main

.data
all_fives: .string "all fives"
not_all_fives: .string "not all fives"

.text
// Input: A string as argv[1]
// Output:
//   - If the input string contains only '5's, print "all fives".
//   - Else, print "not all fives".

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

/*
int
all_match(char c, char* text)
{
    for (int ii = 0; text[ii] != 0; ++ii) {
        if (text[ii] != c) {
            return 0;
        }
    }
    return 1;
}

Variable mapping:
 - ii: %r10
 - c: %rdi
 - text: %rsi
 - temp: %r9
 - return value defaulting to 1: %rax
*/


all_matches:
    enter $0, $0
    mov $1, %rax

    mov $0, %r10
loop_cond:
    // text[ii] != 0 
    movb (%rsi,%r10,1), %r9b
here1:
    cmpb $0, %r9b
    je loop_done

   
here2:
    cmpb %dil, (%rsi,%r10,1)
    je all_matches_loop_else

    // body of the if
    mov $0, %rax
    jmp loop_done
    
all_matches_loop_else:
    
    inc %r10
    jmp loop_cond
loop_done:

    leave
    ret


/*
int
main(int argc, char* argv[])
{
    if (all_match('5', argv[1])) {
        puts "All fives";
    }
    else {
        puts "Not all fives";
    }
    return 0;
}
*/

/* Variable mappings:
   Nothing?
*/

main:
    enter $0, $0

    mov $'5, %rdi
    mov 8(%rsi), %rsi
    call all_matches

    cmp $0, %rax
    je main_else
main_then:
    mov $all_fives, %rdi
    call puts
    jmp main_done
main_else:
    mov $not_all_fives, %rdi
    call puts
main_done:
    mov $0, %rax
    leave
    ret



