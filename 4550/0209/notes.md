
# Homework Questions

 - Q:Why does my server break when my local VM
   stops?
 - A: You're running your server in your SSH 
   session.
 - Solution: Create a system service to run
   your app, using systemd.

 - Pattern matching

# Hangman: Server Side Logic

Core design goal for new version: The secret 
stays secret, the game logic is enforced.

To do that, we need to keep the secret and game
code on the server, not expose it to the JS code
in the browser.

Create a new Elixir / Phoenix app, precondition
is installing Elixir and Phoenix per their docs.

# Plan for Server-Side Logic

 - We're going to model our game state as a
   single Elixir object.
 - We're going to have an Elixir module that
   models our game state functionally.
   - { State, Event } => State
 - When an event occurs in the browser, we
   send it to the server.
 - Server will send back a new state.
 - That state object can be our React state.

For this plan to work, our React state needs
to be a single object.


# Browser <=> Server Comms

 - Phoenix makes websockets easy with
   Phoenix Channels
 - Allows bi-directional message passing
   of JSON objects.
  
  
  
   












