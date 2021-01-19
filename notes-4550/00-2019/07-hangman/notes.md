+++
title = "./07-hangman/notes.md"
+++

## Homework questions?

## Hangman in the Browser

 - Git repo: https://github.com/NatTuck/hangman-2019-01/
 - This section goes from branch "master" to "01-31-channel-hangman"

 - Remove the TODO header from lib/hangman\_web/templates/page
 - Move todo.jsx to hangman.jsx
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

### Concept: Websockets & Phoenix Channels

Traditional HTTP provides request-response semantics. 
  
  - The browser makes a request
  - The server sends back response.

One problems:

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

Edit games_channel.ex

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










