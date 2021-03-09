---
layout: default
---

## Homework questions?

## Hangman in the Browser

 - Git repo: https://github.com/NatTuck/hangman-2019-01/
 - This lecture goes from branch "master" to "01-31-channel-hangman"
 - Fix app.js
 - Work in hangman.jsx

### Design for Hangman

The game:

 - There's a secret word.
 - The player trys to guess the letters in the word.
 - The letters in the word are shown, initially as blanks and
   then as letters once correctly guessed.
 - The player has a fixed number of lives available,
   they lose one per bad guess, zero lives = loss.

The game state:

 - The word
 - The guesses so far
 - Starting lives

### Hangman Code

 - hangman-browser.jsx

## Channels & Server-side Logic
 
 - Start branch "01-31-channel-hangman"
 - End branch "01-31-browser-hangman"

## Server-side Logic: Why, How, and Complications

 - We're going to spend a lot of time focusing on the idea of Application State.
 - React makes this concept really clear:
   - We have some structured data.
   - What we display to users depends on the current value of that data.
   - We want to modify that data in response to events - in fact, that's the
     core thing our application does is change its state.
 - State seems really simple, and in local sequential applications it can be. In
   simple OO programming it's just fields on objects.
 - State gets more complicated with concurrency and distribution. There are two
   main problems that come up:
   - Data races, when there are two conflicting updates to data "at the same
     time".
   - Inconsistency, when an update to one part of the overall state isn't
     reflected in duplicate copies or saved derived values.
 - I'll repeat this point, but keep it in mind going forward.

Issues with in-browser Hangman.

 - Right now our state lives in the state field of of our root react component.
 - That's great for a single-user app with no hidden information.
 - Problem: We want hidden information.
   - In hangman, the user shouldn't know the secret word.
   - We send that right to them in our JS code.
   - We'd rather not do that.
 - Problem: Cheating
   - The game rules are enforced in JS code.
   - Users can modify JS code running on their own machine.
 - Problem: What about multiple users?
   - Hangman isn't the greatest example for a multi-player game, but...
   - We can imagine two-player hangman where either player can guess at any
     time.
   - Having the state in one user's browser doesn't work here. Both players
     need to be able to view the up-to-date game display, and both players need
     to be able to guess letters (thus modifying the state).
 - This isn't just Hangman
   - Games in general are a good example for wanting hidden information.
   - Most web application are likely to want multiple users.

New design: State on the server

 - First, we move the state from the react component to code running on
   our server machine.
 - We can hide information by simply not sending it.
 - Instead, we send the minimum information nessiary for the display to be
   rendered and users to be able to interact.
 - User actions are sent to the server, and the updated minimum information
   (the user's "view" of the current state) is sent back.
 - The server code is now responsible for handling the game rules, so cheating
   is harder.
 - The server code is responsibile for maintaining data consistency - that's
   still a problem in a multi-user app, but the server code is in a position to
   try to solve it in some reasonable way.
 - We're not going to do multiplayer quite yet.

### Concept: Websockets & Phoenix Channels

Traditional HTTP provides request-response semantics. 
  
  - The browser makes a request
  - The server sends back response.

One problem:

 - Server can't initiate a message to browser.

We could build Hangman with traditional HTTP requests. But if we wanted to add
multiplayer, then there would be no way to show actions by other players without
polling.

Solution: Websockets

Idea: A persistent stream layer (basically TCP) on top of HTTP. 

 - Client sends an "Upgrade: websocket" header in initial TCP request.
 - Server sends a "Upgrade: websocket" response header.
 - After that, the socket stays open as a websocket.
 - After that, either party can send data at any time.

Since a websocket connection is basically just a TCP connection, you need
to have a protocol on top of it. Phoenix gives us one: Phoenix Channels.

## Adding a Channel to Hangman

```
$ mix phx.gen.channel games
```
  
Add to .../channels/user_socket.ex:

```
  channel "games:*", HangmanWeb.GamesChannel
```

Open .../js/socket.js - scan through.

 - userToken for transferring session from cookie -> websocket.
 - Move the channel join stuff over to app.js
 - Set the channel to join as "games:default"

Add to head in .../layout/app.html.eex

```
    <script>
      window.userToken = "TODO";
    </script>
```

Add a route, to .../hangman_web/router.ex

```
    get "/game/:name", PageController, :game
```

Add a function to page_controller.ex

```
  def game(conn, %{"name" => name}) do
    render conn, "game.html", name: name
  end
```

Copy the index.html.eex page to game.html.eex

Edit index.html.eex to this:

```
<div class="row">
  <div class="column">
    <h1>Roll a Die</h1>
    <p><button id="roll-button">Roll</button></p>
    <p id="roll-output"></p>
  </div>
</div>
```

Edit app.js to add:

```
function roll_init() {
  $('#roll-button').click(() => {
    channel.push("roll", {}).receive("roll", msg => {
      console.log("roll", msg);
      $('#roll-output').text(msg.roll);
    });
  });
}

$(() => {
  roll_init();
```

Edit games_channel.ex:

```
  def join("games:" <> name, payload, socket) do
    if authorized?(payload) do
      {:ok, %{"join" => name}, socket}
    else
      {:error, %{reason: "unauthorized"}}
    end
  end

  def handle_in("roll", payload, socket) do
    resp = %{ "roll" => :rand.uniform(6) }
    {:reply, {:roll, resp}, socket}
  end
end
```

## Now lets serverize Hangman

Update index.html.eex:

```
<div class="row">
  <div class="column">
    <h1 id="index-page">Join a Game</h1>
    <p><a href="/game/demo">Join "demo"</a></p>
  </div>
</div>
```

page_controller.ex:



game.html.eex:

```
<script>
 window.gameName = "<%= @name %>";
</script>

<div class="row">
  <div class="column">
    <h1>Hangman Game: <%= @name %></h1>
    <div id="root">
      <p>React app not loaded...</p>
    </div>
  </div>
</div>
```

app.js:

```
import socket from "./socket";
import game_init from "./hangman";

function start() {
  let root = document.getElementById('root');
  if (root) {
    let channel = socket.channel("games:" + window.gameName, {});
    // We want to join in the react component.
    game_init(root, channel);
  }
}

$(start);
```

Write attached code:

 - First, the abstract game: Hangman.Game (games.ex)
 
games\_channel.ex:

```
  alias Hangman.Game

  def join("games:" <> name, payload, socket) do
    if authorized?(payload) do
      game = Game.new()
      socket = socket
      |> assign(:game, game)
      |> assign(:name, name)
      {:ok, %{"join" => name, "game" => Game.client_view(game)}, socket}
    else
      {:error, %{reason: "unauthorized"}}
    end
  end

  def handle_in("guess", %{"letter" => ll}, socket) do
    game = Game.guess(socket.assigns[:game], ll)
    socket = assign(socket, :game, game)
    {:reply, {:ok, %{ "game" => Game.client_view(game)}}, socket}
  end
```

Then update the JSX code:

 - JSX code becomes pure UI: hangman-server.jsx


