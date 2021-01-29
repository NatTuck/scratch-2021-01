

# Labels in assembly

We use labels for (at least) three things:

 * Functions (destinations for "call")
 * Control flow within functions (destinations for
   jmp / conditional jumps)
 * Names for global data
   * Like string literals
   * Global variables

# How to write Assembly Programs

Getting started with Assembly programs is
tricky.

## Design Recipe for Assembly Programs

 1. An assembly (or C) program is a bunch of functions.
 2. Figure out at least some of the functions that
    you're going to need.
    - Hint: You need "main". You can start there.
 3. Apply the recipie for a function below to write
    each function.
 4. As you discover you need more functions, add those
    to the list.

## Design Recipe for an Assembly Function

### 1. Signature

Figure out arguments, their types, return value, and
its type.

This can be written down as a C prototype.

### 2. Pseudocode

Thinking in assembly is too much work, at least until
you've spent way too much time with it.

Working C code makes good pseudocode, but working isn't
required here. Vague python-looking code is mostly good,
as long as it handles pointers like C.

### 3. Variable Mappings

Assembly doesn't let us name our variables, but our
pseudocode *does* have variable names.

Figure out, for each variable or temporary value,
where we're storing it. Write down a variable mapping
comment in our assembly saying what goes where.

Where can we store values in an assembly program?

 - Registers: Safe or Temporary? Argument reg? Return register?
 - Stack
 - Elsewhere in memory (e.g. string literals, global vars)

### 4. Skeleton

```asm
name:
    // Prologue
    // Push safe registers here
    enter $??, $0
  
    // Body
    // Code that does stuff goes here.
    // TODO
    
    // Epilogue
    leave
    // pop safe registers here, in reverse order
    ret
```

### 5. Write the Code

Translate the pseudocode to assembly instructions.


## Translating C to ASM

 * C translates to assembly nearly 1:1.
 * Every C statement can be used to fill in a corresponding
   assemlby "template".
 * The resulting ASM performs the same computation as the C.
 * There are multiple possible templates for each statement,
   so the examples I give are not definitive.

### Variables, Temporaries, and Assignments

 * Each C local variable (long, pointer) maps to a register
   or a stack location.
 * Temporary values can usually map to a temporary register.
 * Registers can be shared/reused if you run out. 

Example:

```C
  int a = 5;
  int b = 3 * a + 1;
```

Variable mappings:

 * a is 0(%rsp)
 * b is 16(%rsp)
 * Temporary for both (3\*a) and (3\*a+1) is %rax


```asm
// int a = 5;
    mov $5, 0(%rsp)
    
// int b = 3 * a + 1;
    mov $3, %rax
    imulq 0(%rsp)
    add $1, %rax
    mov %rax, 16(%rsp)
```

* There are patterns for other statements as well.


## Which registers?

 * Temporary registers
   * Is it an argument? Can it stay there?
   * Is this the return value? Should this go in %rax?
   * %r10 and %r11 have no conflicts
 * Safe registers if needed
 * If you run out of registers, it's stack allocation
   time.

## Example: Collatz Conjecture

 * Start with an integer > 1
 * If even, divide by 2.
 * If odd, take 3*x+1.
 * Iterate repeatedly 

Conjecture:

 * All integers > 1 eventually get you to 1.

Our program will take an input on the command line
and print the sequence of numbers and number of iterations
to get to 1.





  
  





