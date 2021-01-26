---
layout: default
---

## Functional Programming: Key Idea (at least for us)

 - Is a programming style, like procedural, OO, or logic programming.
 - Some languages expect this style, others support it, others make it hard.
   - Like how OO is expected in Java and well supported but optional in
     Python.
 - Languages expecting functional code: Haskell, OCaml, Clojure, Elixir.
 - Languages supporting functional code: Most modern languages.
 - Languages that make it hard: Java, C, FORTRAN
 - 10 years ago functional programming was esoteric, now it's something that
   any practicing developer will run into and should be comfortable with.

Constraints in programming;

 - When you're writing assembly code, or C, you can conceptually do anything.
   You can get a pointer to any memory location and write there, you can "goto"
   memory addresses and start executing code, etc.
 - Sometimes those things are useful, but in most code we want to have some
   constraints to make reasoning about things easier.
 - In most languages there are some built-in constraints:
   - Only a function can access its own local variables.
   - You can't jump to code outside of the current function except by calling
     another function or returning.

```
function foo() {
  let xs = [1,2,3,4];
  bar();
  // xs is still [1,2,3,4] - it's a local variable, so bar
  // can't mess with it.
  
  // In assemby this wouldn't be true.
  // Also not true in Emacs LISP.
```

 - Languages and programming styles add constraints to make specific patterns
   easier while avoiding specific kinds of bugs.
 - Consider "private" in Java:
   - We can make fields, methods, and classes only accessible from "local" code.
   - This prevents non-local code from doing things we don't expect.
   - With this, we can guarantee invariants about the state of objects while
     only needing to reason about the local code.
   - One key problem this solves is unexpected mutation - if external code could
     access and modify private fields, we could no longer tell of our invariants
     held across the whole program just by looking at the current class.

Constraints in functional programming:

 - The functional programming style is all about a constraint on one thing: side
   effects.
   - A function has a side effect if it does anything that can be externally
     observed other than returning a value.
     - ex: Mutating a global variable
     - ex: Printing to the screen
     - ex: Sending an HTTP request.
     - ex: document.getElementById('foo').text = "hello"
   - A function with no side effects that returns a value based entirely on the
     values of its parameters is called a "pure" function.
     - Pure functions don't access global / external state.
     - Not accessing global state isn't a functionality constraint - that state
       can simply be passed as an argument.
 - It's impossible to avoid side effects. Programs with no side effects don't do
   anything.
 - Pure functions are extremely easy to reason about because they don't do anything.
 - But, side effects can be isolated.
 - The purpose of this is just like "private" in Java: to allow us to reason
   about our code, especially our invariants.
   - Every pure function can be reasoned about seperately.
   - A pure function can't possibly break anything anywhere else.
   - Pure functions can be easily tested, since if you check that you get
     the right outputs for the right inputs that's the whole story.

```
// we start with a global variable
var numbers = [1,3,2,4];

// procedural style
function append_sort_and_print(int vv) {
    numbers.push(vv); // side effect: mutate global
    numbers.sort(); // side effect: mutate global
    console.log(numbers); // side effect: I/O
}

// functional style

// pure function does the computation
function append_and_sort(xs, vv) {
    let ys = xs.concat([vv]); // xs is unchanged
    ys.sort(); // side effect: mutate local variable
               // This is not externally visible, so function is still pure.
    return ys;
}

// side effects are isolated so we can keep an eye on them
function append_sort_and_print(int vv) {
    let ys = append_and_sort(numbers, vv);
    
    // We still need to actually do the task,
    // but we've isolated the side effects from the computation.
    numbers = ys;
    console.log(numbers);
}
```

With a functional language like Elixir, we go a step further: completely
eliminating object mutation. This prevents us from writing side effects by
mistake by mutating shared references.

## Functional Programming and React

React is a display framework - currently getting overhyped, but still a good
tool - that's based on an idea from Academia called "Functional Reactive
Programming".

The research question was: How do we render graphics while constraining side
effects? Initally, this was mostly a game engine thing.

The solution:

 - Seperate logical "state" data from the rendering logic.
 - Have a pure function that transforms the current state - and possibly the
   time stamp - into one display frame.
 - When events occur, have a pure function that transforms (state, event) =>
   state, then re-render.
 - Isolate side effects (actually collecting user input and other events,
   drawing the rendered frame) into an engine component seperate from the
   application code.

"But the DOM API is imperative, and we don't want to change it every frame like a
game engine, so this makes no sense."

 - If we really did a full re-render at 60 Hz, game engine style, it wouldn't be
   great.
 - Optimization: Only re-render when the state changes.
 - Optimization: Don't just delete and reinsert the whole DOM sub-tree,
   calculate the changes and make the minimum nessisary changes.
 - DOM changes are comparatively slow, so even regenerating the full "shadow
   DOM" tree on every change and then calculating changes is faster than a
   template solution that would require reparsing the full HTML.

More recently some other solutions are similar performance to React, but the
initial speed boost was enough to convince people to try an FRP style solution,
and FRP turns out to be pretty nice in this application.

What that means:

 - When writing a stateful React componet, your job is to write a handful of
   pure functions:
   - The render() method.
   - One event handler function per type of event (e.g. mouse click on a type of
     button, setTimeout happened, got network packet, etc).
   - To get real pure functions:
     - render() { return pureRender(deepCopy(this.state)); }
     - onEvent(ev) { this.setState(pureOnEvent(deepCopy(this.state), deepCopy(ev.data))) }
 - React relies on the state field containing an immutable value that is never
   changed, only replaced with a new value by setState in event handlers.

