    .global main

/*
	A LinkedList is one of:
	 - 0 (a null pointer), for an empty list
	 - The (non-zero) address of a Cell

	A Cell is a struct with two fields:
	 - head: A long
	 - tail: A LinkedList

    Operations:
	 - cons
	 - car (aka "first")
	 - cdr (aka "rest")
     - free_list
*/

    .text
    // (cons x ys) => { head: x, tail: ys }
cons:
    push %r12
    push %r13
    enter $0, $0
    mov %rdi, %r12
    mov %rsi, %r13

    mov $16, %rdi
    call malloc
    mov %r12, 0(%rax)   // cell->head
    mov %r13, 8(%rax)   // cell->tail

    leave
    pop %r13
    pop %r12
    ret

    // (car { head: 5, tail: ... }) => 5
car:
    enter $0, $0

    mov 0(%rdi), %rax

    leave
    ret

    // (car { head: 5, tail: ... }) => ...
cdr:
    enter $0, $0

    mov 8(%rdi), %rax

    leave
    ret

free_list:
    push %rbx
    enter $8, $0
    mov %rdi, %rbx
    cmp $0, %rdi
    je free_list_done

    call cdr
    cmp $0, %rax
    je free_this_cell

    mov %rax, %rdi
    call free_list

free_this_cell:
    mov %rbx, %rdi
    call free

free_list_done:
    leave
    pop %rbx
    ret

sum:
    push %r12  // car xs
    push %r13  // xs
    enter $0, $0

    // (if (empty? xs)
    //     0
    //     (+ (car xs)
    //        (sum (cdr xs)))

    mov $0, %rax
    cmp $0, %rdi
    je sum_done

    mov %rdi, %r13

    call car
    mov %rax, %r12

    mov %r13, %rdi

    call cdr
    mov %rax, %rdi
    call sum

    add %r12, %rax

sum_done:
    leave
    pop %r13
    pop %r12
    ret

print_cell:
    enter $0, $0

    call car

    mov $cellfmt, %rdi
    mov %rax, %rsi
    mov $0, %al
    call printf

    leave
    ret

print_list:
    push %r12
    enter $8, $0
    mov %rdi, %r12

    cmp $0, %rdi
    jne print_not_done

    mov $eol, %rdi
    mov $0, %al
    call printf
    jmp print_done

print_not_done:
    call print_cell

    mov %r12, %rdi
    call cdr
    mov %rax, %rdi
    call print_list

print_done:
    leave
    pop %r12
    ret

main:
    push %rbx
    enter $8, $0

    /*
    mov $10, %rdi
    mov $0, %rsi
    call cons

    mov $20, %rdi
    mov %rax, %rsi
    call cons

    mov $30, %rdi
    mov %rax, %rsi
    call cons
    mov %rax, %rbx
    */
    mov $0, %rbx

    mov %rbx, %rdi
    call sum

    mov $pfmt, %rdi
    mov %rax, %rsi
    mov $0, %al
    call printf

    mov %rbx, %rdi
    call print_list

    mov %rbx, %rdi
    call free_list

    mov $0, %rax
    leave
    pop %rbx
    ret

    .data
pfmt:   .string "%ld\n"
cellfmt: .string "%ld "
eol:    .string "\n"
