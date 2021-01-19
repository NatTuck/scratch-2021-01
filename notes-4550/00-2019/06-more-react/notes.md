+++
title = "./06-more-react/notes.md"
+++

## Homework questions?

## New Example App: Hangman

Let's build it.

 - This app will use entirely client-side logic. 
 - Therefore, the first thing we need is to install Phoenix. 
 - Why?
 - Because we want to use extensions to JavaScript that aren't
   supported by browsers.
 - Specifically, ES2016 and JSX.
 - The standard tools for this development strategy are now:
   - webpack - a build system for web dev
   - babel - a JS-to-JS compiler
 - Phoenix gives us these tools set up reasonably by default.

### Installing Phoenix 1.4

 - First, make sure erlang / elixir / nodejs are installed
   on your dev machine.
 - Follow the installation guide in the Phoenix docs.

### Creating our App

```
$ mix phx.new hangman --no-ecto
$ cd hangman
$ mix phx.server
```

 - With --no-ecto, no database server required.
 - Visit http://localhost:4000/ to see default app index.


### Removing / Replacing Defaults

```
$ cd assets
$ rm static/images/phoenix.png
$ rm css/phoenix.css
$ npm install --save milligram
```

css/app.css

```
# remove @import "./phoenix.css"
@import "~milligram/dist/milligram";
```

add "resolve" block to webpack config:

```
  //},
  resolve: {
    extensions: ['.js', '.jsx', '.css'],
  },
  //plugins: [
```

In lib/hangman\_web/templates/layout/app.html.eex:

 - Delete the (header)

### Review: React

React is about having a data model and a function that renders it to
an HTML DOM tree. When the data model changes, React re-renders for
you.

This means that React gets full control of both the HTML content and
event handling code for the portion of the page that is a React
component.

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


