
# Web Development: Semester Review

## What is Web Development?

 - Creating services that are available over
   the internet?
   - Is developing an email server (SMTP) 
     web development?
 - Developing applications that run in the
   browser?
   - Is porting Quake to asm.js by running
     a compiler web development?
 - Information served over HTTP?
   - Is writing a Matrix (chat) client
     web development?
   - If so, why isn't an IRC client?

Is an Electron app web development?

 - It's certainly using web development
   tools.
 - The Atom text editor doesn't really
   feel like a "web app".

This semester:

 - We've actually built websites.
 - Web dev is probably broader than that.

## What did we do this semester

 - We built websites.
 - We started with a game: Bulls & Cows
   - Multiplayer
 - We built a CRUD App: Events
 - Now you've finished a project that should
   have had most of the components we covered
   this semester in it.

You now have:

 - Three deployed web apps.
 - Deployed on the public internet.
 - Source code on public Github
 - These might be useful portfolio examples. 

## Admin Your Own Server

After admining the server all semester:

 - You can build and host arbitrary web
   apps.
 - You're prepared to investigate "dev ops".
   - Before you can write code to solve a
     problem, you need to figure out the problem.
   - You've been the admin all semester.
 - You're prepared to evaluate tools like Heroku.
   - Infrastructure as a service.
   - Trade away flexibility for someone else's
     automation.
 - You're prepared to evalute tools like Amazon
   AWS.
   - You get some extra capabilities.
   - The cost is potential vendor lock-in.

What is dev ops?

 - Infrastructure as code
 - Automating sysadmin tasks
 - Having *developers* maintain and deploy
   apps, rather than non-developer ops teams.
 - Continuous integration / deployment

## The Browser

Browser technologies:

 - HTML, CSS, JS + DOM
 - Images, Videos, Audio
 - Canvas, WebGL, WebAssembly, etc.

## Elixir

On the server, there's no technology constraints
at all.

Elixir has some neat properties:

 - Dynamic, functional
 - Runs on the Erlang virtual machine
 - Reasonable library support for web development

The Erlang VM gives us:

 - Lightweight processes for native concurrency
   (and good support for multi-core execution)
 - Unique "let it crash" / supervision tree
   structure for reliability
 - Unique patterns for state management
 - Built in support for (small) clusters

Disadvantages:
 
 - Not as crazy popular as NodeJS, PHP, Python,
   or Ruby. 
   - You may not have known it already when we started.
   - You can't just find all your answers on StackOverflow
 - Not as fast as well-optimized C++, Rust, Go, C#, or 
   Java - at least on CPU bound tasks.

Recommended next steps:

 - Right now, you should be at least moderately
   comfortable working with Elixir/Phoenix.
 - I recommend writing a couple more practice apps.
 - Consider making personal projects an opportunity
   to try new tools:
   - NodeJS is really popular.
   - You should try writing a web app in a low level
     language like C++ or Rust:
     - This gives you a better indication of what
       you're getting from high level tools.
     - You'll also see what performance opportunities
       you're giving up with higher level tools.
 - Keep an eye out for cases where Elixir + Phoenix
   is the right tool.

### Phoenix Framework

Default web framework for Elixir. Gives you a script
to generate an empty default app, with a bunch
of ressonable defaults.

It provides four key things:

**Serving Web Pages**

Basic flow in Phoenix:

```
request
|> router
|> pipeline
|> controller action
|> view
|> template
```

**Phoenix Channels**

Bidirectional communications mechanism
allowing client <=> server messaging.

Key feature: Can push data from server to
client.

**Ecto + Generators**

Provides a decent pattern for accessing an SQL
database.

**Assets with Webpack**

This makes it easy to integrate simple JS / CSS
libraries for in-browser functionality.

Not quite as nice as create-react-app for complex
apps, but definitely sufficient for simple cases.

## Modern JavaScript Toolchain

We used create-react-app, which gave us access to

Some stuff that we also got from Phoenix

 * npm
 * Webpack
 * Babel

Some other stuff configured nicely:

 * React set up
 * A server with state-preserving live reload
 * Jest tests by default

Advantages to having a build step for JS.

 * We can check for errors and warnings at 
   "compile time".
 * We can optimize and minifiy our code.
 * We can use language variants not supported
   by the browser:
   * JSX
   * SASS
   * TypeScript

## React

Some applications benefit from having dynamic 
content on a single page without full page loads.

Modifying live webpages by hand with the DOM API 
is tricky, common solutions:

 * We could regenerate portions of the page by
   rendering a template.
 * We can act like an efficient game engine:
   * Represent app state as a model
   * Render that model to a portion of a web page
   * React optimizes rendering by diffing the
     output of the render process against the
     current page and making minimal changes.
   * Need JSX structured "templates".
 
A react component has:

 * A pure function that renders "props" and (optional)
   state into a virtual DOM tree.
 * An optional state, which is an immutable value,
   along with a function to replace that value and
   trigger a render.

Alternatives / enhancments to React:

 * More complete framework with react Next.js
 * Angular.js
 * Ember.js

If you just want dynamic templating:

 * Consider Vue.
 * Moustache / Handlebars

Another fun tool is Elm.

Generic tools for building a website:

Just a site, not an app:

 - Hugo (static site generator)
 - Non-technical users? Maybe Wordpress (dynamic CMS)

It's an app:

 - Time for Phoenix or similar

### OTP: GenServers, Agents, Supervisors

This is the magic of the Erlang VM, allowing state
in concurrent apps and providing effective tools
to manage failure.

Questions to keep in mind with any web app?

 - How do we store state outside the database?
   - A GenServer
   - A message queue service
 - How do access that state from other physical servers?
   - Erlang cluster
   - A message queue client
 - How do we handle errors?
 - When is crashing and recovering easier than handling
   a given error?
 - What do we lose when something crashes? Can that data
   be recreated when the component comes back up?

## Examples / Assignments

 - Draw
 - Bulls and Cows
 - Events
   - CRUD App
   - Traditional multi-page web app
   - React SPA (react router, redux)

## Other stuff

Security

 - Secure Passwords

Common web security issues:

 - SQL Injection
   - Enter name: "Bob'; drop table users;"
   - Sending raw text from the user to your SQL
     database as a command.
   - Any modern library will tend to prevent this

 - XSS
   - Do you allow users to post text to your site?
   - What if they post "<script>" tags?
   - You probalby do't want to let them do that.
   - Phoenix tries to protect against this by marking
     external text as unsafe.

```
# Controller

 post = get_post_from_db()
 render(..., post: post)

# Template

<!= @post.text %> 
<!-- any "<" will be escaped -->
```


 - XSRF
   - Does your server validate incoming
     requests to make sure they're from your site.
   - Web frameworks usally provide some sort of XSS
     protection mechanism.
   - Phoenix solves this with XSRF tokens.

```
 <form action="https://army.gov/launch" method="post">
   <p>Get Free Cake!</p>
   <input type="Subit">

 - Failure to validate requests
   - Can a non-admin do admin actions?


## Any questions?




