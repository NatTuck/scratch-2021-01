---
layout: default
---

## Homework questions?

## Where are we?

 - We have our Elixir / Phoenix environment set up.
 - We have a new app called Hangman.
 - We've cleaned up the defaults and installed Milligram.
 - Now we're going to look at our client-side rendering framework: React

### Concept: React

 - Traditionally, web sites consisted of a series of pages.
 - On each page, you could scroll around and click links.
 - To get new content, you clicked a link, sent an HTTP
   request, and the server responded with a new page.

Some applications can benefit from having dynamic content on
a single page, without further page loads.

 - This can be accomplished with client-side rendering.
 - Obvious plan: Template system in JS.
 - Less obvious plan: Act like a functional version of a 3D 
   game engine.

React uses plan B:

 - A react component consists of:
   - A single "state" value.
   - A pure function that renders the state as HTML content.
     - render(state) => DOM tree
   - Event handlers that produce a new state and trigger a re-render.
 - This is a lot like Big Bang from Fundies 1.
 - When a new DOM tree is produced, it's diffed with the real DOM to
   minimize rendering costs.

React was developed at Facebook because this design works well with how HTML is
actually displayed in browsers, and has become one of the most popular
client-side rendering systems. It also happens to work really well with our plan
to use Elixir and Phoenix for server-side logic.

In order to produce DOM trees as its output, React has a kind of weird seeming
plan: Instead of normal templates, it uses a JavaScript extension called JSX to
embed XML directly in JavaScript code. Luckly, babel can handle JSX.


### Adding React Support

(in assets)

```
$ npm install --save-dev @babel/preset-env @babel/preset-react
$ npm install --save react react-dom
$ npm install --save lodash jquery # common libraries
```

Add react preset to babel in webpack config:

```
 module: {
    rules: [
      {
        test: /\.(js|jsx)$/,
        exclude: /node_modules/,
        use: {
          loader: 'babel-loader',
          options: {
            presets: ['@babel/preset-env', '@babel/preset-react'],
          },
        }
      },
      {
        test: /\.css$/,
        use: [MiniCssExtractPlugin.loader, 'css-loader']
      }
    ]
  },
```

### Adding a React Component

In lib/hangman\_web/templates/page/index.html.eex

Replace the contents with:

```
<div class="row">
  <div class="column">
    <h1>TODO</h1>
    <div id="root">
      React component loading...
    </div>
  </div>
</div>
```

Call your new component from assets/js/app.js:

```
// Add to bottom of app.js
import todo_init from "./todo";

window.addEventListener("load", (_ev) => {
  let root = document.getElementById('root');
  if (root) {
    todo_init(root);
  }
});
```

TODO state:

 - The state of the TODO app is a list of tasks.
 - Each task has a description and a "done?" flag.

TODO rendering:

 - Bulleted list
 - toggle buttons 
 - new item form.

 - Write todo.jsx


### Hangman

Create assets/js/hangman.jsx:

```
import React from 'react';
import ReactDOM from 'react-dom';

export default function hangman_init(root) {
  ReactDOM.render(<Hangman />, root);
}

class Hangman extends React.Component {
  constructor(props) {
    super(props);
  }

  render() {
    return <div>
      <h2>Hangman loaded.</h2>
    </div>;
  }
}
```

Call your new component from assets/js/app.js:

```
// Add to bottom of app.js
import hangman_init from "./hangman";

window.addEventListener("load", (_ev) => {
  let root = document.getElementById('root');
  if (root) {
    hangman_init(root);
  }
});
```

### Design for Hangman

The game:

 - There's a secret word.
 - The player trys to guess the letters in the word.
 - The letters in the word are shown, initially as blanks and
   then as letters once correctly guessed.
 - The player has a fixed number of bad guesses available, they
   lose if they run out.

The game state:

 - The word
 - The guesses so far
 - Max guesses allowed

### Hangman Code

 - hangman.jsx

### Setup for Deployment

 - Add elixir module: distillery.
 - Follow deploy guide to create a deploy script.
 - Worry about secrets.
   - config/prod.secret.exs - Copy manually from dev machine.
   - rel/config.exs - Random cookie code; will break if we try
         to run in a cluster.
 - There should *never* be secrets in your git repo.


