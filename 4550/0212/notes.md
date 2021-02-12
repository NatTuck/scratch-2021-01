
# Homework Questions: Elixir Practice

# Web Dev: Server-Side State (part 2 of ??)

## Hangman: Where are we currently?

 - Where does socket.js fit in?

## Deploying

Before deploying the first time:

 - Make sure "server: true" in config/prod.secret.exs
 - Set hostname in config/prod.exs
 
# Secrets

Session cookies are cryptographically signed.

 - Server sends cookie "you are logged in as user_id = 7"
 - Browser sends back cookie on every request
 - Server checks cryptographic signature

The key used for signatures is derived from ```SECRET_KEY_BASE```

So if the user knows that value, they can forge session cookies.

Secrets, at least ones we care about, should never be published,
including committing them to source control. That includes even
private source control.

Other secrets that apps have include:

 - Database passwords
 - API keys to external and internal APIs.
 - etc
 
## State Handling: Options

Plan A: Browser Only

 - Pro: 100% in-browser JS code
 - Pro: No load on server
 - Con: User actually controls what the code does
 - Con: State is stuck in one user's browser
 - Con: State is lost if user navigates away
   - Or, with local storage, if they reinstall browser

Plan B: State on server - Elixir/Phoenix assumed

Plan B.1: Store State in Channel

 - Simple concept
 - Specific to that connection
 - Lost on disconnect
 
Plan B.2: Store state outside of channel

 - Where do we put it?
 - Unfortunately, this is a hard problem in general.
 - Fortunately, Erlang/Elixir have a bunch of possible
   solutions built in or easily constructed.

## Built-in mechanism: Agent

 - Default strategy in Erlang to store "global" state
   is to have a process dedicated to managing it.
 - Agent is a built-in module that manages this
   sort of process.
 - It holds the state and we send it messages:
   - Get the current state
   - Update the state by applying a function
 - All access is serialized, but any proces can
   send message.
 - Logially, like a single object protected by a mutex
   in C or Java.
   
## More advanced map options: dets / ets

 - ets is a per-process map that's slighltly more efficient
   than map
 - dets is ets, but stored on disk






















