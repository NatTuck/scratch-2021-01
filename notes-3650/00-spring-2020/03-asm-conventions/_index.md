+++
title = "./03-asm-conventions/notes.md"
+++

## First: HW Questions

 - Homework questions?

## Course Resources

 - In case you missed it: http://khoury.neu.edu/~ntuck/

## Compare

 * compare.S
 * Explain prototype for "main".
 * Draw the array of strings memory layout.
 * Explain strings; null termination.

## Review Recursive Factorial

 - Introduce the stack
 - Draw the stack, with %rsp and %rbp
 - Walk through fact(2), showing the stack contents.

## How to write an ASM program

Getting started with writing assembly programs can be tricky. The language
forces you to deal with individual instructions, which can make it easy to lose
track of the bigger picture.

## Design Recipe for Assembly Programs

 1. An assembly (or C) program is a bunch of functions.
 2. Figure out at least some of the functions you need.
   - Hint: You need a "main" function. You can start there.
 3. Apply the recipe for functions below to write them.
 4. As you discover new functions you need, write those too.

## Design Recipe for An Assembly Function

### 1. Signature

This is a good place to start in any language. You need to know what arguments
the function will take and what it returns.

### 2. Pseudocode

Thinking in assembly to figure out the overall strategy for a function doesn't
help solve the problem. It's better to figure out what you're doing at a higher
level before thinking about CPU instructions. Something like C makes a good
pseudocode to design an assembly function.

### 3. Variable Mappings

Your pseudocode has named variables, but your assembly code won't. For each
variable, decide where that data lives. It could be in a register (which one?),
on the stack (where?), or maybe it's a global variable (what label?).

You should explicitly write down this mapping as a comment in your assembly code.

Which registers?

 * There are two pure temporary registers: %r10 and %r11.
   * Temporary registers go bad when you call a function.
 * There are five available callee-saved registers: %rbx, %r12-%r15
   * These are safe across function calls, but if you use them in
     your function you need to save them in your prologue and restore
     them in your epilogue before returning.
 * The first six function arguments go in: %rdi, %rsi, %rdx, %rcx, %r8, %r9
   * These are temporary registers and can be re-used as such.
   * Remember that %rdx is sometimes written to by instructions (e.g. idiv)
 * The value returned by a function goes in %rax.
   * This is also a temporary, but some instructions (e.g. idiv) write to it.

### 4. Skeleton

```
label:
  /* Prologue: */
  /* save callee-save registers */
  enter $??, $0
  /* end of prologue */
 
  /* TODO: function body */

  /* Epilogue: */
  leave
  /* restore (pop) callee-save registers */
  /* pops must be in reverse order from pushes */
  /* end of epilogue */
  ret
```

 * Every function starts with a label and ends with a "ret" instruction.
 * Every function has a prologue and an epilogue to manage the function's
   use of the stack.
 * The prologue saves any callee-save registers to the stack and allocates
   a stack frame for use by the function.
 * The epilogue restores callee-save registers and "frees" the stack frame.
 * Your variable mapping tells you which registers to save: any callee-save
   registers that you're using for anything.
 * Stack alignment: To call further functions, your stack pointer must be
   on a 16-byte boundary.
   - (Stack frame size / 8) + (# of callee-save pushes should be even)
   - If you think you have an exception, explain in a comment.

What do the enter and leave instructions do?

```
  /* enter allocates a stack frame */
  /* the enter $X, $0 instruction acts like: */
  push %ebp
  mov %esp, %ebp
  sub $X, %esp
  /* waste 8 clock cycles */


  /* leave deallocates a stack frame */
  /* the leave instruction acts like: */
  mov %ebp, %esp
  pop %ebp
```

### 5. Write the body of the functions

In this step, you translate your pseudocode to assembly.


## Translating C to ASM

 * C can translate to ASM nearly 1:1.
 * Every C statement can be used to fill in a corresponding
   ASM "template".
 * The resulting ASM will perform the same computation.
 * There are multiple possible templates - these are only
   examples.

## Variables, Temporaries, and Assignment

 * Each C (int, pointer) variable maps to either a register
   or a stack location.
 * Temporary values map to a temporary register.
 * Registers can be shared / reused if you run out.

Example:

Pseudocode:
```
  int a = 5;
  int b = 3 * a + 1; 
```

Mapping variables:

 * a is -8(%rbp)
 * b is -16(%rpb)
 * Our temporary for (3\*a) and (3\*a+1) is %r11

Assembly:
```
# int a = 5;
  mov $5, -8(%rbp)

# int b = 3 * a + 1;
  mov -8(%rbp), %r11
  imulq $3, %r11
  inc %r11
  mov %r11, -16(%rbp)
```

## Which Registers

 * There are two pure temporary registers: %r10 and %r11.
   * Temporary registers go bad when you call a function.
 * There are five available callee-saved registers: %rbx, %r12-%r15
   * These are safe across function calls, but if you use them in
     your function you need to save them in your prologue and restore
     them in your epilogue before returning.
 * The first six function arguments go in: %rdi, %rsi, %rdx, %rcx, %r8, %r9
   * These are temporary registers and can be re-used as such.
   * Keep in mind that some instructions (e.g. idiv) write to %rdx.
 * The value returned by a function goes in %rax.
   * This is also a temporary, but some instructions (e.g. idiv) write to it.

## Data Directives

```
.global main
.text
main:
  enter $0, $0
  
  mov $longfmt, %rdi
  mov $foo, %rsi
  mov $0, %al
  call printf

  mov $longfmt, %rdi
  mov $bar, %rsi
  mov $0, %al
  call printf
  
  mov $longfmt, %rdi
  mov $baz, %rsi
  mov $0, %al
  call printf
  
  leave
  ret

.data
longfmt:
  .string "%ld\n"
# .align 16
foo: 
  .space 24, 0
bar: 
  .int 10
baz:
  .int 20
  .int 30
  .int 40
```

# More Examples

 * Use the recipe to build a "too low / too high" guessing game.
   * main calls play_game in a loop.
   * play\_game calls get\_guess and check\_guess in a loop

