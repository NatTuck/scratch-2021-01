.global main
.text

  /*
  if (atol(argv[1]) > 10) {
    print "Bigger";
  else
    print "Not bigger"
*/

main:
  enter $0, $0

  mov 8(%rsi), %rdi
  call atol

  cmp $10, %rax
  jg big

sml:
  mov $sml_msg, %rdi
  call puts
  jmp done

big:
  mov $big_msg, %rdi
  call puts

done:
  leave
  ret

.data
big_msg: .string "bigger"
sml_msg: .string "not bigger"


