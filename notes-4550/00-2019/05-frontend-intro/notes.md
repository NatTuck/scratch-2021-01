+++
title = "./05-frontend-intro/notes.md"
+++

## Homework questions?

## Review: Phoenix Framework / Practice App

Install Elixir and Erlang (see guides page)


```
sudo apt install elixir=1.8.0-1 esl-erlang=1:21.2.3-1
```

Clone repo:

```
# clone git repo
$ git clone https://github.com/NatTuck/elixir-practice.git
$ cd elixir-practice

# fetch deps
$ mix deps.get
$ (cd assets && npm install)

# run local dev server
$ mix phx.server
```

 - Double a number.

HTTP story for doubling a number.

 - HTTP GET /
   - Reply: Index page.
 - HTTP POST /double
   - Request: Send value to double
   - Response: Show result page.

We can think of a HTTP request as being like a function call. The server
needs to take inputs (request path, query string, post data) and produce an
output (the response).

We can think of Phoenix as being a tool that helps us define those functions.

The basic flow in Phoenix is:

```
request
 |> router 
 |> pipeline
 |> controller action 
 |> view
 |> template
```

Let's walk through that for doubling a number.

Note that most of the logic is in lib/pratice_web, where
the web-specific stuff lives, but that we eventually call
out to a file outside that directory to actually double
the number. Our app could conceptually end up having,
e.g., a command line interface that doesn't use HTTP
and the code to double a number would still be used.

## Deployment Example

 - Deployment is important.
 - If your app doesn't deploy, it doesn't really matter what works on
   your development machine, nobody will see it.
 - It's worth half the points on the rest of the homeworks.

### Deploy Process Setup

Set up deployment environment on dev server.

 - log in to demo server
 - Set up Erlang / Elixir environment
 - Go to "install elixir" link on course site Phoenix install guide.

```
# wget https://packages.erlang-solutions.com/erlang-solutions_1.0_all.deb && sudo dpkg -i erlang-solutions_1.0_all.deb
# apt update
# apt install -y build-essential
# apt install -y esl-erlang elixir
# apt install -y nodejs npm
# apt install -y postgresql postgresql-client libpq-dev
```

### Deploy Practice App

Deploy hw02 app on demo server

 - log in to demo server
 - create practice user
 
```
$ sudo su -
# apt install -y pwgen
# pwgen 12 1
<random pw>
# adduser practice
...
New password: <random pw>
Retype new password: <random pw>
...
  Full name: Practice App
...
```

 - become new user
 - clone practice app

```
# sudo su - practice
$ git clone https://github.com/NatTuck/elixir-practice.git
```

 - install the provided nginx config file

```
# cd ~practice/elixir-practice
# cp practice.nginx /etc/nginx/sites-available/practice.ironbeard.com
# cd /etc/nginx
# view sites-available/practice.ironbeard.com
# ln -s /etc/nginx/sites-available/practice.ironbeard.com sites-enabled
# service nginx restart
```
 - pull up the deploy guide from github / master / guides / deployment
   - click through to the distillery guide
 - deploy the app
 
```
# sudo su - practice
$ cd elixir-practice
$ view deploy.sh
$ ./deploy.sh
```

 - if deploy is slow, "time host repo.hex.pm"
   - try 8.8.8.8 / 4.4.4.4 in /etc/systemd/resolvd.conf ?
 - no styling?
 - check source: stylesheet is "practice2.ironbeard.com"
 - edit config/prod.exs to have URL = practice.ironbeard.com
 - rerun deploy script

## New Example App: Hangman

Here's our first sample app (show it).

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


