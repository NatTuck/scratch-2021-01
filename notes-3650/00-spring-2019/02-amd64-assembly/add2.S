// add2.S

.global main

.text
  /*
  long add2(long x)
   - the argument comes in in %rdi
   - we return the result by putting it in %rax
  */
add2:
  enter $0, $0

  // long y = x;
  mov %rdi, %rax

  // y = y + 2;
  add $2, %rax

  // return y;
  leave
  ret

main:
  enter $0, $0

  // long x = 5;
  mov $5, %rdi

  // y = add1(x)
  call add2
  // result in %rax

  /*
  printf("%ld\n", y)
    - first arg goes in %rdi
    - second arg goes in %rsi
    - for a variable arg function, we need to zero %al
      - %al is the bottom 8 bits of %ax/%eax/%rax
  */
  mov $long_fmt, %rdi
  mov %rax, %rsi
  mov $0, %al
  call printf

  leave
  ret

.data
long_fmt: .string "%ld\n"
