+++
title = "./21-wasm/notes.md"
+++

 - Project Questions?

## History of Native Code in Browser

Until 1995, web pages were static things. They didn't run code.

That changed when Java Applets were introduced in May 1995. The initial release
of the Java programming language included a browser plugin that allowed Java
code to be downloaded and run in a user's browser, sandboxed for security.

A few months later, in September, Netscape Navgiator 2.0 released a native
browser scripting language called LiveScript. Because Java was hyped as the
language of the web, LiveScript was renamed to JavaScript by December.

For the next 20 years, these were the two widely supported methods to run code
in a browser: plugins and scripts. Java applets died to be replaced by another
plugin: Flash. Other scripting languages like VBScript got no traction.

Around 2010, two separate teams had the same idea for a third plan. They noticed
that users were spending more time in browsers but that most exisisting
applications, game engines, libraries, programming languages, etc were written
to target native code platforms like desktop apps.

The new plan: Run native code in the browser, by making the browser a new target
platform for compilers and such.

### NACL

The first team, at Google, released NACL in 2011. 
 
 - Subset of 32-bit x86 code called NaCL that
 - Had a standard library that was basically Linux.
 - Could be easily sandboxed by static analysis.
 - Disadvantage: Not portable to non-x86 processors

### asm.js

The second team, at Mozilla, relased ASM.js in 2013.

 - A subset of JavaScript.
 - Made a reasonable compiler target for native code languages.
 - Could be JIT compiled to efficient machine code easily.
 - Disadvantage: Weird

## QuakeJS

http://www.quakejs.com/

 - This is Quake 3, a top PC game from 1999, later open sourced.
 - It's written in C.
 - It's running in the browser, at basically full performance.
 - The trick: ASM.js
 - We saw better graphics yesterday in pure JS + fancy shaders running on the
   GPU, but this is basically just the Linux version of Quake recompiled.
 - Writing in C over JS means a small speedup, no GC pauses, etc.

## Web Assembly

 - A binary code format that can run in web browsers.
 - Kind of like Java bytecode, but very efficiently compiles to
   machine code.
 - Provides an assembly language (hence the name), which is already
   supported by LLVM as a target for C and C++ code.

Using techniques demonstrated by Mozilla's ASM.js and Google's NaCL, native code
can be run directly in the browser with a relatively small (20% maybe)
performance penalty - at least in theory.

Another way to look at this is that it lets you write code that runs in the
browser and is (up to) 2-5x faster than JS.

Quake demonstrated two of the key ideas:

 - Graphically / computationally intensive programs.
 - The ability to recompile exisiting C/C++ programs with the browser
   as the target platform.

Two more things to consider:

 - Web app frontends don't need to be in JavaScript.
 - Web app frontends don't need to be in HTML/CSS either.

## Performance

https://takahirox.github.io/WebAssembly-benchmark/

In practice, small tests in JavaScript tend to be pretty fast, so it can be hard
to see the performance benefit.

In order to get a significant benefit, you need complex data structures where
data layout is important.

## Fib Example

(cd ~/Code/wasm/wasmception)

Prereqs:
 
  - wasm starter: https://github.com/yurydelendik/wasmception
  - wasm tools: https://github.com/WebAssembly/wabt
  - Note: LLVM takes like an hour to compile.

Start in the wasmception directory.

```
$ cp -r example fib
$ cd fib
```

Take a look at index.html

 - WASM code is downloaded by JS
 - We run it through a WASM -> native compiler
 - We give it a memory space (in 64k WASM pages)
 - We can pass references to JS functions to our WASM code.
 - We can then call WASM functions from JS code.

Take a look at main.c

 - Forward declare functions getting passed from JS.
 - Then we can declare functions to export.
 - Note the way string is getting passed to JS as
   int-from-pointer and length. This same stratgy can be
   used to transfer any serialized data.

Back to index.html

 - Note use of "decoder" in implementation of ```__console_log```.

Let's run the starter code:

```
$ python3 -m http.server
```

Now let's create a fib form (in index.html):

```
<body>
  <h1>WASM Fib</h1>

  <div>
    <input id="x" type="text">
    <p id="y">None</p>
    <button id="btn">Fib!</button>
  </div>

  <script type="text/javascript">
   function btn_click() {
     let x = parseInt(document.getElementById('x').value);
     let t0 = performance.now();
     let y = window.fib(x);
     console.log(performance.now() - t0);
     document.getElementById('y').innerText = y;
   }
   document.getElementById('btn').addEventListener('click', btn_click);
  </script>

  ...
  
      WebAssembly.instantiate(module, imports).then(function(instance) {
      //console.log(instance.exports.do_something(2));
      window.fib = function(x) {
        return instance.exports.fib(x);
      }
    });
```

And a fib function in the C file:

```
int 
fib(int x)
{
  if (x <= 2) {
    return 1;
  }
  else {
    return fib(x-1) + fib(x-2);
  }
}
```

Update the Makefile to export "fib".

```
		-Wl,--export=fib \
```

Run that, then add a JS fib and compare:

```
   function fib2(x) {
     if (x <= 2) {
       return 1;
     }
     else {
       return fib2(x-1) + fib2(x-2);
     }
   }
```

Next, let's look at the 'wat' file.

```
$ wasm2wat main.wasm -o main.wat
$ view main.wat
```

## WASM with Rust

Let's create a new Rust WASM project.

Prereqs:

 - Rustup installed, latest rust.
 - https://www.rust-lang.org/
 - Setup steps here:
 - https://rustwasm.github.io/book/introduction.html
 - Might require updated node/npm with nvm.

This app is going to use Stooge Sort to sort a list of numbers.

```
$ cargo generate --git https://github.com/rustwasm/wasm-pack-template
Project Name: stooge-sort-wasm
```

Look at src/lib.rs:

 - Need to explicitly declare JS functions we can call.
 - Need to explicitly mark functions for export to JS.

Let's implement Stooge Sort. It works like this:

 - Take an array.
 - If the start and end elements of the array are in the wrong order, swap them.
 - If there are more than two elements in the array:
   * Sort the first two thirds, recursively.
   * Sort the last two thirds, recursively.
   * Sort the first two thirds, recursively.
 - Note: All "two thirds" calculations round *up*.

So here's the Rust function:

```
#[wasm_bindgen]
pub fn stoogesort(xs: &mut [f64]) {
    let nn = xs.len();
    let jj = nn - 1;

    if jj > 0 && xs[0] > xs[jj] {
        xs.swap(0, jj);
    }

    if jj < 2 {
        return;
    }

    let mut kk = (2 * nn) / 3;
    if (2 * nn) % 3 != 0 {
        kk += 1;
    }

    stoogesort(&mut xs[0..kk]);
    stoogesort(&mut xs[(nn - kk)..nn]);
    stoogesort(&mut xs[0..kk]);
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_sorts() {
        let mut xs = vec![35.0,-4.1,2.5,1.0,4.0,8.0,1.0,1.0,7.0];
        let mut ys = xs.clone();

        stoogesort(&mut xs[..]);
        ys.sort_by(|aa, bb| aa.partial_cmp(bb).unwrap());

        assert_eq!(xs, ys);
    }
}
```

We can test this like any Rust library, with:

```
$ cargo test
```

Now to use it, we want to turn our Rust library into a JS module, and
publish it as a local NPM module.

```
$ wasm-pack build
$ cd pkg
$ npm link
$ cd ..
```

Now we can call this module from JS. Let's just make a stand-alone JS app:

```
$ npm init wasm-app www
$ cd www
$ npm install
$ npm link stooge-sort-wasm
```

Here's our index.js:

```
import { stoogesort } from 'stooge-sort-wasm';
let xs = new Float64Array([8,1,7,2,6,3,5,4]);
stoogesort(xs);
console.log(xs);
```

Now we can run this with:

```
npm run start
```

Let's make this fancier:


Add to index.html

```
    <input type="text" id="inp">
    <p id="outp"></p>
```

And new content for index.js

```
import { stoogesort } from 'stooge-sort-wasm';

function inp_change(_ev) {
  let text = document.getElementById('inp').value;
  let xs = new Float64Array(text.split(/[^\d\.]+/).map(parseFloat));
  stoogesort(xs);
  document.getElementById('outp').innerText = xs.join(' ');
}

document.getElementById('btn').addEventListener('click', inp_change);
```

# Other Web Assembly Toolchains

https://github.com/appcypher/awesome-wasm-langs

A bunch of other languages (C#, Java, Lua, Perl, Python) have projects that
provide at least some support for web assembly. It looks like C (and C++ with
the usual complications) and Rust still have the best toolchain support.


