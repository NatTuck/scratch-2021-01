
    // lines.S
    //  - reads a file called "msg.txt"
    //  - count the lines (= '\n' chars) in it
    //  - print the count

    .global main
    .text
main:
    push %r14 // file descriptor for "msg.txt"
    push %r15 // size of msg.txt contexts
    enter $256, $0
    // 0(%rsp)   - read buffer, 192 bytes
    // char read_buffer[192];
    //
    // 192(%rsp) - print buffer, 64 bytes
    // char print_buffer[64];

    mov $intro, %rdi
    call strlen

    mov $1, %rdi     // fd 1 = stdout
    mov $intro, %rsi // buffer
    mov %rax, %rdx   // strlen(buffer)
    mov $1, %rax     // syscall 1 = write
    syscall

    mov $file, %rdi
    mov $0, %rsi     // O_RDONLY = 0
    mov $2, %rax     // syscall 2 = open
    syscall
    mov %rax, %r14   // fd of open file

    mov %r14, %rdi     // fd
    lea 0(%rsp), %rsi  // &buf
    mov $192, %rdx     // buffer size
    mov $0, %rax       // syscall 0 = read
    syscall
    mov %rax, %r15     // size of file data

    mov %r14, %rdi     // open file
    mov $3, %rax       // close
    syscall

    mov $0, %rcx       // ii
    mov $0, %rdx       // count
loop_cond:
    cmp %r15, %rcx
    jge loop_done

    lea 0(%rsp), %r9         // %r9 is buf
    mov (%r9,%rcx,1), %al    // %al = buf[ii];

    cmp $'\n', %al
    jne loop_next

    inc %rdx

loop_next:
    inc %rcx
    jmp loop_cond
loop_done:
    lea 192(%rsp), %rdi    // &pbuf
    mov $fmt, %rsi         // "%ld"
    mov %rdx, %rdx
    mov $0, %al
    call sprintf

    mov $1, %rdi   // fd 1 = stdout
    lea 192(%rsp), %rsi  // print buffer
    mov %rax, %rdx  // print buffer size
    mov $1, %rax   // write
    syscall

    leave
    pop %r15
    pop %r14
    ret

    .data
intro:  .string "lines in msg.txt:\n"
file:   .string "msg.txt"
fmt:    .string "lines = %ld\n"
