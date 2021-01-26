
# CS3650 Lecture 3

Today: More Assembly

```
&aa = 0x7ffc22f376a8 (top)
&bb = 0x7ffc22f376a0 (8 bytes lower)
&yy = 0x7ffc22f37678 (40 bytes lower)

sizeof(long) = 8
```

The stack:

At program startup, the register %rsp has the 
address of the top of the stack. Our program is
allowed to write data below that address.

As our program executes, we "allocate" stack space
by moving %rsp down. That space is "deallocated" by 
moving it back up.

The stack for "dub":

(call main; assume aligned)
 * Return address from main
 * Old %rbp from before main <- base ptr after leave from dub
 * aa
 * bb <- stack ptr after leave from dub
(call dub)
 * Return address from dub
 * %rbp for main stack frame
 * yy
 * (spacer)
(now we're aligned)

The code for "dub":

```asm
// Not our code, the C runtime library.
    call main
the_line_after_that:    
```

```
    dub:
        enter $16, $0 // allocate yy, 8 byte spacer
        
        leave
        ret

    main:
        enter $16, $0 // allocate aa, bb
       
        call dub
    ret_from_dub:
       
        leave
        ret
```

# Call and Ret

The call instrucition:

 * Pushes the return address (address of the 
   next instruction) to the stack.
 * Jump to the provided (label) address

The ret instruction:

 * Pop the return address from the stack
 * Jump there

# Enter and Leave

```asm
    enter $XX, $0
// is the same as
    push %rbp
    mov %rsp, %rbp
    sub $XX, %rsp
    /* waste 8 clock cycles */

    leave
// is the same as
    mov %rbp, %rsp
    pop %rbp
```

# Stack Alignment Rule

Whenever we issue a "call" instruction, the stack
must be aligned to 16 bytes.

When we "call", %rsp must contain a multiple of 16.

In general, we try to make it so this is always
true in the body of a function.

# Register Conventions

Temporary Registers:

 - %rdi, %rsi, %rdx, %rcx, %r8, %r9
 - %rax
 - %r10, %r11

Temporary registers are assumed to have their value
corrupted by function calls or system calls.

Temporaries can be pushed to the stack around
function calls if you must save them and don't want
to allocate a safe register. This is called
"caller-save".

Safe Registers:

 - %rbx, %r12..%r15

Safe registers are not destroyed by a function or
system call.

But... if you use a safe register, you must save
the old value before use and restore it before
returning. This is called "callee-save".







