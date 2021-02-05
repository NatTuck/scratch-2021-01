    .global main
    .text

// open "msg.txt"
// count the lines (= '\n' chars) in it
// print the count
//
// We're doing all our system calls manually

main:
    push %r14     // file descriptor of msg.txt
    push %r15     // # of bytes read from msg.txt
    enter $256, $0
    // 0(%rsp) is our read buffer (192 bytes)
    // char rdbuf[192];
    //
    // 192(%rsp) is our print buffer (64 bytes)
    // char prbuf[64];

    mov $intro, %rdi
    call strlen

    mov $1, %rdi        // stdout = fd 1
    mov $intro, %rsi    // buffer is intro
    mov %rax, %rdx      // strlen(intro)
    mov $1, %rax        // syscall 1 = write
    syscall

    // fd = open("msg.txt", O_RDONLY)
    mov $file, %rdi
    mov $0, %rsi
    mov $2, %rax
    syscall
    mov %rax, %r14

    // size = read(fd, rdbuf, 192);
    mov %r14, %rdi
    lea 0(%rsp), %rsi
    mov $192, %rdx
    mov $0, %rax
    syscall
    mov %rax, %r15

    // close(fd)
    mov $3, %rax
    mov %r14, %rdi
    syscall

    // count = 0;
    // for (ii = 0; ii < size; ++ii) {
    //   if (rdbuf[ii] == '\n') count++;
    // }
    mov $0, %rcx   // ii
    mov $0, %r8    // count
loop_cond:
    cmp %r15, %rcx
    jge loop_done

    lea 0(%rsp), %r10
    mov (%r10,%rcx,1), %r9b     // r9b = buf[ii]
                 // mov => lea  == r9b = &(buf[ii])
    cmp $'\n', %r9b
    jne loop_next

    inc %r8

loop_next:
    inc %rcx
    jmp loop_cond
loop_done:

    // nn = sprintf(prbuf, fmt, count)
    lea 192(%rsp), %rdi
    mov $fmt, %rsi
    mov %r8, %rdx
    mov $0, %al
    call sprintf

    // write(1, prbuf, nn)
    mov $1, %rdi
    lea 192(%rsp), %rsi
    mov %rax, %rdx
    mov $1, %rax
    syscall

    mov $0, %rax
    leave
    pop %r15
    pop %r14
    ret

    .data
intro:  .string "lines in msg.txt:\n"
file:   .string "msg.txt"
fmt:    .string "lines = %ld\n"
