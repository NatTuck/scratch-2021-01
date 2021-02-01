
# Hangman with React

Hangman overview:

 * Computer selects a secret word.
 * The word is displayed with the letters hidden.
 * User guesses letters.
 * When guesses match letters in word, those are no longer hidden.
 * If whole word is shown, user wins.
 * On 7th bad guess, user loses.

To create JavaScript app with external libraries, first we want ```npm```. To
get that, we want ```nvm```.

## Dev Tools

Install nvm: https://github.com/nvm-sh/nvm

Install the latest stable node: ```nvm install --lts```

Check node version: ```node --version```

## Create App
 
create-react-app docs: https://create-react-app.dev/docs/getting-started/

Create our Hangman app: ```npx create-react-app hangman```

This gives us a new app preconfigured with some common JS tools:

 - A build system with Webpack and Babel
 - Development server with live code updates
 - Tests with Jest
 - etc

This is a reasonable set of defaults for simple in-browser app development with
React.

Before I go nuts, let's add a ```.eslintrc``` to our new project root:

```
{
    "parser": "babel-eslint",
    "rules": {
        "no-unused-vars": ["warn", { "varsIgnorePattern": "^_" }]
    }
}
```

Run the starter code: ```cd hangman; npm start```

## Introduce React

Look at ```src/app.js```:

 * Rather than putting our HTML in our HTML file, we've stuck our HTML
   in the JavaScript file.
 * The traditional way to generate HTML from code is a template system, where
   snippets of HTML are represented as text. We'll use that method a bit later,
   but that's not what's happening here.
 * The HTML here isn't a string, or even valid JavaScript. Instead, it's a
   syntax extension called JSX where XML is embedded in our JavaScript code.
 * The build system translates our JS + XML files into normal JS files by replacing
   the XML with expressions that build a data structure.
   
Example JSX input:

```
function hello(name) {
    return (<p>Hello, <i>{name}</i>!</p>);
}
```

Example generated normal JS:

```
function hello(name) {
    return xml_tag(
        'p', ['Hello, ', xml_tag('i', [name]), '!']);
}
```

Doing this rather than using a traditional template requires an extra build
step, but has some advantages:

 * Basic validity of the XML (e.g. balanced tags) can be checked at
   build time.
 * A data structure representing that part of the HTML tree is built, this is
   called the Virtual DOM, since it's like the struture the browser uses
   (but simpler).
 * Having a data structure rather than unparsed text means that structure
   can be traversed for various reasons.
 * The virtual DOM can be converted to real DOM structure (with
   document.createElement, etc)
 * Mutating the DOM is one of the most expensive steps. This can be optimized
   with a virtual DOM by traversing both structures and only making the minimum
   required change.
 * That optimization makes it possible to *logically* regenerate the whole
   document subtree any time anything changes.
 * This ends up making dynamic pages work kind of like the 3D rendering engine
   from a video game, where every frame is rendered by transforming a model
   representing the current state of the game into a visual frame rather than
   by considering and transforming the previous frame.

What's actually happening here?

 * View source on the starter page.
 * We've got a div with id="root"
 * We've got some script tags.
 * Those script tags are the build output of a program that logically
   starts at src/index.js - open that.
 * We've got a bunch of module imports
   * From external libraries like React
   * From in our app, like "./App"
 * We use React to render a JSX tree into the element with id="root", its
   content is replaced by the JSX tree.
 * StrictMode - https://reactjs.org/docs/strict-mode.html
   * This tries to make sure that we follow the React rules, at least
     in dev mode.
 * JSX nodes with a capitalized name refer to variables, so the App
   tag here calls the App function from "./App.js" to get more tree.

Let's replace this starter app with a simple counter.

 * We have a number, displaying the current count.
 * The count starts at zero.
 * We have a "+1" button which adds 1 to the count, and a "reset" button
   which resets the count to zero.

First version:

```
function App() {
  let count = 0;
  return (
    <div className="App">
      <h1>{count}</h1>
      <p>
        <button>+1</button>
        <button>Reset</button>
      </p>
    </div>
  );
}
```

Thoughts:

 * The className attribute in JSX gives us the class attribute in HTML, because
   class is a reserved word in JS and so class is called className in the DOM.
 * This displays the count.
 * The buttons don't work.

Making the buttons work:

 * Functions that generate JSX to be displayed in a React app are called
   "render" functions.
 * React requires that render functions are *pure* functions, in the functional
   programming sense, with some specific exceptions.
 * Pure, in this context, means:
   * Produces a single result value (a JSX tree).
   * That result varies only based on the function arguments.
     * That means no accessing mutable global state
     * Wait, there are no arguments, so the result is constant.
   * No side effects like I/O
     * Doing console.log for debugging is probably fine
 * That means we can't simply mutate the value of the "count" variable.
   * It violates the requirement that the function be pure.
   * It won't work - the function needs to get called again to rerender, and
     mutating a local variable doesn't make that happen.
 * React provides a mechanism called "state" which helps here. Here's how
   that works:
   
```
import { useState } from 'react';
import './App.css';

function App() {
  const [count, setCount] = useState(0);
  return (
    <div className="App">
      <h1>{count}</h1>
      <p>
        <button onClick={() => setCount(count + 1)}>+1</button>
        <button onClick={() => setCount(0) }>Reset</button>
      </p>
    </div>
  );
}
```

This useState mechanism works by telling React that our App component logically
has its own internal state. The useState "hook" function returns two things: the
current value of the component's state, and a "set" function. The set function
sets the state, but it critically also triggers a rerender so the display
matches the new state.

With useState this function is no longer pure in a strict theoretical sense
because it references external state - unless we think of the state as being an
implicit parameter. Note that React still requires that render functions be pure
aside from use of "hook" mechanisms like useState.

The other thing going on here that needs some comment is "onClick":

 * This isn't really producing an onClick attribute, it's telling react to
   bind an event handler to the resulting element after render.
 * We're passing in an anonymous function (a lambda), so execution is delayed.
 * Since the lambda isn't called during the execution of the render function,
   it's allowed to have arbitrary side effects.
 * A useState "set" function is a pretty helpful side effect, since it updates
   the state and triggers a rerender of the component.

So that's the basics of what React is and how it works. React is one of these
"archetecure" things, that makes simple things more complicated and more
complicated things simpler by providing structure and constraints.

## Building Hangman

In designing a React app, we can come at it from two different directions:

 * What does the UI want to look like?
 * What state do we need to track to be able to render our UI?

For hangman, the basic state is:

 * The secret word.
 * The user's guesses.

Most of the other stuff can be calculated from those two things. For example,
which letters to show in the word doesn't need to be an explicit part of the
state because we can derive it from those two things.

So we can start with something like this:

```
import { useState } from 'react';
import './App.css';

function App() {
  const [secret, setSecret] = useState("elephant");
  const [guesses, setGuesses] = useState([]);
  
  return (
    <div className="App">
      <p>Secret: {secret}</p>
      <p>Guesses: {guesses.join(' ')}</p>
    </div>
  );
}
```

Then we need some way to handle input. Let's add a text box that takes
single letter inputs and a "reset" button:

```
import { useState } from 'react';
import './App.css';

function App() {
  const [secret, _setSecret] = useState("elephant");
  const [guesses, setGuesses] = useState([]);

  function guess(ev) {
    let guess = ev.target.value;
    setGuesses(guesses.concat(guess));
  }

  return (
    <div className="App">
      <h1>Secret: {secret}</h1>
      <h1>Guesses: {guesses.join(' ')}</h1>
      <p>
        <input type="text" value="" onChange={guess} />
      </p>
      <p>
        <button onClick={() => setGuesses([])}>
          Reset
        </button>
      </p>
    </div>
  );
}

export default App;
```

Form inputs are a bit weird with React. The plan with React is to make all state
explicit with useState, but form inputs tend to have their own internal state.
In the case of a "text" input, that state is the text in the box.

There are two ways to handle this with React:

 * It's possible to have an "uncontrolled" input, where React renders the form
   field but doesn't manage its state. This can work, but requires a careful
   analysis of when React might re-render the control. If that happens, React
   won't preserve the input's state because it doesn't know about it.
 * The default option is to create a "controlled" input, where the input's state
   is explicitly handled by React. This requires setting the "value" and
   "onChange" parameters.

Here's a version with a "guess" button and an explicit controlled input:

```
function App() {
  const [secret, _setSecret] = useState("elephant");
  const [guesses, setGuesses] = useState([]);
  const [guess, setGuess] = useState("");

  function updateGuess(ev) {
    let text = ev.target.value;
    if (text.length > 1) {
      text = text[0];
    }
    setGuess(text);
  }

  function makeGuess() {
    setGuesses(guesses.concat(guess));
    setGuess("");
  }

  function keypress(ev) {
    if (ev.key == "Enter") {
      makeGuess();
    }
  }

  return (
    <div className="App">
      <h1>Secret: {secret}</h1>
      <h1>Guesses: {guesses.join(' ')}</h1>
      <p>
        <input type="text"
               value={guess}
               onChange={updateGuess}
               onKeyPress={keypress} />
        <button onClick={makeGuess}>
          Guess
        </button>
      </p>
      <p>
        <button onClick={() => setGuesses([])}>
          Reset
        </button>
      </p>
    </div>
  );
}
```

Note that we need to explicitly handle:

 * Updating the value in the input field when it changes.
 * Limiting the length of the field to 1 character.
 * Handling the Enter key.
 * Clearing the field when a guess is made.


Next, we want to add some game functionality.

```
// App.js

import { useState } from 'react';
import { uniq, bad_guesses, word_view, lives_left } from './game';
import './App.css';

function App() {
  const [secret, _setSecret] = useState("elephant");
  const [guesses, setGuesses] = useState([]);
  const [guess, setGuess] = useState("");

  let view = word_view(secret, guesses);
  let bads = bad_guesses(secret, guesses);
  let lives = lives_left(secret, guesses);

  function updateGuess(ev) {
    let text = ev.target.value;
    if (text.length > 1) {
      text = text[0];
    }
    setGuess(text);
  }

  function makeGuess() {
    setGuesses(uniq(guesses.concat(guess)));
    setGuess("");
  }

  function keypress(ev) {
    if (ev.key == "Enter") {
      makeGuess();
    }
  }

  if (lives <= 0) {
    return (
      <div className="App">
        <h1>Game Over</h1>
        <p>
          <button onClick={() => setGuesses([])}>
            Reset
          </button>
        </p>
      </div>
    );
  }

  return (
    <div className="App">
      <h1>Word: {view.join(' ')}</h1>
      <h1>Guesses: {guesses.join(' ')}</h1>
      <h1>Bads: {bads.join(' ')}</h1>
      <h1>Lives: {lives}</h1>
      <p>
        <input type="text"
               value={guess}
               onChange={updateGuess}
               onKeyPress={keypress} />
        <button onClick={makeGuess}>
          Guess
        </button>
      </p>
      <p>
        <button onClick={() => setGuesses([])}>
          Reset
        </button>
      </p>
    </div>
  );
}

export default App;
```

```
// game.js

export function uniq(xs) {
    return Array.from(new Set(xs));
}

export function word_view(secret, guesses) {
    let view = [];
    for (let cc of secret.split('')) {
        if (guesses.includes(cc)) {
            view.push(cc);
        }
        else {
            view.push("_");
        }
    }
    return view;
}

export function bad_guesses(secret, guesses) {
    let letters = secret.split('');
    let bads = [];
    for (let gg of guesses) {
        if (!letters.includes(gg)) {
            bads.push(gg);
        }
    }
    return uniq(bads);
}

export function lives_left(secret, guesses) {
    return 8 - bad_guesses(secret, guesses).length;
}
```

More stuff to do:

 * Make game prettier, factor out components.
 * Have word be random from list.
