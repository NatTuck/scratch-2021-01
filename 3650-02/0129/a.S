/*
    int
    main(int argc, char* argv[])
	{
        if (argv[1][0] == 'a') {
            print "yup"         ;
	    }
        return 0                ;
	}
Vars:
	- argv is %rsi
	- argv[1] is %r10
	- argv[1][1] is %dl
    */
    .global main
    .text
main:
    enter $0, $0

    mov 8(%rsi), %r10    // movq
    mov 1(%r10), %dl     // movb

    cmp $'a, %dl
    jne no_print

    mov $yup, %rdi
    call puts

no_print:
    mov $0, %rax
    leave
    ret

    .data
yup:    .string "yup"
