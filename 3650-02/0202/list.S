    .global main

/*
	A LinkedList is one of:
	- 0 (a null pointer), for an empty list
	- The (non-zero) address of a Cell

	A Cell is a struct with two fields:
	- head: A long
	- tail: A LinkedList
*/

/*
    Linked list operations:
    - cons
	- first (aka car)
	- rest  (aka cdr)
	- free_list
*/


    .text
car:
    enter $0, $0
    mov 0(%rdi), %rax
    leave
    ret

cdr:
    enter $0, $0
    mov 8(%rdi), %rax
    leave
    ret

    // (cons hd tl) => (cell hd tl)
cons:
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

free_list:
    push %r12 // cell ptr
    enter $8, $0
    mov %rdi, %r12

    call cdr
    cmp $0, %rax
    je free_this_item

    mov %rax, %rdi
    call free_list

free_this_item:
    mov %r12, %rdi
    call free
    leave
    pop %r12
    ret

sum:
    push %r12 // list
    push %r13 // sum of rest
    enter $0, $0

    // (if (empty? %rdi)
    //     0
    //     (+ (car=first %rdi)
    //        (sum (cdr=rest %rdi)))

    mov $0, %rax
    cmp $0, %rdi
    je sum_done

    mov %rdi, %r12

    call cdr
    call sum

    mov %rax, %r13

    mov %r12, %rdi
    call car

    add %r13, %rax

sum_done:
    leave
    pop %r13
    pop %r12
    ret

main:
    push %r12
    enter $8, $0

    mov $10, %rdi
    mov $0, %rsi
    call cons

    mov $20, %rdi
    mov %rax, %rsi
    call cons
    mov %rax, %r12

    mov %r12, %rdi
    call sum

    mov $pfmt, %rdi
    mov %rax, %rsi
    mov $0, %al
    call printf

    mov %r12, %rdi
    call free_list

    leave
    pop %r12
    ret

    .data
pfmt:   .string "%ld\n"
