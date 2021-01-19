+++
title = "./08-server-state/notes.md"
+++

## Homework Questions?

## Server-side Hangman

https://github.com/NatTuck/hangman-2019-01 (branch: 01-31-channel-hangman)
 
 - We completed the server-side code for Hangman.
 - Review: Game module.
 - Review: GamesChannel
 - Complete Hangman with hangman-server.jsx

### Setup for Deployment

 - Add elixir module: distillery.
   - in mix.exs: {:distillery, "~> 2.0"}
   - mix release.init
 - Follow deploy guide to create a deploy script.
 - Worry about secrets.
   - config/prod.secret.exs - Copy manually from dev machine.
   - rel/config.exs - Random cookie code; will break if we try
         to run in a cluster.
 - There should *never* be secrets in your git repo.

### Generate Secrets at Deploy Time

This snippet of code is useful for managing secrets in config/prod.exs and
rel/config.exs

Feel free to use this in your projects with the attribution comment. This
assumes that your build machine and your production server are the same. That
may not be a good idea for larger deployments and stops working once you have
two web servers.

```
get_secret = fn name ->
  # Secret generation hack by Nat Tuck for CS4550
  # This function is dedicated to the public domain.
  base = Path.expand("~/.config/phx-secrets")
  File.mkdir_p!(base)
  path = Path.join(base, name)
  unless File.exists?(path) do
    secret = Base.encode16(:crypto.strong_rand_bytes(32))
    File.write!(path, secret)
  end
  String.trim(File.read!(path))
end

## In config/prod.exs
secret_key_base: get_secret.("key_base");
...
password: get_secret.("db_pass") # Manually make file match password

## In rel/config.exs
set cookie: String.to_atom(get_secret.("dev_cookie"))
```

## Problem: State loss on crash

### Show 'z' crashes

 - iex -S mix phx.server
 - 'z' crashes
 - :observer.start()
 - Just look at process tree.
 - See if we can figure out which process is crashing.
   - (Maybe not).

### Browser-Only

e.g. Hangman from last week.

A single-page / single component React app.

 - One state as value object.
 - Pure function renders state to web page.
 - In last week's version, update logic was in browser.

- Client-Side Only like this is plan A for state.

 - Pro: Pure JS
 - Pro: Free for Server
 - Con: User actually controls what the code does
 - Con: State is stuck in one user's browser
 - Con: State is lost if user navigates away from page.

### Move State to Server

Plan B: Server-Side

 - Pro: We have access to the data
 - Pro: We control the app logic
 - Pro: Can have multiple user sessions interact
 - Con: More complex
 - Con: Computation is on server

But how to manage the state on the server?

Can't easily use HTTP requests.

 - HTTP is stateless.
 - Cookies add state, but stored client-side.
 - Traditionally we can have cookie index into a
   database. That works, but is not great for heavy 
   interactivity like a game.

Plan B.1: Store State in Channel

e.g. Hangman from Tuesday

   - Simple concept
   - Specific to that connection
   - Lost on disconnect
   - Problem: Homework says don't lose it on disconnect

Plan B.2: Outside of Channel

   - Where do we put it?
   - Unfortunately, this is a hard problem in general.
   - Fortunately, this is a problem that has a bunch of good
     solutions in Erlang / Elixir.

Let's start looking at strategies for managing state.

## Shared State with Procs

 - shared-state.ex

## Built-in Mechanism: Agent

 - Spawn a process to hold our state.
 - Agent is a built-in module.
 - It holds the state and we send it messages:
   - Get current state
   - Update state by applying function
 - All access is serialized, but any process can send it message.
 - Logically kind of like a single object protected by a mutex
   in C or Java.
 - No data races or deadlock, but no parallel speedup either.

## Back to Hangman

Where are we?

- We have a Phoenix app with a channel.
   - Server maintains game state.
   - Sends a "view object" to client.
   - Browser makes "view object" its local state, which triggers
     react re-render.
   - Events on browser send message to server, server updates state,
     sends new "view object", browser rerenders.
   - Visiting page causes browser to connect to channel.
   - Channel connection is process on server that surivives until
     the browser disconnects.
   - Game state is associated with channel process.

Keep going:

 - Currently crashes when you press "z"
 - State is stored in channel, so we lose it.

## Hangman with Backup Agent

 - https://github.com/NatTuck/hangman-2019-01
 - Start branch: 01-31-channel-hangman
 - End branch: 02-04-backup-agent

We're going to add an agent to our hangman game to back up the
game state.

lib/hangman/backup_agent.ex:

```
defmodule Hangman.BackupAgent do
  use Agent

  # This is basically just a global mutable map.
  # TODO: Add timestamps and expiration.

  def start_link(_args) do
    Agent.start_link(fn -> %{} end, name: __MODULE__)
  end

  def put(name, val) do
    Agent.update __MODULE__, fn state ->
      Map.put(state, name, val)
    end
  end

  def get(name) do
    Agent.get __MODULE__, fn state ->
      Map.get(state, name)
    end
  end
end
```

Start this process with the app.

lib/hangman/application.ex, in children list:

```
   ...
   # Starts a worker by calling: Hangman.Worker.start_link(arg)
   # {Hangman.Worker, arg},
   Hangman.BackupAgent,
   ... 
```

Modify the channel to make backups:

```
  def join("games:" <> name, payload, socket) do
    if authorized?(payload) do
      game = BackupAgent.get(name) || Game.new()
      socket = socket
      |> assign(:game, game)
      |> assign(:name, name)
      BackupAgent.put(name, game)
      {:ok, %{"join" => name, "game" => Game.client_view(game)}, socket}
    else
      {:error, %{reason: "unauthorized"}}
    end
  end

  def handle_in("guess", %{"letter" => ll}, socket) do
    name = socket.assigns[:name]
    game = Game.guess(socket.assigns[:game], ll)
    socket = assign(socket, :game, game)
    BackupAgent.put(name, game)
    {:reply, {:ok, %{ "game" => Game.client_view(game)}}, socket}
  end
```

 - iex -S mix phx.server
 - :observer.start()
 - Look at state for the BackupAgent process.

 - New problem: No way to get rid of z word.
















