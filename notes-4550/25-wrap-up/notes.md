---
layout: default
---

## First: Project Questions?

Presentation Q&A

 - Starts on Friday
 - Teams are posted on Piazza; order per day will be random
 - There will be a peer-evaluation component.
 - You need to show up both days.

# Web Development, Fall 2019

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
 - Then you built a larger game as a project.
 - Then we made a CRUD app
 - Now you're working on a larger application with CRUD elements.
 - By the end of semester you have at least three applications deployed to the
   public internet, with source on Github, that you can potentially show off as
   class projects.

### Admin Your Own Server

The first thing that we did was to each deploy our own virtual server.

Now after getting to enjoy a semester of administrating your own server and
doing application deploys manually:

 - You can build and host arbitrary serious web apps.
 - You're prepared to appreciate "dev ops" and fully automating deployments.
 - You're prepared to evalute tools like Heroku, which trade away some
   capabilities in favor of letting someone else be the sysadmin.
 - You're prepared to evalute tools like Amazon AWS, which provide extra
   capabilities over a commodity VPS at the cost of potential vendor lock-in.

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

Next steps:

 - At this point, you should be at least moderately comfortable working with
   Elixir/Phoenix.
 - I recommend building a couple of apps on your own as general practice and to
   try out new things.
 - Consider taking personal projects as an opportunity to try new tools:
   - NodeJS is really popular. Try building an app with that and find out what
     the advantages and drawbacks are.
   - It's worth building a web app in a low level language like Rust or C++.
     This will give you some idea of A.) what higher level tools are giving you
     and B.) what performance higher level tools force you to give up.
 - Keep an eye out for cases where Elixir + Phoenix is clearly the right tool.
   Having built-in server-side transient state and distribution can be powerful.

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

 - There are lots of libraries available for JS (and CSS)
 - Being able to install them with NPM and efficiently bundle them is nice.
 - Consider looking into yarn as an alternative to npm.
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

Alternatives to React:

React is a rendering library, which when combined with other libraries like
Redux gives you the tools needed to build browser-side logic.

There are also more complete frameworks with more traditional (template style)
rendering, like Angular or Ember.

If you just want dynamic templating, a popular choice is Vue.

Another thing that's worth looking into is Elm. It's a functional language that
compiles to JavaScript, and goes even further than React on the Functional
Reactive Programming strategy for UI rendering.

### WebSockets / Channels

This recent additon to browsers significantly changes what's possible in web
applications. With websockets, the server can push information to the browser
without any need for polling.

Five years ago, apps that relied on pushing updates from the server to the
browser were uncommon. Going forward, I expect that websockets will become the
norm, incrementally replacing AJAX-style calls.

Phoenix Channels are an excellent toolkit for websockets, but most languages and
frameworks now provide decent support as well. 

Keep in mind that good support for websockets requires language and runtime
support: Either every socket needs a concurrent process as in Phoenix, or it
needs to be managed with async IO and some sort of language support for async
events.

Websockets also mess with traditional methods for HTTP load balancing. Requests
are no longer short or stateless.

### OTP: GenServers, Agents, Supervisors

This is the magic of the Erlang VM, allowing transient state to be maintained
even in a concurrent application and failure to be managed.

Questions to keep in mind in any web app:

 - How do we store state outside the database?
 - How can we access that state from other physical servers?
 - How do we handle errors?
 - When is crashing and recovering easier than handling the error?
 - What do we lose when something crashes? Can that data be recreated when that
   component comes back up?

### Example: Draw!

 - http://draw.ironbeard.com
 - https://github.com/NatTuck/draw

### Assignment: Bulls & Cows

 - http://pigs.ironbeard.com
 
### REST, Resources and Relations

 - Route conventions
 - Mapping to a Relational DB
 - Ecto
 - Support in Phoenix

### AJAX and JSON Resources

 - JSON is a nice format for transfering data rather than HTML.
 - This is useful both for non-browser clients and browsers making
   requests without a full page load.

### Assignment: Events

 - CRUD app
 - Traditional multi-page web app

### Single Page Applications

 - Concept
 - React with React-Router
 - Redux

### Security

Passwords:

 - We don't want people impersonating our users.
 - Standard conventions for password security.

Common Web Security Issues:
 
 - XSS
 - XSRF
 - Injection attacks
 - Simple logic errors:
   - Failure to verify permissions on requests.
 - Denial of Service

### WebAssembly

WebAssembly is the technology I'd bet on for the biggest impact to web
development over the next ~5 years.

WebAssembly means that code for the browser no longer needs to be JavaScript.
That enables two major changes:

 * You can use libraries from other languages in traditional JavaScript apps.
   * We saw an example of building a Rust crate into an NPM package.
 * You can write in-browser web apps entirely in other languages.
   * Like Eclipse + Java + Maven? The browser can be a build target.

Here's a Java app with a Swing UI running in the browser:

 * http://reportmill.com/snaptea/RM15/
 
Here's a C++ app with a Qt UI:

 * https://www.qt.io/web-assembly-example-slate

Initially I expect this to mostly be used for porting existing desktop apps to
run in the browser. But I also expect to see apps developed from scratch with
this in mind.

Maybe we'll even get cross platform + browser apps that aren't written using
Electron. That'd be great.

### Extra Topics

 - Canvas
 - WebGL
 - NoSQL
 - Distributed Elixir

