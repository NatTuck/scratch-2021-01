.global main
.text
main:
  /* allocate 16 bytes on stack */
  enter $16, $0
  /* enter moves %rsp down 16 bytes,
     so we have 16 bytes starting at %rsp */

  /*
  Pseudocode:

  main() {
    long x;             // x is 0(%rsp)
    scanf("%ld", &x);
    long y = fact(x);
    printf("fact(%ld) = %ld\n", x, y);
  }

  */

  mov $scanfmt, %rdi
  lea (%rsp), %rsi
  mov $0, %al
  call scanf

  mov (%rsp), %rdi
  call fact

  mov $printfmt, %rdi
  mov (%rsp), %rsi
  mov %rax, %rdx
  mov $0, %al
  call printf

  leave
  ret

fact:
  /* 8 is padding, skip initially */
  enter $8, $0

  /*
  	fact(x) {
	    if (x <= 1) return 1;
	    return x * fact(x-1);
	  }
  */

  mov $1, %rax
  cmp $1, %rdi
  jle fact_done

  push %rdi /* skip initially */
  dec %rdi
  call fact

  pop %rdi /* skip initially */
  imul %rdi /* wait, we lost x */

  /* after doing caller-save,
     try callee-save with %rbx */

fact_done:
  leave
  ret


.data
scanfmt: .string "%ld"
printfmt: .string "fact(%ld) = %ld\n"

