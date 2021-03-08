---
layout: default
---

## Project Questions?

## Where are we?

 - We're building a photo sharing app: Lens

Last time, we:

 - Created a new Phoenix app.
 - Set up the database
 - Installed Bootstrap
 - Set up for deployment and did a test deploy.
 - Created our first resource: Users
 
Bootstrap Styling

 - We're using bootstrap to make our pages not look ugly.
 - The default tables we got are kind of ugly.
 - Bring up http://getbootstrap.com
 - Show Documentation / Content / Tables
 - Scroll down to striped rows.
 - Show lib/lens_web/templates/user/index.html.eex 
   - Added "table table-striped" classes.
 - Show new index page in browser.
 - Show new form page in browser.
 - Get rid of the awful grey background.
 - In layout.html.eex, switch navbar to "navbar-dark bg-dark".

## Adding Photos

 * https://github.com/NatTuck/lens/
 * Start Branch: 2-users
 * End branch: 3-photos

We want a photo sharing app, so we need to store / manage photos.

Photos are a resource, so we'll generate a new scaffold for our resource.

```
$ mix phx.gen.html filename:string desc:text user_id:references:users
```

 * "string" is a short string (usually max 255 chars)
 * "text" is a potentially long string
 * The user_id field is an integer referencing the "id" field in the users
   table. The DB will enforce this relationship.

Next, we want to edit the migration file.

 * No field can be null.
 * What happens to a photo when we delete a user?
   * Show Ecto.Migration docs
   * Probably :delete_all makes sense here.
 * The index on user_id makes lookups fast when we ask for photos belonging
   to a specfic user.

Then:

 * Run the migration: ```mix ecto.migrate```
 * Add the router line to the router.
 * We changed the router, so we need to restart our server.

Clean up the templates:

 * Add "table table-striped" in index.

```
  <div class="form-group">
    <%= label f, :filename %>
    <%= text_input f, :filename, class: "form-control" %>
    <%= error_tag f, :filename %>
  </div>
```

We need to deal with photo uploads, but we have a bigger problem first. Let's
try to create a photo just by filling out the form.

Problem: User Sessions

 - Photos belong to users.
 - When you create a photo, it should have your user_id
 - We need to be able to log in to the site as a user.

## User sessions

We have two basic operations:

 - Log in
 - Log out

One convenient way to look at this is to treat sessions as a resource, kind of
like users or photos. Of our seven "RESTful" paths, three of them are useful
here:

 - new: Our login form
 - create: Actually log in
 - delete: Log out

We're going to use cookies to store sessions, so we don't need a DB table. We
also don't need most of the routes of a normal resource. So it'll be faster to
just write our resource from scratch rather than using the code generator and
removing stuff.

lib/lens\_web/controllers/session\_controller.ex:

```
defmodule LensWeb.SessionController do
  use LensWeb, :controller

  def new(conn, _params) do
    render(conn, "new.html")
  end

  def create(conn, %{"email" => email}) do
    user = Lens.Users.get_user_by_email(email)
    if user do
      conn
      |> put_session(:user_id, user.id)
      |> put_flash(:info, "Welcome back #{user.email}")
      |> redirect(to: Routes.page_path(conn, :index))
    else
      conn
      |> put_flash(:error, "Login failed.")
      |> redirect(to: Routes.page_path(conn, :index))
    end
  end

  def delete(conn, _params) do
    conn
    |> delete_session(:user_id)
    |> put_flash(:info, "Logged out.")
    |> redirect(to: Routes.page_path(conn, :index))
  end
end
```

Create a view module in views/session\_view.ex:

```
defmodule LensWeb.SessionView do
  use LensWeb, :view
end
```

Add our new resource to our router:

```
resources "/sessions", SessionController, 
  only: [:new, :create, :delete], singleton: true
```

Singleton means each user only sees one session, so the delete path is just
"/session/delete" rather than "/session/delete/:id".

Create our form in lib/lens\_web/templates/sessions/new.html.eex:

```
<%= form_for @conn, Routes.session_path(@conn, :create), fn f -> %>
  <div class="form-group">
    <%= label f, :email %>
    <%= text_input f, :email, class: "form-control" %>
    <%= error_tag f, :email %>
  </div>

  <div>
    <%= submit "Log In", class: "btn btn-primary" %>
  </div>
<% end %>
```

Finally, create our helper function in users.ex:

```
  def get_user_by_email(email) do
    Repo.get_by(User, email: email)
  end
```

Now we can create a session, but it doesn't really do anything.

Whenever a user loads any route in our app we want to fetch their user record
from the database and store it in our conn for use during the request.

To do that, we'll use a Plug.

lib/inkfish\_web/plugs/fetch\_current\_user.ex:

```
defmodule LensWeb.Plugs.FetchCurrentUser do
  import Plug.Conn

  def init(args), do: args

  def call(conn, _args) do
    user = Lens.Users.get_user(get_session(conn, :user_id) || -1)
    if user do
      assign(conn, :current_user, user)
    else
      assign(conn, :current_user, nil)
    end
  end
end
```

That means we need a helper function in users.ex:

```
  def get_user(id), do: Repo.get(User, id)
```

And we want to call the plug in our router, in the browser pipeline:

```
  pipeline :browser do
    ...
    plug :fetch_session
    plug LensWeb.Plugs.FetchCurrentUser
```

Performance note:

 * This is a signficant addition to our application: Every page load now does
   a DB query.
 * Without any DB queries, Phoenix can usually respond to an HTTP request really
   fast: it can sometimes work in under 100 microseconds.
 * With a DB query, we're looking at times more like 5 milliseconds.
 * This is still plenty fast, and the DB query isn't really avoidable, but
   it's useful to keep in mind this difference.

Now that we have our user, we can update our pages.

In layouts/app.html.eex:

```
        <!-- in header, make middle column width 4 -->
        <div class="col-4">
          ...

        <!-- in header, add right column -->
        <div class="col-4 text-light">
          <%= if @current_user do %>
            User: <%= @current_user.name %>
            (<%= link("Logout", to: Routes.session_path(@conn, :delete),
              method: :delete) %>)
          <% else %>
            <%= link("Login", to: Routes.session_path(@conn, :new)) %>
          <% end %>
        </div>
```

Now we need users to test with. We could just create users manually, but instead
we're going to add some standard test data for our app by creating a "seeds"
file for the DB.

In priv/repo/seeds.exs:

```
alias Lens.Repo
alias Lens.Users.User

Repo.insert!(%User{name: "Alice", email: "alice@example.com"})
Repo.insert!(%User{name: "Bob", email: "bob@example.com"})
```

We can run this with:

```
  $ mix run priv/repo/seeds.exs
```

Or we can reset our database to contain *only* the seed data with:

```
 $ mix ecto.reset
```

Demo our users:

 * Show the user list at /users
 * Demonstrate logging in as Alice.

## Creating Photos

Now that we have user sessions, we can work on creating photos.

First, let's add the correct user_id to newly created photo records.

In photo\_controller.ex:

```
  def create(conn, %{"photo" => photo_params}) do
    photo_params = Map.put(photo_params, "user_id", conn.assigns[:current_user].id)
```

That's not enough, we need to change our schema too:

In lib/lens/photos/photo.ex:

```
  def changeset(photo, attrs) do
    photo
    |> cast(attrs, [:filename, :desc, :user_id])
    |> validate_required([:filename, :desc, :user_id])
  end
```

Now for the fun part, uploading actual photos.

.../photo/form.html.eex:

```
<%= form_for @changeset, @action, [multipart: true], fn f -> %>
  ...
  <div class="form-group">
    <%= label f, :photo_upload %>
    <%= file_input f, :photo_upload, class: "form-control btn btn-file" %>
    <%= error_tag f, :photo_upload %>
  </div>
```

Let's see what we got:

```
  def create(conn, %{"photo" => photo_params}) do
    photo_params = Map.put(photo_params, "user_id", conn.assigns[:current_user].id)

    IO.inspect(photo_params["photo_upload"]
```

Three fields in the upload struct:

 * Content-type: Reported by browser, not 100% trustworthy.
 * Filename: As the user saw it.
 * Path: Where the file is temporarily stored on disk on the server.

So we want to copy the file somewhere where we can find it later given our
photo record read from the DB.

Our app will have a user account, so we can put our app in the user's home
directory. We also want to develop, so we should put it somewhere reasonable
for development.

A good base is: ~/.local/data

We should separate by app for development: ~/.local/data/lens

Separate by resource, in case there are other uploads later: ~/.local/data/lens/photos

Then we want to distinguish per upload, since two users may upload files with
the same name.

 * Obvious solution: DB row id
 * Problem: We haven't saved the row yet, and we'd like to have the file saved
   before we save the DB record for consistency.
 * Solution: Random UUID. A 128-bit random number will be reliably unique.

Let's add some logic to lib/lens/photos/photo.ex:

```
  def changeset(photo, attrs) do
    photo
    |> cast(attrs, [:desc, :user_id, :photo_upload])
    |> validate_required([:desc, :user_id, :photo_upload])
    |> generate_uuid()
    |> save_photo_upload()
  end

  def make_uuid() do
    :crypto.strong_rand_bytes(16)
    |> Base.encode16
  end

  def generate_uuid(cset) do
    if get_field(cset, :uuid) do
      cset
    else
      put_change(cset, :uuid, make_uuid())
    end
  end

  def save_photo_upload(cset) do
    up = get_field(cset, :photo_upload)
    uuid = get_field(cset, :uuid)
    if up do
      dir = photo_upload_dir(uuid)
      File.mkdir_p!(dir)
      File.copy!(up.path, Path.join(dir, up.filename))
      put_change(cset, :filename, up.filename)
    else
      cset
    end
  end

  def photo_upload_dir(uuid) do
    base = Path.expand("~/.local/data/lens/photos/")
    Path.join(base, uuid)
  end
```

We need to add the uuid field to the DB.

 * For now, it's safe to edit the migration and reset the DB.
 * Once we have production data, we'd need a new migration.

Edit priv/repo/*\_create\_photos.exs:

```
  add :uuid, :string, null: false
```

```
 $ mix ecto.reset
```

One more change: We're getting filename from the upload, so we should remove it
from the form.

photo/form.html.eex: Remove filename field

Now we can upload files. They really get copied to the correct directory.

```
 $ ls ~/.local/data/lens/photos
```

Now our problem is that photos don't show up on the photo/show page.

To fix that, first we need a way to request the photo. We still want the
existing show page to show photo + metadata, so we'll need a new path to
request the photo itself.

In router.ex:

```
    get "/photos/:id/file", PhotoController, :file
```

In photo_controller.ex:

```
  def file(conn, %{"id" => id}) do
    photo = Photos.get_photo!(id)
    dir  = Photo.photo_upload_dir(photo.uuid)
    data = File.read!(Path.join(dir, photo.filename))
    conn
    |> put_resp_header("content-type", "image/jpeg")
    |> put_resp_header("content-disposition", "inline")
    |> send_resp(200, data)
  end
```

Now we can add the image to templates/photo/show.html.eex:

```
<div>
  <%= img_tag(Routes.photo_path(@conn, :file, @photo)) %>
</div>
```

## Adding owner names to photos

In templates/photo/show.html.eex:

```
  <li>
    <strong>User:</strong>
    <%= @photo.user.name %>
  </li>
```

Unfortunately, user name is in a different table from the other photo
metadata, so we'll need to update our code to fetch that data.

First, we need to tell the Photo schema about the users table.

In photos/photo.ex:

```
    # Remove the :user_id field, replace with
    belongs_to :user, Lens.Users.User
```

Now we need to fetch the user when we fetch the photo.

In lib/lens/photos.ex:

```
  # Replace get_photo with:
  def get_photo!(id) do
    Repo.one! from p in Photo,
      where: p.id == ^id,
      preload: [:user]
  end
```

