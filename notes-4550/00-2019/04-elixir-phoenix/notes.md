+++
title = "./04-elixir-phoenix/notes.md"
+++

## Elixir Language

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

## Phoenix Framework / Practice App

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


### Deployment

 - Create "pratice" user on server.
 - Clone git repo to server.
 - Read deploy.sh - do TODOs.
 - Double check config/prod.exs - set URL.
 - Run deploy.sh
 - Install nginx config file.
 - Read start.sh - do TODOs.
 - Run start.sh
 - Think about what happens on reboot, either:
   - Add @reboot rule to crontab (crontab -e ; @reboot /home/practice/.../start.sh )
   - Create and install a systemd service file for your app.

The deployment documentation:

 - https://hexdocs.pm/distillery/guides/phoenix\_walkthrough.html
 - https://github.com/phoenixframework/phoenix/blob/master/guides/deployment/deployment.md

## Tick Example

 - See Tick example.

