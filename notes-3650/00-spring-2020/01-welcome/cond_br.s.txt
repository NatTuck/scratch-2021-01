.global main
.text

main:
  enter $0, $0

  # print prompt
  mov $prompt, %rdi
  call puts

  mov $long_fmt, %rdi
  mov $num, %rsi
  mov $0, %al
  call scanf

  # copy value at address
  # with dollar sign, copy literal address
  mov num, %rax

  # if (%rax <= 10)
  cmp $10, %rax
  jle smaller_than_ten

bigger_than_ten:
  mov $bigger, %rdi
  jmp main_done

smaller_than_ten:
  mov $smaller, %rdi

main_done:
  call puts

  leave
  ret

.data
num: .string "12345678" # 8 bytes, to fit a long
prompt: .string "enter a number"
long_fmt: .string "%ld"
eol: .string "\n"
bigger: .string "bigger than ten"
smaller: .string "smaller than ten"
