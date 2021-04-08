
/* lines.S - reads "msg.txt", counts lines ('\n' characters) */

  .global main
  .text
main:
  push %r14   /* file descriptor for msg.txt */
  push %r15   /* size of msg.txt contents */
  /* %rcx is loop counter */
  /* %rdx is count */
  /* %rax is current byte */
  enter $256, $0
  /* 0(%rsp) is read buffer, 192 bytes */
  /* 192(%rsp) is print buffer, 64 bytes */
  mov $intro, %rdi
  call strlen

  mov $1, %rdi       /* fd 1 is stdout */
  mov $intro, %rsi   /* buffer */
  mov %rax, %rdx     /* size */
  mov $1, %rax       /* syscall 1 is write */
  syscall

  mov $file, %rdi   /* file name */
  mov $0, %rsi      /* O_RDONLY ; see bits/fcntl-linux.h */
  mov $0, %rdx      /* mode is unused on read */
  mov $2, %rax      /* syscall 2 is open */
  syscall
  mov %rax, %r14    /* save fd */

  mov %r14, %rdi    /* read from file */
  lea 0(%rsp), %rsi /* read to stack */
  mov $192, %rdx    /* buffer size */
  mov $0, %rax      /* syscall 0 is read */
  syscall
  mov %rax, %r15    /* save bytes read */

  mov $0, %rcx
  mov $0, %rdx
loop_cond:
  cmp %r15, %rcx
  jge loop_done

  mov $0, %rax
  mov (%rsp,%rcx,1), %al

  cmp $'\n', %al
  jne loop_next

  inc %rdx

loop_next:
  inc %rcx
  jmp loop_cond

loop_done:
  lea 192(%rsp), %rdi
  mov $fmt, %rsi
  /* count already in %rdx */
  mov $0, %al
  call sprintf

  mov $1, %rdi        /* fd 1 is stdout */
  lea 192(%rsp), %rsi /* buffer */
  mov %rax, %rdx      /* size */
  mov $1, %rax        /* syscall 1 is write */
  syscall

  /* TODO: look up close syscall to close file */

  mov $0, %rdi
  mov $60, %rax   /* syscall 60 is exit */
  syscall

  leave
  pop %r15
  pop %r14
  ret

  .data
intro: .string "lines in msg.txt:\n"
file: .string "msg.txt"
fmt: .string "lines = %ld\n"
