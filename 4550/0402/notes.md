
# Project Questions?

...


# ... Web Assembly

## History of Code in the Browser

Web pages initially static.

In May 1995, Java Applets were introduced. This
was a browser plugin that could run sandboxed
apps.

In September 1995, Netscape Navigator 2.0 came out.
This included a scripting language called LiveScript,
which was quickly renamed to JavaScript.

For the next 20 years, these were the two
basic mechanisms for running code in browser.

At some point Java Applets effectively died, 
replaced by Flash. Some other compeditors did
a little better:

 - Silverlight
 - Java Webstart?

The way usage happened:

 - JavaScript for simple stuff.
 - Flash for stuff that needed really exact rendering
   or edge case behavior.

JavaScript eventually won, with flash being killed off
last year or so.

Around 2010, two separate teams had basically the same
idea for a third option.

Stuff both teams noticed:

 - Web browsers had taken over as the primary
   thing people did on computers.
 - But almost all existing code, including libraries,
   was native code for native apps.

The new plan: Run native code in the browser.

 - How? Make the browser a new compilation target
   for native code (i.e. C) compilers.

## NACL

The first team, at Google, released NACL in 2011.

 - Subset of 32-bit x86 code called NaCL that
 - Had a standard library that was basically
   desktop Linux
 - Could be easily sandboxed by static analysis
 - Disadvantage: Not portable to non-x86 processors

PNACL.

 - Trying to be cross platform.
 - Distribute LLVM bitcode instead of x86 machine
   code.

## asm.js

The second team, at Mozilla, released ASM.js in 2013.

 - Subset of JavaScript
 - Made a reasonable compiler target for native code
   languages.
 - Could be JIT compiled to efficient machine code.
 - Disadvantage: Weird

Example trick:

```
   let a = 25|0;  // int a = 25;
```

## Web Assembly

 - A binary code format that can run in web browsers.
 - Mechanically, similar to asm.js, except not backwards
   compatible with existing JS runtimes.
 - There's a text-based version of the format which
   acts as an assembly code - it's supported by LLVM
   as a target.

This is a mechanism to write code that runs in the
browser and:

 - Is up to 2-5x faster than JS.
 - With more control over performance tradeoffs.

Quake demonstrated two key applications:

 - Graphically / computationally intensive programs.
 - Recompiling existing C/C++ code and targeting
   the browser.

This drastically changes browser / frontend dev:

 - Web app frontends don't need to be in JavaScript.
 - Web app frontends no longer need to be HTML / CSS.
 - We could use C++ / OpenGL for our app if we want.

```
struct Foo {
  int field1;
  float field2;
}

vector<Foo>
```

To run webassembly you need to download, compile,
initialize, and run the module from JavaScript code.

