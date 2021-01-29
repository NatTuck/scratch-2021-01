

# Labels in Assembly

We use labels for three different things:

 * Function (we get to this with "call", it follows the
   skeleton with enter/leave, returns with ret).
 * Control flow within a function (targets for
   jmp / conditional jumps like je, jle, jg, etc)
 * To name global data
   - Stuff like string literals (longfmt: .string "%ld")
   - Global variables

# How to Write an Assembly Program

 - Writing computer programs is hard
 - It's hard to know what to type into
   an empty editor window.
 - This is especially bad in assembly, because
   assembly wants to know what machine instructions
   to run

## Design Recipe for Assembly Programs

 1. An assembly (or C) program is a bunch of functions.
 2. Figure out at least some of the functions we need
    - Hint: All programs need "main", so when in doubt
      start there.
 3. Apply the recipe for a function below to each
    function you need until you've written the program.
 4. If you discover new functions you need, add them
    to the list.

## Design Recipe for an Assembly Function

### 1. Signature

It's useful to know what arguments a function takes,
what their types are, and what type it returns.

This can just be the function declaration from C.

### 2. Pseudocode

Thinking in assembly is annoying and not hugely 
useful. We're not used to designing functions one
instruction at a time.

Instead, it's easier to figure out what the function
does and how first, then translate to assembly later.

Best pseudocode: Working C. 

Second best: Almost working C.

### 3. Variable Mappings

Figure out where you're going to store each variable
and value in the function. This should go in a
variable mapping comment in your assembly code.

Places values can go:

 - Registers: Safe? Temporary?
 - Memory:
   - The stack
   - Elsewhere in memory (not local variables)

### 4. Skeleton

Every function follows the same basic structure:

```asm
name:
    // Prologue
    // push safe registers
    enter $??, $0    // make sure stack ptr is aligned

    // Body
    // TODO: write body
    
    // Epilogue
    leave
    // pop safe registers in reverse order
    ret
```

### 5. Write the Code

Translate the body pseudocode into assembly.


## Translating C into Assembly

 * C can be translated into assembly basically 1:1.
   - That's how C works - the compiler conceputally translates
     C to assembly.
 * Every C statement can be used to fill a corresponding
   ASM template.
 * There are multiple possible templates for each thing.

### Example: Variables, Temporaries, Assignment

Our C code (from the body of some function):

```
  long a = 5;
  long b = 3 * a + 1;
```

Variable mappings:

 * a is 16(%rsp)
 * b is 0(%rsp)
 * (3\*a) is %rax

Translated to ASM:

```
  // long a = 5;
    movq $5, 16(%rsp)
  
  // long b = 3 * a + 1;
    mov $3, %rax
    imulq 16(%rsp)
    add $1, %rax
    mov %rax, 0(%rsp)
```

## Where to put variables

 - Temporary registers
   - Is this the return value? Can it go in %rax?
   - Is this an argument? Can we leave it where it is?
   - %r10, %r11 have no conflicts
 - Safe registers
   - Does this variable need to survive a function call?
 - Stack
   - Do you need to take the address of the variable?
   - Are you out of registers?

You can reuse registers if needed, just be clear about
what you're doing in your variable mapping.

# Example: Collatz Conjecture

 - Start with an integer > 1
 - If even, divide by two
 - If odd, take 3*x+1
 - Iterate repeatedly 
 
The conjecture:

 - Starting from any integer > 1, 
   you eventually get to 1.

Our program will take an input on the command line
and print the sequence to get to one and the number
of iterations it took.












