
# Systems Lecture 3: More Assembly

# dub memory layout

First run:

shared address prefix for locals: 0x7fff2c75

&aa = 8c28
&bb = 8c20

sizeof(long) = 8 = 64 bits

Second run:

&aa = 0x7ffeac24ccf8 +8
&bb = 0x7ffeac24ccf0 +24
&yy = 0x7ffeac24ccc8 +16
&xx = 0x7ffeac24ccb8 0

%rsp is the Stack pointer - the bottom of the current
        stack frame
        
%rbp is the base pointer - the top of the current
        stack frame

When our program starts, %rsp points to the top
of a region of memory called "the stack" - below
that pointer is space where our program can freely
write stuff.

Alignment rule: When we issue a call instruction,
%rsp must be aligned to 16 bytes. That means
the value in %rsp must be evenly divisible by 16.

Not our code:

```asm
// in the C runtime library
    call main
return_from_main:   // ret address from main
```

In our code:

```asm
dub:
    enter $16, $0
    // is xx a local variable? yes
    // local variable yy
    leave
    ret
    
main:
    enter $16, $0
    // two local variables: aa, bb
    call dub
    // (executing here after ret from dub)
    leave
    ret
```

The stack:

call main: (assume %rsp mult of 16)
 * ret address from main <- %rsp after leave
 * old value of %rbp
 * == start stack frame for main
 * space for aa
 * space for bb
 * == end stack frame for main
call dub:
 * ret address from dub   <- %rsp after leave
 * old value for %rbp
 * == start stack frame for dub
 * space for xx
 * space for yy   <- %rsp before leave
 * == end stack frame for dub

## Invalid code

```asm

fun1:
    // do thing
    ret

main:
    call fun1 // stack not aligned to 16 bytes
    // call main pushed ret addr
    // so stack is (multiple of 16) + 8 = unaligned
    //
    // doing enter $0, $0 aligns the stack
    // by pushing the base ptr.
    ret

```

## Call and ret

Call:
    
    - push return address
    - jump to label
    
Ret:

    - Pop return address
    - Jump there

## Enter and leave

```asm
    enter $XX, $0
// means
    push %ebp
    mov %esp, %ebp
    sub $XX, %rsp


    leave
// means
    mov %ebp, %esp
    pop %ebp
```

## Saving Registers

Temporary registers:

 - %rsi, %rdi, %rcx, %rdx, %r8, %r9
 - %rax
 - %r10, %r11

You should assume that values stored in a temporary
register are lost when you call any function or
make a system call.


Safe registers:

 - %rbx, %r12..%r15
 - %rbp, %rsp (don't use these)

Before you use a safe register, you must save that
value. Before you return, you must restore the value
of any saved safe registers.






