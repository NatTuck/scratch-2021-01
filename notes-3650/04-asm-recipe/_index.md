+++
title = "./04-asm-recipe/notes.md"
+++

## Rant: Efficient Workflows

 - Pull up cookie clicker.
 - Point out that the efficient strategy is to get upgrades ASAP,
   whether they're linear or mutiplicative.

Example upgrades for programming:

 - Touch typing.
   - Save some time typing.
   - Save getting distracted to find a key.
 - Getting an editor with automatic indentation and syntax highlighting.
   - Syntax highlighting automatically catches many syntax errors, saving
     a whole try-to-compile and then debug cycle each time.
 - Getting a keyboard-focused editor and learning the keybindings.
   - In vim, swapping two lines is 3 keystrokes.
 - Becoming familiar with the libraries you're using.
   - Especially the standard library for your language.
   - How do you convert a string to a long in C?
   - Autocompletion doesn't replace this.
 - Learn your environment's debugger in some detail. For us, that's GDB.
 - You'll see more upgrade opportunities. I suggest prioritizing them, even
   if it costs some time right now, even if you're not sure it's an upgrade.

## Discuss the Homework

 - Pull it up on bottlenose
   - Write task in C
   - Then rewrite it in ASM
   - No compiler output
 - Pull up solution directory
 - Show calc running.

## First: HW Questions

 - Homework questions?
 
## Today: Assembly Recipie Examples


### Example 1: Collatz Conjecture

 - Start with an integer > 1.
 - If even, divide by two.
 - If odd, take 3*x+1.
 - Iterate repeatedly

The conjecture:

 - All integers > 1 eventually get you to one.

Our program will take an input on the command line and
print the sequence to one and the number of iterations. 


```
long
iterate(long x)
{
  if (x % 2 == 0) {
    return x/2;
  }
  else {
    return x*3 + 1;
  }
}

int
main(int argc, char* argv[])
{
  long x = atol(argv[1]);
  long i = 0; 
  
  while (x > 1) {
    printf("%ld\n", x);
    x = iterate(x);
    i++;
  }
  
  printf("i = %ld\n"):
  return 0;
}
```

Now, translate to ASM with the recipe. 

Note patterns for "if" and "while" statements.

### Example 2: Fives

 - Key point here: Dealing with individual characters in strings.

### Bonus Examples:

 - Sum Array
 - Linked List

