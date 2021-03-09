
# Re-ordering cards by vote count

 - Best way: Rewrite this to use React
 
Raw Browser DOM API plan:

 - DOM .remove() on moved card
 - Iterate through cards
 - .insertBefore() if we find a card
   that our card should go before.
 - If we don't find a card to go before
   our card goes at the end: .appendChild()

# Questions on this Example?

# Homework Questions?

 - There is no "feed" in the requirements.
   - That doesn't mean you can't have one.
 - To show the list of events for the logged
   in user:
   - user has_many events through invites
   - my_events = user.events or map(user.invites, &1.event)
 - To login a user on signup.
   - User#create creates a session cookie.

# Twitter-style followers:

 - A user has many users. (called "followers") 
 - A user has many users. (called "people they're following")

# Invites are easier than that

 - An event has many users (who were invited)
 - An event has one user (the owner)
 
```
schema "monkeys"
    ...
    belongs_to :user, OurApp.Users.User
```

  - Creates a field on our schema, called "user"
  - Describes a foreign reference
    - A "user_id" field on monkeys references
      the "id" field on users.

```
schema "cars"
    ...
    belongs_to :owner, OurApp.Users.User
```
 
 - Foreign key field defaults to "owner_id",
   references the "users" table.
 - If things don't match up, we can specify
   options like foreign_key.

```
    car |> Repo.preload(:owner)
```

Where to preload?

 - Definitely in the context module (lib/our_app/cars.ex)
 - Maybe in the ```list_cars```, maybe in ```get_car!```
 - Or, maybe in a ```load_owner```.

There probably needs to be an "invites" table.

# Web APIs in General

Two cases:

 - Browser to server communication. Votes was
   an example of this.
 - Server to server communication.
   - Also, non-browser to server communication.

## REST w/ JSON endpoints

This is probably the most common API style today.

## Authenticating to an API

## Tokens

Authenticate you, the user, who also
probably developed the client app.

Example: 
 
 - Writing an Amazon AWS script.
 - Similarly, Vultr automation.

If you have an account with a service and
want to use that account, you'll frequently be
given a token.

Generally you pass the token with your requests
as a header. Basically it works like a password,
even if there's complicated crypto happening
in the background.

This is also what we did in the PhotoBlog example.

### Cryptographic Keys

Authenticate you, the user who
probably developed (or licensed) the client app.

Same use case as tokens.

Instead of getting a token from the service, we
sign our own token using our own cryptographic key.

If someone steals the token, they get less than
they would with a normal token - it may not even
work for them once.

Disadvantage: More complex for user.

### OAuth2

This is a way for a user to authenticate
themselves through a third-party API client.

The API client is *also* authenticated to the
API provider.

To a user, it looks like this (with GitHub
as the API provider):

 - We load the site we want.
 - We click on "authenticate with GitHub"
 - We get "Some site ants access to your
   GitHub account. Approve?"
 - Clicking Approve gets us back to the
   origional site, which now has access
   to (some of) our GitHub account.

To the app it looks like this:

 - We send the user off to Github.
 - They come back on a GET request with
   a single-use code.
 - We POST that code to GitHub and get
   back a token.
   - Post includes client identifier
     and client secret to authenticate
     the app to the service.
   - Your app identity may be used, e.g.,
     to bill you, so you don't want to
     leak the secrets or the token to
     the user.
 - We can then use the token in API reqests
   to access resources proviate ot the user.

## Postbacks

Some APIs involve "postbacks", where the
remote server will make a POST request to
your application server.

This is hard to test on a local development
workstation - you generally don't have a public
IP.

Testing this may require running your app in
development mode on your server.

Example: Paypal

## Same Origin Policy / CORS

JSON APIs, when accessed from the browser,
have some interesting rules.

Generally, JavaScript code can only do non-trivial
HTTP requests to the same origin: (protocol, host, port).

Non trivial means: GET, HEAD, POST (form data) with
no special headers.

If we want to POST JSON, or do stuff like PATCH or 
DELETE, we need to specify "CORS" headers on the 
server providing the API.

We have 30 seconds left, so this is a warning of
something to look up when you implment this.

https://github.com/NatTuck/bug_bot
