
# CS4550 Lecture 3

Today: Mostly JavaScript

## JavaScript 

 - Dynamically typed
 - Hash-oriented: Primary data type is called an
   "object", but it's basically a String => X
   HashMap.
 - Prototype-based object model
   - Object Oriented, but no classes
   - Instead, you create a new object based on
     a "prototype" object. One object inherits
     from another object.
 - Provides decent support for programming in
   a functional style.

# Stuff that you should know about that matters in JS

 - Lexical Scope - Variables live for the duration
   of the containing block ("lexical" means "text",
   in this case the text is curly braces)
 - Lexical capture
 
```js
function make_counter(aa) {
    return function() {
        aa += 1;
        return aa;
    }
}
```

 - Anonymous functions (was "lambda" in Fundies)
 - Delayed execution

```js
function make_counter(aa) {
    function inc() {
        aa += 1;
        return aa;
    }

    function reset() {
        aa = 0;
    }

    return [inc, reset];
}
```

Things we have in JavaScript:

 - Floating point #'s
 - Strings
 - Objects
 - Arrays - these are really just objects that are broken
 - Function

2D Arrays:

 - Fixed size: Flatten to 1D
 - Dynamic size: Array of Array
 - Whatever works?

## Functional Programming

This is a programming style that programmers chose
to use, potentially with language support, just like
"Object Oriented Programming".

 - Immutable data
   - Once you've constructed an object, it never changes.

```
function ex1() {
    let xs = [1,2,3,4]; // should be treated as immutable
    foo(xs); // should be pure function
    console.log(xs); // result should be [1,2,3,4]
}
```

 - Pure functions
   - A pure function has no side effects
   - A side effect is something that sticks around after
     you've called the function: e.g. I/O, mutation of 
     shared state.
   - Pure functions can't do anything except return
     a value based on its arguments and immutable 
     shared information.

The rendering library we use, React, requires immutable
data and pure functions in various places.

## OO Programming in JS

Core language design concept: Prototypes

```js
(function() {
    function Posn(x, y) {
        this.x = x;
        this.y = y;
    }

    function x2(posn, ...) {
        
    }

    Posn.prototype.dist_from_origin = function() {
        let x2 = this.x * this.x;
        let y2 = this.y * this.y;
        return Math.sqrt(x2 + y2);
    }

    window.Posn = Posn;
})()

let aa = new Posn(0, 10);
aa.dist_from_origin();
```

## Alternative: Object.create

```js
let default_posn = {
    x: 0,
    y: 0,
    dist: function() {
        let x2 = this.x * this.x;
        let y2 = this.y * this.y;
        return Math.sqrt(x2 + y2);
    }
};

let aa = Object.create(default_posn, {x: 5, y: 7});
```

```js
// Post 2015 JavaScript
class Posn {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }

    dist_from_origin() {
        let x2 = this.x * this.x;
        let y2 = this.y * this.y;
        return Math.sqrt(x2 + y2);
    }
}
```

## Document Object Model (the DOM)

This is the browser API for accessing HTML
documents from JavaScript.

 - HTML documents are a tree, so this API operates
   on that tree.
 - Basic thing is object representing an HTML element.
 - Each tag in the HTML document produces an element.
 - There additional things that are tree nodes, like 
   text.
 - Each object has properties we can inspect or change
   to update the tree.
 - When tree updates, page rerenders.

## HW02 and the Future

HW02 says you should write all your code yourself.

In the future, we'll be pulling in libraries to
make things easier / better.

Examples:

 - Some sort of CSS framework like Bootstrap or Milligram.
 - lodash: Functional-style common functions like map, 
   filter, reduce; other utility functions
 - React: A virtual DOM rendering library. This lets us
   avoid manually manipulating the DOM when dealing with
   frequently changing page contents.


