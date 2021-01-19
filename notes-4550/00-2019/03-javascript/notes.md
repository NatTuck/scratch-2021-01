+++
title = "./03-javascript/notes.md"
+++

Thursday:  	EL 312 	11:45am - 1:25pm

Friday:     WVG 102	3:25pm - 5:05pm

## First: HW Questions?

 - Today we'll talk about JavaScript and the DOM.

## Quick Review of HTML + CSS

(show page/page.html)

 - Today we're talking about how to write code that will run in a web page.
 - Web pages are HTML documents.
 - HTML documents are a tree of tags / elements.
 - Different tags mean different stuff.
 - Every element has a bunch of associated data.
   - Tags have attributes, specified in the HTML.
   - Tags have style data, modified using CSS rules.
 - Browsers render web pages by building the tree in memory.
 - JavaScript can inspect and manipulate the tree.
 - If the tree is changed, the browser re-renders the page. 

## JavaScript: The Language

JavaScript is:

 - A language initially designed in two weeks by Brendan Eich at Netscape in 1995.
 - About as different from Java as possible while still having curly braces for blocks.
 - Dynamically typed.
 - Hash-oriented. The primary data type is a thing called an object, which is a key-value
   map with string keys.
 - Has a prototype based object model - this makes it "object oriented" without classes.
 - Provides decent support for programming in a functional style.

Examples:

 - var thing = {a: 5, b: 7};
 - thing.b
 - thing["b"]
 - We can type examples in "node" in a terminal.
 - Does type coersions:
   - 5 == "5"
   - [] == ![]
   - 5 === "5"
 - Happens in cases other than equality:
   - 1 < 2 < 3
   - 3 > 2 > 1
   - Be careful, try to get an understanding of what the rules actually are, especially
     if you're going to write a lot of JS.

Object prototypes:

```
function Posn(x, y) {
  this.x = x;
  this.y = y;
  this.dist_from_origin = function() {
    return Math.sqrt(this.x * this.x + this.y * this.y);
  };
}

var aa = new Point(0, 10);
aa.dist_from_origin();
```

Problems:

 - No mechanism for inheritence (maybe a good thing?)
 - Really easy to forget "new" and just call the function. This makes a mess.

A note on "this":

 * The variable "this" refers to the object that the current method was called on.
 * If you call something as a function rather than a method, it's best to assume
   that "this" is assigned randomly.
 * ES 2015 adds arrow functions: (a, b) => {return a + b}, which lexically capture
   the outer "this" binding. Not supported in IE 11.

### Alternative: Object.create

```
let default_posn = {
  x: 0,
  y: 0,
  this.dist = function() {
    return Math.sqrt(this.x * this.x + this.y * this.y);
  };
};

let posn1 = Object.create(default_posn, {x: 5, y: 7});
```

### Alternative: class

 * Introduced in ECMAScript 2015
 * If you try to call a class as a function, you get an error.
 * Not supported in IE 11 (without Babel)

## DOM: The Web Page API

 - (Ctrl + =) to zoom firefox dev console.
 - HTML documents are a tree.
 - This tree is exposed as data from JavaScript.
 - The browser either exposes or simulates exposing the data objects
   representing the nodes in the tree.
 - More specifically: 
   - Each tag produces an element, which is a kind of tree node.
   - There can be tree nodes that are not elements (like text).
 - Each one of these objects has both properties, which we can inspect
   or modify, and methods that we can call.
 - The exposed tree root is called "document".
 - The children of a node are in an object property called "children". 
   This looks like an array, but isn't one. We can treat it as a read-only array.
 - Non-root nodes have a "parentElement" property.
 - There are also "nextSibling" and "previousSibling" properties.
 - To mutate the children array, we use methods:
   
```
var body = document.children[0].children[1];
var new_h1 = document.createElement("h1");
new_h1.innerText = "New Heading";
var ch0 = body.children[0];
body.insertBefore(new_h1, ch0);
body.children[0].remove();
body.insertBefore(new_h1, ch0);
new_h1.style.color = "purple";
new_h1.style;
```

 - There are a bunch of properties and methods available on HTML elements.
 - Some kinds of element have extra properties / methods.

```
// Align is only available on block elements that can contain text.
new_h1.align = "center";
```

 - There are better ways to get a reference to specific elements than by
   traversing the document tree:

```
var para = document.getElementById("hello");
para.innerText = "Here's the 'hello' element.";

var bdivs = document.getElementsByClassName("bbb");
for (var ii = 0; ii < bdivs.length; ++ii) {
  var div = bdivs[ii];
  div.innerText = "bbb div #" + ii;
}

// Array like objects are weird.
// In modern browsers, we can just copy them to am array.
var bda = Array.from(bdivs);
bda.forEach(function (div) {
  div.innerText = "this is a div";
});

var bdivs2 = document.querySelectorAll(".bbb"); // CSS-style selector
var bdiv = document.querySelector(".bbb"); // Gets first matching element
var para2 = document.querySelector("#hello"); // Gets first matching element
```

## Namespaces and Strict Mode

 - Traditional JavaScript doesn't have a module system.
 - That means any names that are declared at the top level of your code - which is especially
   common for fuctions - are global.
 - Luckily, JavaScript provides lexical scoping, which can be used to simulate modules.
 - (See code.js)
 - JavaScript also has a great feature: assignment to an unknown variable name declares a
   new global variable. 
 - This can be disabled with the "use strict" mechanism, which requires that all variables
   be explicitly declared (with var, let, or const) before use.

 - Once we start writing modern JS we won't need to worry as much about this stuff - until 
   we try to debug our code, and then we realize that this is what the transpiler does.


## JS Events

```
<script>
  //alert("Scripts execute in order, blocking page rendering.");
</script>

<p><button onclick="alert('clicked button');">Click me</button>

<p><button id="button2">Button 2</button>
<script>
  function showPopup() {
    alert("clicked button 2");
  }
  
  var btn = document.getElementById("button2");
  btn.onclick = showPopup;
</script>

<p><a href="http://google.com/" id="link1">Click Me</a>
<script>
  function showPopup2(ev) {
    //ev.preventDefault();
    alert("clicked button 2");
  }
  var link1 = document.getElementById("link1");
  link1.onclick = showPopup2;
</script>

<p><button id="button3">Button 3</button>
<p id="b3text">Button 3 text</p>
<script>
  var b3p = document.getElementById("b3text");
  function changeTextBack() {
    b3p.innerText = "Button 3 text"; 
  }

  function changeText() {
    b3p.innerText = "some other text";
    window.setTimeout(changeTextBack, 2000);
  }

  var btn3 = document.getElementById("button3");
  btn3.addEventListener("click", changeText);
</script>
```

## Looking Forward

After we get going, we're going to pull in some libraries to make things more
complicated, and possibly easier:

 - jQuery: Provides some shortcuts for DOM stuff like queries and manipulation
   of elements.
 - Some sort of CSS design framework like Bootstrap or Milligram, that lets us
   work from a bunch of standard elements with reasonable styling.
 - lodash: Some extra functions that allow a functional programming style.
 - React: A virtual DOM rendering library. This lets us avoid manually
   manipulating the document tree when dealing with frequently-changing page
   contents - which is a good thing.


