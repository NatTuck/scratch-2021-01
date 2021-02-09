
# Hangman: Server-Side Logic

We're going to move our browser-only Hangman app to have the core game logic
handled by code running on the server.

Figuring out whether logic goes on the server or in the browser involves a bunch
of trade-offs, but one thing we're going to explicitly consider here is that we
don't want to allow users to cheat by running the React debugger addon or
altering our JavaScript code to reveal the secret early.

To avoid this:

 - The secret stays on the server.
 - No information is revealed to the browser code that shouldn't be revealed
   to the user.

## Creating a Phoenix App

Precondition: I've followed the official docs to install Elixir, NPM, and set up
Phoenix.

```
$ mix phx.new hangman --no-ecto
```

Let's see what we got.

```
$ mix phx.server
```

Cleaning up the default stuff:

 - Remove text from layout.
 - Replace text on page/index
 - Remove logo
 - Remove stock styles
 - Install milligram

```
<!-- page/index.html.eex -->
<div class="row">
  <div class="column">
    <div id="root">
      React component loading...
    </div>
  </div>
</div>
```

Setting up for React:

 - Follow the [course
guide](https://www.ntuck-neu.site/2021-01/cs4550/guides/react-phoenix/).

## Moving Hangman code from Browser version

Copy stuff from ```src``` in CRA app to ```assets/js``` in Phoenix app:

 - App.js => Hangman.js
 - game.js

Fixup:
 
 - Rename App() to Hangman()
 - Remove references to App.css
 - Explicitly import React from Hangman.js:
 
```
import React, { useState, useEffect } from 'react';
```

Add to the bottom of app.js:
 
```
import "phoenix_html";
import React from 'react';
import ReactDOM from 'react-dom';

import Hangman from './Hangman';

ReactDOM.render(
  <React.StrictMode>
    <Hangman />
  </React.StrictMode>,
  document.getElementById('root')
);
```

## Refactoring for Single State

```
  const [state, setState] = useState({
    secret: "elephant",
    guesses: [],
  });

  let {secret, guesses} = state;
  
  ...
  
  function makeGuess(guess) {
    let guesses = uniq(guesses.concat(guess));
    let state1 = Object.assign({}, state, {guesses});
    setState(state1);
  }
```

## Implementing Hangman in Elixir

Here's the general plan:

 - Define an object layout representing our game state.
 - Define functions that take {state, event} and produce a new state.
 - Define a view function that produces the data we're willing to reveal to the
   player given a game state.
 - This game module is functional - the functions are mostly pure, and we
   definitely aren't *doing* anything, just computing stuff.

lib/hangman/game.ex:

```
defmodule Hangman.Game do
  def new do
    %{
      secret: random_secret(),
      guesses: MapSet.new,
    }
  end

  def guess(st, letter) do
    %{st | guesses: MapSet.put(st.guesses, letter) }
  end

  def view(st) do
    word = st.secret
    |> String.graphemes
    |> Enum.map(fn xx ->
      if MapSet.member?(st.guesses, xx) do
        xx
      else
        "_"
      end
    end)
    |> Enum.join("")

    %{
      word: view,
      guesses: MapSet.to_list(st.guesses)
    }

    %{
      word: word,
      guesses: MapSet.to_list(st.guesses)
    }
  end

  def random_secret do
    Enum.random(~w[
          elephant embezzle bayou blizzard jiujitsu
          jovial rythm rhubarb zephyr zipper
    ])
  end
end
```

## Creating a Channel

We want to be able to send messages between our JS code in the browser and our
Elixir code on the server. There are a couple ways to do this, but the Phoenix
framework comes with a built-in setup for easy websockets: Phoenix Channels.

A Channel gives us a long-running bidirectional connection that lets us pass
messages between our JS and Elixir code.

To set this up:

```
$ mix phx.gen.channel Game
```

Add this to ```user_socket.ex```:

```
    channel "game:*", HangmanWeb.GameChannel
```

Now take a look at ```game_channel.ex```.

 - When the browser makes a connection, this creates a "socket" object.
 - Then it calls ```join/3```, giving you the socket to update on the join
   event. This is passed the channel name, in case we want that.
 - Whenever there's an incoming message, it calls ```handle_in/3```. Generally
   that has clauses matching on the first argument, which is the event name
   string as sent by the other end of the channel.

First, update join to take an arbitrary game ID in the name and starts a new
game when we connect. Note that we can use ```assign/3``` to store any data we
want in the socket object.

```
  @impl true
  def join("game:" <> _id, payload, socket) do
    if authorized?(payload) do
      game = Hangman.Game.new()
      socket = assign(socket, :game, game)
      view = Hangman.Game.view(game)
      {:ok, view, socket}
```

Then, change handle_in so it handles exactly "guess" messages and uses them to
update our game state:

```
  @impl true
  def handle_in("guess", %{"letter" => ll}, socket) do
    game0 = socket.assigns[:game]
    game1 = Hangman.Game.guess(game0, ll)
    socket = assign(socket, :game, game1)
    view = Hangman.Game.view(game1)
    {:reply, {:ok, view}, socket}
  end
```

## Hooking up the Browser End

First, we need to clean up socket.js a bit:

```
import {Socket} from "phoenix";

let socket = new Socket("/socket", {params: {token: ""}});
socket.connect();

let channel = socket.channel("game:1", {});

let state = {
  word: "______",
  guesses: [],
};

let callback = null;

function state_update(st) {
  console.log("New state", st);
  state = st;
  if (callback) {
    callback(st);
  }
}

export function ch_join(cb) {
  callback = cb;
  callback(state);
}

export function ch_push(guess) {
  channel.push("guess", guess)
         .receive("ok", state_update)
         .receive("error", resp => { console.log("Unable to push", resp) });
}

channel.join()
       .receive("ok", state_update)
       .receive("error", resp => { console.log("Unable to join", resp) });
```

Now we can update our Hangman.js:

```
// function Hangman() {
// ...

  useEffect(() => {
    cb_join(setState);
  });

  function makeGuess(guess) {
    cb_push(guess);
  }
```

## More stuff to do:

 - Move bad guesses and lives into the server-side view.
