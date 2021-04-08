  .global main

  /*
A LinkedList is one of:
  - 0 (a null pointer), for an empty list
  - The (nonzero) address of a Cell

A Cell is a struct with two fields:
  - head: A long
  - tail: A LinkedList
  */


  .text
car:
  /* get first item from cell */
  /*  - %rdi is address of cell */
  enter $0, $0

  mov 0(%rdi), %rax

  leave
  ret

cdr:
  /* get first item from cell */
  /*  - %rdi is address of cell */
  enter $0, $0

  mov 8(%rdi), %rax

  leave
  ret

cons:
  /* allocate a linked list cell
     - arg 1 -> %r12 is an item, the head of the new list
     - arg 2 -> %r13 is a list, the tail of the new list
     - %rax - address of the new cell
  */
  push %r12
  push %r13
  enter $0, $0

  mov %rdi, %r12
  mov %rsi, %r13

  mov $16, %rdi
  call malloc

  mov %r12, 0(%rax)
  mov %r13, 8(%rax)

  leave
  pop %r13
  pop %r12
  ret

sum:
  /* add up the numbers in a list

     long
     sum(cell* xs)
     {
         if (xs == 0) {
             return 0;
         }
         else {
             return car(xs) + sum(cdr(xs));
         }
     }

     - %r12 - address of current cell
     - %r13 - current head
     - %rax - sum
  */
  push %r12
  push %r13
  enter $8, $0

  mov $0, %rax
  cmp $0, %rdi
  je sum_done

  mov %rdi, %r12

  call car
  mov %rax, %r13

  mov %r12, %rdi
  call cdr
  mov %rax, %rdi

  call sum
  add %r13, %rax

sum_done:
  leave
  pop %r13
  pop %r12
  ret

  /* initially leave out free */
  /* run in valgrind */
  /* then add free */
free_list:
  push %r12 /* current cell */
  push %r13
  enter $0, $0

  cmp $0, %rdi
  je free_list_done

  mov %rdi, %r12

  call cdr
  mov %rax, %rdi
  call free_list

  mov %r12, %rdi
  call free

free_list_done:
  leave
  pop %r13
  pop %r12
  ret

main:
  push %rbx
  push %r12  /* r12 is xs */
  enter $0, $0

  mov $10, %rdi
  mov $0, %rsi
  call cons

  mov $5, %rdi
  mov %rax, %rsi
  call cons

	mov $17, %rdi
  mov %rax, %rsi
  call cons

  mov %rax, %r12
  mov %rax, %rdi
  call sum

  mov %rax, %rsi
  mov $fmt, %rdi
  mov $0, %al
  call printf

  mov %r12, %rdi
  call free_list

  leave
  pop %r12
  pop %rbx
  ret


  .data
fmt: .string "%ld\n"

