
## Homework Questions?

## Review Hangman w/ Channel State

 - Review: Hangman.js
 - Review: socket.js - note state caching
 - We completed the server-side code for Hangman.
 - Review: GamesChannel
 - Review: Game module.
 - Add "crash on z" feature to Game module's
   guess function.
   
```
    def guess(game, "z") do
      raise "invalid letter"
    end
```

### Setup for Deployment

Once per app, optional:

```
hangman$ mix release.init
```

This creates a "rel" directory with creates some config files that we can use to
modify are releases in ways that matter for more complicated situations.

Once per app, required:

 - In ```config/prod.secret.exs```
 - Uncomment this line: ```config :hangman, HangmanWeb.Endpoint, server: true```
 - Else no web server, app won't work.

Also:

 - In ```config/prod.exs```
 - ```url: [host: "hangman.ironbeard.com", port: 80],```
 - This should match the *external* host and port.

Now, Let's follow the deploy guide to write a deploy script.

 - https://hexdocs.pm/phoenix/
 - Section is DEPLOYMENT => Deploying with Releases

Time to worry about secrets.

 - Secrets are things like passwords and crypto keys that your app
   needs to work but shouldn't be published.
 - There should *never* be secrets in your git repo.
 - That includes private repos - your project may have contributors that
   shouldn't know your AWS API key or whatever.
 - For this app, we've got one secret: ```SECRET_KEY_BASE```
 - If we're running our deploy scripts on the server, we can generate
   this on our first deploy in our deploy script.

```bash
#!/bin/bash
# This is deploy.sh

export MIX_ENV=prod
export PORT=4801
export SECRET_KEY_BASE=insecure

mix deps.get --only prod
mix compile

CFGD=$(readlink -f ~/.config/hangman)

if [ ! -d "$CFGD" ]; then
    mkdir -p $CFGD
fi

if [ ! -e "$CFGD/base" ]; then
    mix phx.gen.secret > "$CFGD/base"
fi

SECRET_KEY_BASE=$(cat "$CFGD/base")
export SECRET_KEY_BASE

npm install --prefix ./assets
npm run deploy --prefix ./assets
mix phx.digest

mix release
```

Now we need a start script to test with.

```bash
#!/bin/bash

# Could factor some of this out into an env.sh
# to share with deploy.sh
export MIX_ENV=prod
export PORT=4801

CFGD=$(readlink -f ~/.config/hangman)

if [ ! -e "$CFGD/base" ]; then
    echo "Need to deploy first"
    exit 1
fi

SECRET_KEY_BASE=$(cat "$CFGD/base")
export SECRET_KEY_BASE

_build/prod/rel/hangman/bin/hangman start
```

### Actually Deploy

 - Connect to server
 - Create new "hangman" user; ssh-copy-id
 - scp over the hangman directory
 - run deploy
 - copy nginx config from draw or practice
 - test with start run manually, show log
 - copy systemd config from draw or practice

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

```
iex> {:ok, pid} = Agent.start_link(fn -> 0 end, [])
iex> Agent.update(pid, fn x -> x + 1 end)
iex> Agent.get(pid)
```

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
 
## Overflow: Feature Cleanup

 - Implement lives and bad guesses display.

