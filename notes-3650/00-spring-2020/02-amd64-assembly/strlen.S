.global main
.text
main:
  enter $64, $0

  lea (%rsp), %rdi
  mov $64, %rsi
  call readline

  lea (%rsp), %rdi
  call strlen

  mov $fmt, %rdi
  mov %rax, %rsi
  mov $0, %al
  call printf

  leave
  ret

strlen:
  enter $0, $0

  mov $0, %rax
  /* load just one byte with b suffix */
  movb (%rdi), %cl
  cmp $0, %cl
  je strlen_done

  inc %rdi
  call strlen
  inc %rax

strlen_done:
  leave
  ret

.data
fmt: .string "len = %ld\n"
