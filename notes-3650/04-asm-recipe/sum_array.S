.global main
.text
main:
  push %r12 /* n */
  push %r13 /* xs */
  /* rcx is temp and ii */
  enter $0, $0

  cmp $2, %rdi
  je args_ok
  call abort

args_ok:
  /* long n = atol(argv[1]); */
  mov 8(%rsi), %rdi
  call atol
  mov %rax, %r12

  /* long* xs = alloca(n * sizeof(long)) */
  mov $8, %rcx
  imul %rcx

  /* alloca */
  sub %rax, %rsp
  mov %rsp, %r13
  /* end alloca */
  /*
  mov %rax, %rdi
  call malloc
  mov %rax, %r13
  */

  /* align stack, reguardless of value of n */
  mov $15, %rcx
  not %rcx
  and %rcx, %rsp

  /* for (ii = 0; ii < n; ++ii) { xs[ii] = ii } */
  mov $0, %rcx
fill_loop_cond:
  cmp %r12, %rcx
  jge fill_loop_done

  push %rcx
  push %rcx
  mov $longfmt, %rdi
  lea (%r13,%rcx,8), %rsi
  mov $0, %al
  call scanf
  pop %rcx
  pop %rcx

  cmp $1, %rax
  je scanf_ok

  call abort

scanf_ok:


  inc %rcx
  jmp fill_loop_cond

fill_loop_done:

  /* printf("...", sum_array(xs)); */
  mov %r13, %rdi
  mov %r12, %rsi
  call sum_array

  mov $fmt, %rdi
  mov %rax, %rsi
  mov $0, %al
  call printf

  /*
  mov %r13, %rdi
  call free
  */

  leave
  pop %r13
  pop %r12
  ret

sum_array:
  /* %rdi is xs */
  /* %rsi is nn */
  /* %rcx is ii */
  /* %rax is yy */
  enter $0, $0

  mov $0, %rax
  mov $0, %rcx
sum_loop_cond:
  cmp %rsi, %rcx
  jge sum_loop_done

  add (%rdi,%rcx,8), %rax

  inc %rcx
  jmp sum_loop_cond

sum_loop_done:
  leave
  ret

.data
fmt: .string "sum = %ld\n"
longfmt: .string "%ld"
