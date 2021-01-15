.global main
.text
main:
  /* calling a function pushes return address */
  enter $16, $0
  /* enter $16, ...:
      - push %rbp, mov %rsp, %rbp
      - moves %rsp down 16 bytes
      - so we have 16 bytes starting at %rsp */

  /*
  Pseudocode:

  long x;
  scanf("%ld", &x);
  long y = 1;
  for (int ii = x; ii > 0; ii--) {
    y = y * ii;
  }
  printf("fact(%ld) = %ld\n", x, y);
  */

  /*
  Where are we storing what?

   - x is the 8 bytes in memory starting at %rsp
   - y is %rbx (no... %rax)
   - ii is %rcx
  */

  mov $scanfmt, %rdi
  lea (%rsp), %rsi
  mov $0, %al
  call scanf

  mov $1, %rax

  mov (%rsp), %rcx
loop_cond:
  cmp $0, %rcx
  jle loop_done

  imul %rcx

  dec %rcx
  jmp loop_cond

loop_done:
  mov $printfmt, %rdi
  mov (%rsp), %rsi
  mov %rax, %rdx
  mov $0, %al
  call printf

  leave
  /* leave restores stack pointer to before enter */
  /*   mov %rbp, %rsp; pop %rbp */
  ret
  /* ret pops the return address */

.data
scanfmt: .string "%ld"
printfmt: .string "fact(%ld) = %ld\n"

