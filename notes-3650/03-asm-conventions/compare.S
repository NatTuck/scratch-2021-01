.global main
.text

  /*
	long aa = atol(argv[1])
	long bb = atol(argv[2])
  if (aa > bb) {
    print "{aa} > {bb}";
  else
    print "{aa} <= {bb}"
*/

main:
  push %r12 // argv
  push %r14 // aa
  push %r15 // bb
  enter $8, $0
  mov %rsi, %r12

  cmp $3, %rdi
  je main_cmp

  mov $umsg, %rdi
  call puts
  mov $1, %rax
  jmp done

main_cmp:
  mov 8(%r12), %rdi
  call atol
  mov %rax, %r14

  mov 16(%r12), %rdi
  call atol
  mov %rax, %r15

  cmp %r15, %r14
  jg big
  jmp sml

sml:
  mov $le, %rax
  jmp print

big:
  mov $gt, %rax
  jmp print

print:
  mov $msg, %rdi
  mov %r14, %rsi
  mov %rax, %rdx
  mov %r15, %rcx
  mov $0, %al
  call printf
  mov $0, %rax

done:
  leave
  pop %r15
  pop %r14
  pop %r12
  ret

.data
msg:  .string "%d %s %d\n"
gt:   .string ">"
le:   .string "<="
umsg: .string "Usage: ./compare NN NN"
