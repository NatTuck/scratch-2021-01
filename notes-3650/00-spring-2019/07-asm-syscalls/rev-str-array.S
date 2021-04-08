  .global main
  .text
  /* char** reverse_string_array(char** xs, int nn) */
reverse_string_array:
  /* xs is %rdi */
  /* nn is %rsi */
  /* ys is %rax */
  /* ii is %rcx */
  /* tmps are %rdx, %r9 */
  enter $0, $0

  /*  char** ys = malloc(nn * sizeof(char*)); */
  mov %rsi, %rax
  mov $8, %rdx
  imul %rdx

  push %rdi
  push %rsi

  mov %rax, %rdi
  call malloc

  pop %rsi
  pop %rdi

  /*	for (int ii = 0; ii < nn; ++ii) { */
  mov $0, %rcx
rsa_loop_cond:
  cmp %rsi, %rcx
  jge rsa_loop_done

  /* ys[nn - ii - 1] = xs[ii]; */
  mov %rsi, %rdx
  sub %rcx, %rdx
  dec %rdx

  mov (%rdi,%rcx,8), %r9
  mov %r9, (%rax,%rdx,8)

  inc %rcx
  jmp rsa_loop_cond

rsa_loop_done:
  /* return ys; */
  leave
  ret

/* void print_string_array(char** xs, int nn) */
print_string_array:
  push %r12 /* xs is %r12 */
  push %r13 /* nn is %r13 */
  push %r14 /* ii is %r14 */
  push %r15
  /* tmp is %rdx */
  enter $0, $0
  mov %rdi, %r12
  mov %rsi, %r13

  /* for (int ii = 0; ii < nn; ++ii) { */
  mov $0, %r14
psa_loop_cond:
  cmp %r13, %r14
  jge psa_loop_done

  /* write(1, xs[ii], strlen(xs[ii])); */
  mov (%r12,%r14,8), %rdi
  call strlen

  mov $1, %rdi /* stdout */
  mov (%r12,%r14,8), %rsi
  mov %rax, %rdx
  mov $1, %rax /* write */
  syscall

  /* write("\n"); */
  mov $1, %rdi
  mov $eol, %rsi
  mov $1, %rdx
  mov $1, %rax
  syscall

  inc %r14
  jmp psa_loop_cond

psa_loop_done:
  leave
  pop %r15
  pop %r14
  pop %r13
  pop %r12
  ret

/*
  read_line spoiler:

  mov $0, %rdi
  mov buf + ii, %rsi
  mov $1, %rdx
  mov $0, %rax
  syscall
  %rax == 1 or EOF
  */


/* int main(int argc, char* argv[]) */
main:
  push %r12 /* %r12 is aa */
  push %r13 /* %r13 is bb */
  enter $0, $0
  /* char* aa[5]; */
  sub $48, %rsp
  lea 0(%rsp), %r12

  /*
    aa[0] = "one";
    aa[1] = "two";
    aa[2] = "three";
    aa[3] = "four";
    aa[4] = "five";
  */
  /* need size suffix since no registers */
  movq $one, 0(%r12)
  movq $two, 8(%r12)
  movq $three, 16(%r12)
  movq $four, 24(%r12)
  movq $five, 32(%r12)

  /* char** bb = reverse_string_array(aa, 5); */
  mov %r12, %rdi
  mov $5, %rsi
  call reverse_string_array
  mov %rax, %r13

  /* print_string_array(bb, 5); */
  mov %r13, %rdi
  mov $5, %rsi
  call print_string_array

  /* free(bb); */
  mov %r13, %rdi
  call free

  /* return 0; */
  mov $0, %rax
  leave
  pop %r13
  pop %r12
  ret

.data
one: .string "one"
two: .string "two"
three: .string "three"
four: .string "four"
five: .string "five"
eol: .string "\n"
