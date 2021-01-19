+++
title = "./24-wrap-up/notes.md"
+++

## Web Development Wrap Up

### What is Web Development?

What is the web?

 - Protocols: HTTP, URLs
 - Content Standards: HTML / CSS / JS
 - Data Conventions: JSON, REST

What is web dev?

 - Broadly, any software development that involves HTTP or a browser.
 - That could be anything.

What did we cover?

 - Mostly websites.
 - Started with a Game
 - Then we made a CRUD app
 - You have at least three applications deployed to the public internet,
   with source on Github, that you can potentially show off as class
   projects.

### Admin Your Own Server

The first thing that we did was to each deploy our own virtual server.

Now after getting to enjoy a semester of administrating your own server and
doing application deploys manually:

 - You can build and host arbitrary serious web apps.
 - You're prepared to appreciate "dev ops" and fully automating deployments.
 - You're prepared to evalute tools like Heroku, which trade away some capabilities
   in favor of letting someone else be the sysadmin.

This semester we did better than usual: I haven't heard about any students
getting their server hacked.

### The Browser: Web Pages

Browser technology:

 - HTML, CSS, JS + DOM
 - Images, Videos, Audio
 - Canvas, WebGL, WebAssembly, etc...

### Elixir

Elixir is a neat programming languages with some neat properties:

 - Dynamic, functional
 - Runs on the Erlang VM
 - Reasonable library support for web dev

The Erlang VM gives us:
 
 - Lightweight processes for native concurrency
 - A unique "let it crash" / supervisor restarts story for reliability
 - Unique patterns for state management
 - Built-in support for small clusters of communicating machines, which
   allows the state patterns to scale horizontally.

Disadvantages:

 - Not as crazy popular as NodeJS, PHP, Ruby, or Python so you can't always copy
   the program you want off StackOverflow.
 - Not quite as fast as a well-optimized C++, Rust, Go, C#, or Java program.
 - I recommend building a test web app using tools from each of those categories.

### Phoenix Framework

Phoenix is the popular web framework for Elixir. It comes with a script to
generate starter code with a bunch of reasonable defaults that provides four
things:

**Serving Web Pages**

The basic flow in Phoenix is:

```
request
 |> router 
 |> pipeline
 |> controller action 
 |> view
 |> template
```

**Phoenix Channels**

A solid bidirectional communication mechanism server <-> browser using
WebSockets. Combines well with the state management mechanisms provided by
Elixir/Erlang, but can also be used just as a faster replacement for AJAX calls.

**Ecto + Generators**

Provides a reasonable pattern for access to a SQL database.

**Assets with Webpack**

Webpack is the current popular tool for packaging browser assets (JS, CSS), and
it's well integrated into the default Phoenix setup.

### Webpack, Babel, Modern JS

 - There are lots of libraries avaialble for JS (and CSS)
 - Being able to install them with NPM and efficiently bundle them is nice.
 - It's also nice to have code preprocessed for compatibility: ES2016
 - And to get stuff that browsers won't have: SASS, JSX.
 
### React

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

### WebSockets / Channels

This recent additon to browsers significantly changes what's possible in web
applications. With websockets, the server can push information to the browser
without any need for polling.

### OTP: GenServers, Agents, Supervisors

This is the magic of the Erlang VM, allowing transient state to be maintained
even in a concurrent application and failure to be managed.

### Example: Draw!

 - http://draw.ironbeard.com
 - https://github.com/NatTuck/draw

### Assignment: Memory Game

 - http://memory.ironbeard.com

### REST, Resources and Relations

 - Route conventions
 - Mapping to a Relational DB
 - Ecto
 - Support in Phoenix

### AJAX and JSON Resources

 - JSON is a nice format for transfering data rather than HTML.
 - This is useful both for non-browser clients and browsers making
   requests without a full page load.

### Assignment: Task Tracker

 - CRUD app
 - Traditional multi-page web app

### Single Page Applications

 - Concept
 - React with React-Router
 - Redux

### Password Security

 - We don't want people impersonating our users.
 - Standard conventions for password security.

### Extra Topics

 - Canvas
 - WebGL
 - Web Assembly
 - NoSQL
 
### Presentations

 - We did presentations.


