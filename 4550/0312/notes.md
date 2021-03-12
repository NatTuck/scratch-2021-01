

When doing a redirect, how do we know
where we last were so we can send the user
back there (e.g. on login).

 - Plan A: When we redirect to the login
   screen, include the current path asn
   an explicit GET parameter.
   - e.g. (/session/new?redirect_back=/user/show/7)
 - Plan B: Try to do it in JavaScript.
   - There's a History API, and if we can
     guess how many "backs" correctly, we can
     get back there.
 - Plan C: Put it in the session
   - Like Plan A, except with the session.

```
user_controller
  def show(...)
    user = ...
    if user_logged_in?() do
      render
    else
      redirect(to: Routes.new_session_path(...,
          Routes.user_path(conn, :show, user)
      possible shortcut: look at the fields of
      conn and see if you can write a current_path helper
      IO.inspect(conn)
    end
```

# Single Page Applications

 - We wrote our first game fully in-browser.
 - A single-screen app with server-side logic.
 - A traditional multi-page app with page loads
   for every transition.

A popular pattern in apps today:

 - A single page load.
 - Core app logic on the server.
 - A bunch of in browser code to fully control
   our app's UI, including simulation of multiple
   pages / screens.

Next: Start rewriting PhotoBlog this way.

Advantages to this plan:

 - We as the developers have full control over the 
   UI/UX, including transitions between application
   screens.
 - This can let us make the application feel faster
   by controlling when things load.

Disadvantages:

 - This won't work with unexpected browsers / 
   clients. It generally needs 100% support for
   modern JS.
 - You can control everything, but that means
   you're responsible for everything.
   - Back button
   - Displayed path
   - A whole bunch of other smaller things
   - This ends up taking a lot of work.

When we're doing this, we're moving from
"mix phx.gen.html", which autogenerates
entire resources at once, to doing everything
almost by hand (with some library support - but
the libraries don't enforce structure).

## CORS Headers

The browser will disallow HTTP requests from JavaScript
to our API unless our API sends the right headers with
the HTTP response.

We're just going to use an Elixir module that
gives us a plug.

## Complications with initial Users UI / React design

 - Not everything is going to fit on one screen.
   - Maybe we could do one screen per resource, but
     we're going to need to simulate mulitple pages.
 - Our state is a mess.
   - List of users is in root component.
   - Current user to be edited in Users component.
   - Having all the app-wide state in the root component
     is a decent pattern, but that requires threading
     state stuff through every nested component,
     and that's ugly.
 - When do we do loads from the server?
   - We could simulate a multi-page app and only load
     data for users when looking at the users list, but
     that's hardly better than a traditional app.
   - We could load everything up-front once.
   - We could try to predictively load things.
   - Do we ever want to load things while users are
     looking at them? If we trigger a rerender and
     move buttons as users might be trying to click
     them, then we deserve prison.
   - Explicit refresh button?
   - All of this depends on what exactly we're showing.
     - Is this a subset of a bigger data set?
     - Is the data on the server on the change, should
       the server push updates?










