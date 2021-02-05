
# More Work on React App

## Add Milligram

Install a package with npm:

```
$ npm install milligram
```

Import it in our JS, in src/index.js:


## Refactor Our App

This is a large refactoring to add some styling
and use components.

Replace App.css with:

```
p { font-size: 24pt; }
```

Replace App.js with:

```text
import { useState } from 'react';
import 'milligram';

import { uniq, bad_guesses, word_view, lives_left } from './game';
import './App.css';

function GameOver({reset}) {
  return (
    <div>
      <div className="row">
        <div className="column">
          <h1>Game Over</h1>
        </div>
        <div className="column">
          <p>
            <button onClick={reset}>
              Reset
            </button>
          </p>
        </div>
      </div>
    </div>
  );
}

function Controls({makeGuess, reset}) {
  // DANGER: Think very carefully about state not in
  // the root component.
  // Strongly prefer a single centralized state.
  const [guess, setGuess] = useState("");

  function updateGuess(ev) {
    let text = ev.target.value;
    if (text.length > 1) {
      text = text[text.length - 1];
    }
    setGuess(text);
  }

  function keypress(ev) {
    if (ev.key == "Enter") {
      makeGuess(guess);
      setGuess("");
    }
  }

  return (
    <div className="row">
      <div className="column">
        <p>
          <input type="text"
                 value={guess}
                 onChange={updateGuess}
                 onKeyPress={keypress} />
        </p>
      </div>
      <div className="column">
        <p>
          <button onClick={() => { makeGuess(guess); setGuess(""); }}>
            Guess
          </button>
        </p>
      </div>
      <div className="column">
        <p>
          <button onClick={reset}>
            Reset
          </button>
        </p>
      </div>
    </div>
  );
}

function App() {
  const [secret, _setSecret] = useState("elephant");
  const [guesses, setGuesses] = useState([]);

  let view = word_view(secret, guesses);
  let bads = bad_guesses(secret, guesses);
  let lives = lives_left(secret, guesses);

  function makeGuess(guess) {
    setGuesses(uniq(guesses.concat(guess)));
  }

  function reset() {
    setGuesses([]);
  }

  let body = null;

  if (lives > 0) {
    body = (
      <div>
        <div className="row">
          <div className="column">
            <p>Lives: {lives}</p>
          </div>
          <div className="column">
            <p>Word: {view.join(' ')}</p>
          </div>
        </div>
        <div className="row">
          <div className="column">
            <p>Guesses: {guesses.join(' ')}</p>
          </div>
          <div className="column">
            <p>Bads: {bads.join(' ')}</p>
          </div>
        </div>
        <Controls makeGuess={makeGuess} reset={reset} />
      </div>
    );
  }
  else {
    body = (
      <GameOver reset={reset} />
    );
  }

  return (
    <div className="container">
      <div className="row">
        <div className="column">
          <h1>Hangman</h1>
        </div>
      </div>
      { body }
    </div>
  );
}

export default App;
```

Thoughts:

 * Components reset their state when they are "mounted". This means that if
   a component is temporarily hidden, its state will be lost.
 * By having our text input state in the Controls component, we're kind of
   simulating an uncontrolled text input. That may be fine, but it may
   also cause issues especially if we further modify our app.

## Deploying our App

Now that we have our app, a critical next step is to deploy it to our server.

To create a production build for our app, we run:

```
$ npm run build
```

This produces a static web page that we can serve with a web server (e.g. nginx)
just like the sites from previous assignments. We can test it with any old local
webserver:

```
$ (cd build && python3 -m http.server)
```

Deploy to server:

 * Log in to server as admin user: ```ssh nat@cs4550.ironbeard.com```
 * Create new user for site: ```adduser hangman1```
 * Copy over the build directory ```scp -r build hangman1@cs4550:~/www```
 * Set up nginx


## Hook: useEffect

Render functions should be pure. But sometimes displaying the content you
want really does depend on side effects.

The common example is embedding non-React rendering inside a React component,
but we can show a simpler example.


```
import { useState, useEffect } from 'react';
```

```
function SetTitle({text}) {
  useEffect(() => {
    // Set on mount.
    let orig = document.title;
    document.title = text;

    // Restore on unmount.
    return () => {
      document.title = orig;
    }
  });

  return <div />;
}
```

 * Render SetTitle from the GameOver component.
 * Demo it.
 * Note that title is set on GameOver and restored on reset.
 * Remember: React is a rendering framework. Logic in your app that isn't
   about display doesn't need to go in React components at all.

# Introducing Elixir

Why would you pick a specific language?

 - You know it.
 - It has good libraries for your domain. (e.g. FORTRAN has fast FFTs)
 - Not using it requires extra effort. (JavaScript runs in web browsers)
 - The language itself does something that you want. 
   (Rust provides a set of type system enforced safety guarantees.)
 - It has specific performance properties. (C++ is fast for video games
   because it compiles to machine code and has no garbage collector.)

Sequential vs. Concurrent Programming

 - Traditionally, computer programs run sequentially.
 - The computer systems they run on aren't sequential.
   - You can't buy a single core laptop, or even a *phone*, in 2018.
   - Web programming is distributed, which can't be sequential.
   - Web applications have multiple concurrent users.
   - Web *servers* tend to be distributed too, to allow web applications
     to scale *lots* of concurrent users.
   - Draw the multi-web-server, one DB, multi-client picture.
 - Standard solution:
   - Web app is sequential.
   - Run a copy of it for each user request.
   - These copies only interact at the database, which handles any 
     concurrency issues.
   - This works well for many apps, but is silly for apps like "draw".

The Elixir and Erlang Languages:

 - Erlang was developed in the 80's at Ericson for telcom switches.
 - It's a compiled-to-bytecode-with-VM language like Java or C#.
 - Tuned for two specific properties:
   - Reliability
   - Concurrency (initially for reliablity)
 - Built for telecom switches, where “the service will be down for maintenence” 
   is simply not OK.
 - Lightweight processes.
 - “Let it crash”
 - The semantics of Erlang are great.
 - High reliability and a focus on concurrency are awesome for web apps.
 - Erlang syntax is the absolute worst. You literally can’t use user-defined 
   functions in an “if” condition, as a *syntax* rule.
 - Elixir is a language for the Erlang BEAM VM with the Erlang semantics but
   much less painful syntax.

The Elixir plan for web apps:

 - Each request is handled by a new Erlang process.
 - Erlang processes can be used to store state, or to manage long-running
   client connections.
 - Safe communication between processes, even across multiple servers, is
   innately provided by the VM.

The language itself is similar to ISL from Fundies 1:

 - Elixir is a lot like ISL from Fundies 1.
 - Functional language
 - Can't mutate data.
 - Can't re-assign variables.
 - Linked lists as core data type
 - No loop statements
 - Repeat by recursion
 - Loop functions: map, filter, reduce
 - Interactive REPL

But with some extra features not in ISL:

 - Non-LISP syntax
 - Seperate function / variable namespaces
 - Modules
 - Pattern matching
 
 - Side effects (like I/O)
 - Maps (associative arrays) are a core data type
 - Lightweight processes
 - send / receive 

## Elixir examples:

 - Fib
   - Compile Fib
   - Call Fib.fib from REPL (iex)
   - Pattern matching for functions
 - Fact
   - Accumulator pattern
   - Note tail recursion
 - RW
   - ElixirScript
   - Loop functions
   - Functions have module prefixes
   - Pipelines
