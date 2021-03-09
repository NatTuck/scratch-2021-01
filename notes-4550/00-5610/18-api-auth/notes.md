---
layout: default
---

## First Thing

Project questions?

## Web APIs

Two cases:

 - Browser to Server comms. This is how the Lens SPA worked.
 - Server to Server comms. This is the topic today.

## REST w/ JSON Endpoints

We looked at RESTful routes early in the semester. Probably the
most common API style today is RESTful routes + JSON.

 - Show reddit.com/r/aww
 - Show reddit.com/r/aww.json

With REST-style paths, we have a reasonable interface for programatically
accessing and manipulating resources.

I'd like to print out titles from /r/aww

Create a new elixir app:

```
$ mix new aww_titles --sup
```

Add some deps to mix.exs

```
  {:httpoison, "~> 1.5"},
  {:jason, "~> 1.1"},
```

Now "mix deps.get", and let's test this out in "iex -S mix".

```
iex>  resp = HTTPoison.get!("https://www.reddit.com/r/aww.json")
iex>  data = Jason.decode!(resp.body)
```

Here's some code.

```
  # Add to aww_titles.ex 
  def reddit_list(sub) do
    resp = HTTPoison.get!("https://www.reddit.com/r/#{sub}.json")
    data = Jason.decode!(resp.body)
    data["data"]["children"]
  end

  def get_titles(sub) do
    xs = reddit_list(sub)
    Enum.map xs, fn x ->
      x["data"]["title"]
    end
  end
```

Reddit API:

https://github.com/reddit/reddit/wiki/API

 - API Terms
 - OAuth Required

## Authentication

### Tokens

If you have an account with a service and are trying to authenticate to use
that account, you'll frequently get a token.

This token can either be a cryptographic thing like a Phoenix.Token, or it can
just be a random value that basically acts as a password. As the client these
look and work the same.

There's one extra case: JWTs. These are standardized or cryptographically
signed tokens that can be interpreted by the carrier.

Send this token with each request, either in some sort of API-KEY header or
as the password for some sort of HTTP auth.

### Cryptographic Keys

Same use case as Tokens. We're trying to authenticate ourselves to the API.

In this case we use cryptographic keys instead of a token, which is somewhat more
secure but requires a bit more work.

Examples:

 - TLS Client Keys
 - HMAC
 - RSA signature

### OAuth2

Sometimes an app that uses an API needs to be able to get access to
a user's account on the remote service. 

OAuth2 is a protocol that allows a user to delegate access to their account
to a third party service.

To a user, it looks like this:

 - We load the site we want.
 - We click an "authenticate with Github" (or other API provider) link.
 - We get an "Some site wants access to your Github Account, Ok?"
 - Clicking OK gets us back to the original site, which now has access to our
   Github account.
 
To the app it looks like this:

 - We send the user off to Github.
 - They come back on a GET request including a single use code.
 - We POST to Github with that code, and get back a token.
   * The POST includes your client identifier and a client secret to
     authenticate your app to the service.
   * Your app identity may be used to bill you, so you want to be careful
     not to leak your client secret to the user.
   * If you want free stuff, check mobile app downloads for OAuth2 client
     secrets.
 - We can use that token in API requests to access resources private to the user.


## Web API Formats

GET

 - JSON
 - XML
 - Random nonsense

POST

 - Form data
 - JSON
 - XML
 - Random nonsense

## A Closer Look at OAuth2

We're going to look at an example app that uses Oauth2 and Github

https://github.com/NatTuck/bug_bot

 - Check out
 - Create the DB user
 - mix ecto.reset
 - (cd assets && npm install)
 - can't run the server quite yet

Setting this app up to use the Github API:

 - Github profile photo
 - Settings
 - Developer Settings
 - New OAuth App
 - Fill out the form

This gets us to the app page, where we get two things:

 - Client ID (public)
 - Client Secret (private)

Load config/dev.exs

 - Scroll to Github config.
 - It's set up to configure with env variables.
 - This is a common way to handle secrets.
   - In dev, we just stick the values in our ~/.bashrc
   - In production, we can either create a wrapper script
     to launch our app or stick these values directly
     in our systemd service file.
 - In this case, REDIRECT_URI is our callback URL.

Run the app:

 - mix phx.server
 - load up localhost:4000
 - Log in with github
 - Log out
 - Log in a again
 - Go to Github and deauth (Settings / Applications / OAuth Apps)

Developing locally:

 - You can enter the URL and callback URL as localhost.
 - This will allow you to oauth on the local machine.

Let's take a look at this app

lib/bug\_bot\_web/router.ex

 - get "/auth/callback" is the callback URL
 - This is where the provider redirects when authenticating.

.../controllers/auth\_controller.ex

 - Authenticate page redirects to external authentication provider.
   - authorize\_url! says where to auth to
   - We pass in a "scope" parameter to request access to repos.
 - Delete is the standard log out plan.
   - This just drops the session rather than leaving a cookie with user=nil
 - get\_token! does a HTTP post to trade the code for an auth token (see below)
 - get\_user! does an API call using the shiny new token (see below)
 - The OAuth2.Client module lets us do authenticated HTTP reqs.

lib/bug\_bot/auth.ex

 - This should definitely be in ./lib (or ./lib/appname/ in 1.3)
 - Everything's in "@config"

web/templates/page/index.html.eex

 * Show the login with github link.
 
web/templates/page/welcome.html.eex

 - Logged in user, show info and profile photo.

## Issues Demo

Go to /issues

This uses the Github API to access issues.

 - Show issue list.
 - Go to git repo, show that the issues are the same.

Hit "New Issue" button, enter issue.

 - Issue shows up in list.
 - Issue shows up in list on Github.
 - New issue on Github attributed to me - because the app has
   access to act as me via OAuth.

lib/bug\_bot/issues.ex

 - Same format as any other context module.
 - It's just backed by the Github API, not the database.
 
## Other API Complications

Some APIs, like for Paypal, make "postback" requests. This requires developing
on a machine that can handle inbound HTTP connections from the internet.

 - This may require developing on your VPS.
 - Don't develop in production mode.
 - Open port 4000 and set your initial callback URL to be
   the dev server on port 4000 - this lets you use "mix phx.server"
   or "iex -S mix phx server" normally.
 - Don't develop as your production user. This is a good use for your
   non-root personal login account (e.g. log in as yourself)
 - See if your editor supports editing remote files over SSH. Emacs does.

# Accessing APIs from the browser

This isn't applicable to the project, but it's still worth covering.

## Token Based Auth

 - You can't securely use token-based auth from the browser.
 - Browser code has makes the request, so browser code needs to have
   the token.
 - If the browser code has the token, then the user has the token, and they
   can make arbitrary requests using your account.
 - If there's a token that authenticates *you*, then you need to proxy the requests
   through your server so your server can add the token.

## Browser OAuth2 for single page apps.

OAuth2 for browser requests is similar to the flow for server requests, except
there can no longer be a client key. OAuth2 APIs that require a client key are
not appropriate for browser use, since that would leak *your* key to the user
and they could make arbitrary requests authenticated as your app.

The revised flow is:

 * User clicks a link to authenticate with Provider.
 * The user approves the request on a page on the Provider's site.
 * The user is redirected back to your app with a code.
   * This requires a full page load, even for an SPA.
 * Your in-browser JavaScript makes a POST to the Provider to exchange the
   code for an access token.
   * This still requires a client ID, identifying your app.
   * There is no client secret.
   * The Provider must have correctly configured CORS headers.

Using OAuth2 this way is dangerous for services where you are billed for usage.
Without the client secret being securely transmitted server <-> server, there's
no built in mechanism to prevent your app from being spoofed by users.

The sensible way for providers to treat this kind of authentication is simply as
something the user is doing with their account - only billing the user.

## The Same-Origin Policy and CORS

By default browsers only allow AJAX requests (via XMLHttpRequest or fetch) to
the same site. This is a security mechanism to prevent cross-site request
forgery attacks and other mischief.

But sometimes you *want* to allow cross-origin requests, so browsers implement a
mechanism called CORS (cross-origin resource sharing) to allow it.

It works as follows:

 * The "same site" means the same hostname, port, and protocol.
 * Simple requests: Cross-origin GET, HEAD, and some POST requests are allowed,
   as long as their are no weird headers, but the result is not delived to the
   JS code unless it includes appropriate CORS headers.
 * You can only post simple forms, just like if you had a form on your page and
   you used JavaScript to click the submit link.
 * Preflighted requests: Cross-origin requests that have side effects other than
   posting a simple form (e.g. POST, PUT, DELETE, ...) or have weird headers are
   preceeded by an OPTIONS request. The actual request is only made if the
   OPTIONS request returns appropriate CORS headers.

First, this means that you absolutely should not use GET requests for side
effects. You shouldn't anyway - browsers may get clever and preload links or
something - but this is another reason that's a bad idea.

But if you're building an API that wants to accept requests from external
domains, you'll need to set up CORS headers.

Note that this whole thing only applies to browser clients. Native / mobile apps
can make whatever HTTP requests they want.

Two cases:

 * You've split your "frontend" and "backend" code and they're hosted on
   as seperate sites.
 * You're building an API intended to be used by external sites with
   browser-based API clients.

## Flow for Simple Requests

JS code makes a request (GET "http://api.example.com/foo"), then:

Browser sends request, filling in the Origin header itself:

```
GET /foo HTTP/1.1
Host: api.example.com
Origin: http://www.example.com:8080
```

Server sends response, allow all:

```
HTTP/1.1 200 OK
Access-Control-Allow-Origin: *
Content-Length: 100

...
```

Alternatively, server sends response specific to the request:

```
HTTP/1.1 200 OK
Access-Control-Allow-Origin: http://www.example.com:8080
Vary: Origin
Content-Length: 100

...
```

The browser will only return the result to the JS code if the Origin it sent
matches the Access-Control-Allow-Origin header it got back.


## Flow For Preflighted Requests

JS code makes a request:

```
    url: "http://api.example.com/foo", 
    method: 'POST', 
    headers: {"x-auth": "xxx"},
    body: "..."
```

First, the browser will make a pre-flight OPTIONS request to the server:

```
OPTIONS /foo HTTP/1.1
Host: api.example.com
Origin: http://www.example.com:8080
Access-Control-Request-Method: POST
Access-Control-Request-Headers: x-auth
```

The server will respond:

```
HTTP/1.1 204 No Content
Access-Control-Allow-Origin: *
Access-Control-Allow-Methods: POST, GET, OPTIONS
Access-Control-Allow-Headers: x-auth
```

Only if the server responds with headers to allow the origin, method, and
headers will the browser follow through with the actual request. The response to
the actual request still needs an Access-Control-Allow-Origin header for the
browser to return the result to the JS code.

There are generally packages for server-side frameworks to support CORS headers.
In Phoenix, this is a good use of a Plug, and that's what the common Hex package
gives you.

Reference: https://developer.mozilla.org/en-US/docs/Web/HTTP/CORS

