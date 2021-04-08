.text

  /*
typedef struct vec {
    long* data;
    long size;
    long cap;
} vec;
  */

//vec* make_vec();
.global make_vec
make_vec:
  // first use %rbp instead of rbp
  push %rbp // ys
  enter $8, $0
  mov $24, %rdi
  call malloc
  mov %rax, %rbp
  mov $16, %rdi
  call malloc
  mov %rax, 0(%rbp) // ys->data = malloc(2 slots);
  movq $0, 8(%rbp) // ys->size = 0;
  movq $2, 16(%rbp) // ys->cap = 2;
  mov %rbp, %rax
  leave
  pop %rbp
  ret

//void free_vec(vec* xs);
.global free_vec
free_vec:
  enter $16, $0
  mov %rdi, 0(%rsp) // xs
  mov 0(%rdi), %rdi
  call free
  mov 0(%rsp), %rdi
  call free
  leave
  ret

//void push(vec* xs, long xx);
.global push
push:
  push %r12 // xs
  push %r13 // xx
  enter $0, $0
  // %r10, %r11 as temporaries

  mov %rdi, %r12
  mov %rsi, %r13

  mov 8(%r12), %r10 // size
  cmp %r10, 16(%r12) // cap > size?
  jg push_no_resize

  add %r10, %r10 // 2 * size
  mov %r10, 16(%r12) // cap = %r10
  mov $0, %rsi
  lea (%rsi,%r10,8), %rsi // 8 * %r10
  mov 0(%r12), %rdi // start with "mov %r12, %rdi"
before_realloc:
  call realloc
  mov %rax, 0(%r12)

push_no_resize:
  mov 0(%r12), %r10 // data
  mov 8(%r12), %r11 // size
  mov %r13, (%r10,%r11,8) // xs->data[xs->size] = xx
  incq 8(%r12) // size++

push_done:
  leave
  pop %r13
  pop %r12
  ret

//vec* reverse(vec* xs);
.global reverse
reverse:
  push %r12 // xs
  push %r13 // data
  push %r14 // size
  push %r15 // ys
  push %rbx // ii
  // %r10 is size - ii - 1
  enter $8, $0
  mov %rdi, %r12
  mov 0(%r12), %r13
  mov 8(%r12), %r14

  call make_vec
  mov %rax, %r15

  mov $0, %rbx
reverse_loop_cond:
  cmp %r14, %rbx // ii < size
  jge reverse_loop_done

  mov %r14, %r10
  sub %rbx, %r10
  dec %r10

  mov %r15, %rdi
  mov (%r13,%r10,8), %rsi
  call push

  inc %rbx
  jmp reverse_loop_cond

reverse_loop_done:
  mov %r15, %rax
  leave
  pop %rbx
  pop %r15
  pop %r14
  pop %r13
  pop %r12
  ret
