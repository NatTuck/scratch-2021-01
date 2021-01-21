.global add2


// long add2(long xx)
//   - xx is stored in %rdi, the first argument register
//   - the return value is whatever's in %rax
//     when we return

add2:
    enter $0, $0
 
    mov %rdi, %rax
    add $2, %rax

    leave
    ret

