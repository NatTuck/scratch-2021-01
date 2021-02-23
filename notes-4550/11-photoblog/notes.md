
## First: Homework Questions

## What was CRUD?

If we generalize enough, there's a whole class of apps that look basically like this:

 - We have some important data.
 - The app consists of looking at and modifying that data.

So we have a relational database, and our app is a UI that lets us do CRUD
operations: Create, Retrieve, Update, Delete

Things that we can apply CRUD operations to are commonly called "resources". The
Phoenix framework provides some tools to help us build resource-centric apps
using an app structure called REST, where CRUD ops are HTTP verbs.

## Starting the PhotoBlog App

 * Code at https://github.com/NatTuck/photo_blog
 * Progress goes sequentially through branches.


### branch: 01-bootstrap

Create the app:

```
### Confirm Phoenix version is 1.5.7
$ mix help phx.new 

## Create our app
$ mix phx.new photo_blog
```

Set up the database:

```
### Note: The DB dev/test password is not a secret.
$ pwgen 12 1
<password>
$ sudo su - postgres
pg$ createuser -d photo_blog
pg$ psql
postgres=# alter user photo_blog with password '<password>';
pg$ ^D
```

config/dev.exs and config/test.exs

```
...
config :photo_blog, PhotoBlog.Repo,
  username: "photo_photo",
  password: "<password>",
  database: "husky_shop_<env>",
  hostname: "localhost",
  pool_size: 10
```

Create the DB and run stock tests.

```
$ mix ecto.create
$ mix test
```

Clean up defaults; install Bootstrap

```
$ cd assets
$ rm static/images/phoenix.png
$ rm css/phoenix.css
$ npm install bootstrap
$ npm install jquery@3  popper.js@^1.16.1
```

Replace assets/css/app.scss contents with:

```
/* This file is for your main application css. */
@import "bootstrap";
```

Clean up the templates

Update ...templates/layout/app.html.eex to set body contents to:

```
    <div role="main" class="container">
      <%= if function_exported?(Routes, :live_dashboard_path, 2) do %>
        <p><%= link "LiveDashboard", to: Routes.live_dashboard_path(@conn, :home) %></li>
      <% end %>

      <%= if get_flash(@conn, :info) do %>
        <p class="alert alert-info" role="alert"><%= get_flash(@conn, :info) %></p>
      <% end %>
      <%= if get_flash(@conn, :error) do %>
        <p class="alert alert-danger" role="alert"><%= get_flash(@conn, :error) %></p>
      <% end %>

      <%= @inner_content %>
    </div>
```

Replace ...templates/page/index.html.eex with:

```
<div class="row">
  <div class="col">
    Hello, World
  </div>
</div>
```

### branch: 02-add-posts

```
$ mix help phx.gen.html
$ mix phx.gen.html Posts Post posts body:text
```

 - Add the resource line as instructed.
 - Edit the migration.

```
      add :body, :text, null: false
```

We want to think about null values for every single field in every single table.
Here we don't need them - if we need a blank post body we can use an empty
string.

```
$ mix ecto.migrate
```

Look through the generated resource files:

 * posts/post.ex - schema
   * Defines the data type.
   * Validates new items before inserting into DB.
 * posts.ex - context
   * Exposes the DB CRUD operations as functions.
   * This is an API that our web interface uses.
 * post\_controller.ex
   * Handles incoming requests.
   * Delegates to context to actually manipulate database.
   * Delegates to view / template to display HTTP response.
 * post\_view.ex
   * Just default code, which delegates to template.
 * templates/post/*
   * Each controller action has a template.

We've got a scaffold. Let's take a look [```http://localhost:4000/posts```]:

 - Index page; link to New Post
 - New post; Fill in form and post
 - Show page; Edit
 - Edit page; Back to List

Now let's look at the controller, and see how the 4 CRUD operations turn
into a resource with >4 actions in Phoenix.

Let's add some bootstrap styling.

post/index.html.eex:

```
<table class="table table-striped">
```

post/form.html.eex:

```
  <div class="form-group">
    <%= label f, :body %>
    <%= textarea f, :body, class: "form-control" %>
    <%= error_tag f, :body %>
  </div>

  <div>
    <%= submit "Save", class: "btn btn-primary" %>
  </div>
```

replace post/show.html.eex:

```
<p>
  <span><%= link "Edit", to: Routes.post_path(@conn, :edit, @post) %></span>
  <span><%= link "Back", to: Routes.post_path(@conn, :index) %></span>
</p>

<h1>Show Post #<%= @post.id %></h1>

<div>
  <%= @post.body %>
</div>
```

More scaffold demos:

 - Create a post.
 - Show in list.
 - Delete it.

Now that we have posts, let's create our feed of posts on the
index page.

 - Look at .../post_controller.ex
 - Update .../page_controller.ex to have @posts in the template.

Then we can replace page/index.html.eex:

```
<div class="row">
  <div class="col">
    <h1>Photo Feed</h1>
  </div>
</div>

<div class="row">
  <%= for post <- @posts do %>
    <div class="col-sm-4 card">
      <!--
      <img class="card-img-top" src="" alt="">
      -->
      <div class="card-body">
        <p class="card-text"><%= post.body %></p>
        <%= link "view", to: Routes.post_path(@conn, :show, post) %>
      </div>
    </div>
  <% end %>
</div>
```

## Interlude: RESTful Routes

```
$ mix phx.routes
```

The structure that Phoenix gives us for mapping HTTP routes to CRUD actions is
called REST. In REST, each resource has a path on the server, and we access
operations on the resource by sending HTTP requests to that path.

REST Example with a relational DB.

 - Our resource is "posts".
 - Instances are stored in a "posts" table in our DB.
 - The associated HTTP path is "/posts".
 - We support 5 operations.
   - List all posts: "GET /posts"
   - View one post (id = 10): "GET /posts/10"
   - Create a post: "POST /posts"
   - Update a post (id = 10): "PATCH /posts/10"
   - Delete a post (id = 10): "DELETE /posts/10"
 - There are two more paths for a traditional web UI.
   - Show the form for a new post: "GET /posts/new"
   - Show the form to edit a post (id = 10): "GET /posts/edit/10"

This isn't the only way to structure a web app, but it's pretty common today.

This is useful for:

 * Traditional web sites where users directly access these paths with
   their browser.
   * Guarantees that bookmarks work.
   * Allows caching to work as expected.
 * "Single Page Applications", where some JavaScript code - maybe a React UI -
   actually accesses the paths.
   * Interface exposed by server to in-browser code is well defined.
 * Non-browser HTTP clients.
   * REST provides a structure for an HTTP API

## Back to Code

### branch: 03-add-users

```
$ mix phx.gen.html Users User users name:string password:string
```

Edit the migration:

```
      add :name, :string, null: false
      # Not using passwords for now - they're tricky.
      add :password, :string, null: false, default: ""
```

 - Add the line in the router.
 - Run ```mix ecto.migrate```
 - Restart the dev server.

We've got the same scaffolding for users as for posts, but we want to expose it
in a way that makes sense.

So let's add a title bar to our pages that handles user registrations.

In .../layout/app.html.eex:

```
    <div role="main" class="container">
      <div class="row">
        <div class="col">
          <%= if function_exported?(Routes, :live_dashboard_path, 2) do %>
            <p><%= link "LiveDashboard", to: Routes.live_dashboard_path(@conn, :home) %></li>
          <% end %>
        </div>
        <div class="col">
          <%= link "Register", to: Routes.user_path(@conn, :new) %>
        </div>
      </div>
```

Then let's clean up the user form in .../user/form.html.eex:

```
<%= form_for @changeset, @action, fn f -> %>
  <%= if @changeset.action do %>
    <div class="alert alert-danger">
      <p>Oops, something went wrong! Please check the errors below.</p>
    </div>
  <% end %>

  <div class="form-group">
    <%= label f, :name %>
    <%= text_input f, :name, class: "form-control" %>
    <%= error_tag f, :name %>
  </div>

  <div class="form-group">
    <%= label f, :password %>
    <%= password_input f, :password, class: "form-control" %>
    <%= error_tag f, :password %>
  </div>

  <div>
    <%= submit "Save", class: "btn btn-primary" %>
  </div>
<% end %>
```

Since we're only doing server-side validation, we want to force error feedback
to display by editing assets/css/app.scss:

```
.invalid-feedback {
  display: block;
}
```

Now that we can create users, we want to let them log in.

We can think of a "user session" as being a special kind of resource. The
sessions resource is kind of weird in that it doesn't have a DB table or even a
context, because the data is stored in a cookie. But it does have two basic
operations that can be handled as REST methods: create and delete.

Add our new resource to the router:

```
scope "/", PhotoBlogWeb do
  ...
  resources "/sessions", SessionController, 
    only: [:create, :delete], singleton: true
```

Create our controller module:

lib/photo\_blog\_web/controllers/session_controller.ex

```
defmodule PhotoBlogWeb.SessionController do
  use PhotoBlogWeb, :controller

  def create(conn, %{"name" => name}) do
    user = PhotoBlog.Users.get_user_by_name(name)
    if user do
      conn
      |> put_session(:user_id, user.id)
      |> put_flash(:info, "Welcome back #{user.name}")
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

Add a form to the layout:

.../templates/layout/app.html.eex

```
      <div class="row">
        <div class="col my-2">
          <%= if function_exported?(Routes, :live_dashboard_path, 2) do %>
            <p><%= link "LiveDashboard", to: Routes.live_dashboard_path(@conn, :home) %></li>
          <% end %>
        </div>
        <%= if @conn.assigns[:current_user] do %>
          <div class="col">
            User: <%= @current_user.name %> |
            <%= link("Logout", to: Routes.session_path(@conn, :delete),
              method: :delete) %>
          </div>
        <% else %>
          <div class="col">
            <%= form_for @conn, Routes.session_path(@conn, :create),
            [class: "form-inline"], fn f -> %>
              <%= text_input f, :name, class: "form-control" %>
              <%= submit "Login", class: "btn btn-secondary" %>
            <% end %>
          </div>
          <div class="col my-2">
            <p><%= link "Register", to: Routes.user_path(@conn, :new) %></p>
          </div>
        <% end %>
      </div>
```

Create a fetch_session plug:

lib/photo\_blog\_web/plugs/fetch_session.ex

```
defmodule PhotoBlogWeb.Plugs.FetchSession do
  import Plug.Conn

  def init(args), do: args

  def call(conn, _args) do
    user = PhotoBlog.Users.get_user(get_session(conn, :user_id) || -1)
    if user do
      assign(conn, :current_user, user)
    else
      assign(conn, :current_user, nil)
    end
  end
end
```

Add the plug to our router:

```
pipeline :browser do
  ...
  plug PhotoBlogWeb.Plugs.FetchSession
end
```

Add two more accessors to our context:

lib/husky\_shop/users/users.ex

```
...
  # below get_user!
  def get_user(id), do: Repo.get(User, id)
  
  def get_user_by_name(name) do
    Repo.get_by(User, name: name)
  end
```

## Add user_id to post

Reorder the migrations so posts comes after users.

Edit the posts migration.

```
      add :user_id, references(:users), null: false
```

Update the schema module in .../posts/post.ex:

```
  schema "posts" do
    ...
    belongs_to :user, PhotoBlog.Users.User
```

Update the schema module in .../users/user.ex:

```
  schema "users" do
    ...
    has_many :posts, PhotoBlog.Posts.Post
```

Add default accounts, posts for testing:

priv/repo/seeds.ex

```
alias PhotoBlog.Repo
alias PhotoBlog.Users.User
alias PhotoBlog.Posts.Post

alice = Repo.insert!(%User{name: "alice"})
bob = Repo.insert!(%User{name: "bob"})

Repo.insert!(%Post{user_id: alice.id, body: "Alice says Hi!"})
Repo.insert!(%Post{user_id: bob.id, body: "Bob says garblarg!"})
```

Recreate a clean DB with:

```
mix ecto.reset
```

Show user name on posts, update .../post/index.html.eex:

```
      <div class="card-body">
        <p>Posted by <%= post.user.name %></p>
```

Preload the user on post list, update .../posts.ex:

```
  def list_posts do
    Repo.all(Post) 
    |> Repo.preload(:user)
  end
```

Add link to new post, update .../post/index.html.eex:

```
    <h1>Photo Feed</h1>
    <%= link "New Post", to: Routes.post_path(@conn, :new) %>
```

Now add user id to new posts, first in .../post_controller.ex:

```
  def create(conn, %{"post" => post_params}) do
    post_params = post_params
    |> Map.put("user_id", conn.assigns[:current_user].id)
```

Then add the new field it to the changeset in .../posts/post.ex:

```
  def changeset(post, attrs) do
    post
    |> cast(attrs, [:body, :user_id])
    |> validate_required([:body, :user_id])
  end
```

### Next: We'll add photos to our photo app.
