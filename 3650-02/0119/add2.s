
.global add2

// add2(long xx)
//   - xx, first argument, goes in register %rdi
//   - return value is what's in %rax when we return
add2:
    enter $0, $0

    mov %rdi, %rax
    add $2, %rax     // rax += 2

    leave
    ret
