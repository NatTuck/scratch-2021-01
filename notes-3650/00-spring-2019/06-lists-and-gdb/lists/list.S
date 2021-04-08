.text

.global cons
cons:
  push %r12 // data
  push %r13 // next
  enter $0, $0
  mov %rdi, %r12
  mov %rsi, %r13
  mov $16, %rdi // malloc(sizeof(cell));
  call malloc
  mov %r12, 0(%rax) // xs->data = data;
  mov %r13, 8(%rax) // xs->next = next;
  leave
  pop %r13 // leave out the pops
  pop %r12
  ret

.global free_list
free_list:
  // xs is 0(%rsp)
  enter $16, $0  // start with enter $0, $0
  cmp $0, %rdi
  je free_list_done // start with no if
  mov %rdi, 0(%rsp)
  call cdr
  mov %rax, %rdi
  call free_list
  mov 0(%rsp), %rdi
  call free
free_list_done:
  leave
  ret

.global car
car:
  // xs is %rdi
  enter $0, $0
  mov 0(%rdi), %rax // start with lea
  leave
  ret

.global cdr
cdr:
  // xs is %rdi
  enter $0, $0
  mov 8(%rdi), %rax // start with lea
  leave
  ret

.global reverse
reverse:
  push %r12 // xs is %r12
  push %r13 // ys is %r13
  enter $0, $0
  mov %rdi, %r12
  mov $0, %r13
reverse_loop_cond: // while(xs)
  cmp $0, %r12
  je reverse_done
  mov %r12, %rdi
  call car
  mov %rax, %rdi
  mov %r13, %rsi
  call cons
  mov %rax, %r13  // ys = cons(car(xs), ys)
  mov %r12, %rdi
  call cdr   // typo this as "car"
  mov %rax, %r12 // xs = cdr(xs)
  jmp reverse_loop_cond
reverse_done:
  mov %r13, %rax
  leave
  pop %r13
  pop %r12
  ret

