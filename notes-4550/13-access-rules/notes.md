
## First: Homework Questions

## Where were we?

 - We've got a Phoenix app
 - We've got two resources: posts, users
 - We've got the ability to log in and log out.
 - Posts show name of user who made them.
 - We can upload photos

## finish up branch: 04-photo-uploads

That's fun, but we want photos on our feed. Let's update .../page/index.html.eex:

```
  <!-- where the img was comment out in the card -->
      <% src = Routes.post_path(@conn, :photo, post) %>
      <img class="card-img-top" src="<%= src %>" alt="">
```

Now let's update our seeds to actually inject some images.

First, we need to copy the "stock" photos over to priv/photos.

Then, update priv/repo/seeds.exs:

```
alias PhotoBlog.Repo
alias PhotoBlog.Users.User
alias PhotoBlog.Posts.Post
alias PhotoBlog.Photos

defmodule Inject do
  def photo(name) do
    photos = Application.app_dir(:photo_blog, "priv/photos")
    path = Path.join(photos, name)
    {:ok, hash} = Photos.save_photo(name, path)
    hash
  end
end

alice = Repo.insert!(%User{name: "alice"})
bob = Repo.insert!(%User{name: "bob"})

elephant = Inject.photo("elephant.jpg")
moon = Inject.photo("moon.jpg")

Repo.insert!(%Post{user_id: alice.id, photo_hash: elephant, body: "Alice says Hi!"})
Repo.insert!(%Post{user_id: bob.id, photo_hash: moon, body: "Bob says garblarg!"})
```

## branch: 05-access

Problem: We don't enforce any restrictions on users taking actions.

When thinking about access rules, we want to do two separate things:

 - Make sure the app directly enforces the access rules.
   - This must be in server-side code.
 - Alter the UI to not show users actions they can't perform.
   - And possibly do other in-browser validation.

Critically, it's not enough to just alter the UI. The external interface to our
web application is "the user can make arbitrary HTTP requests". Our app should
respond correctly to any arbitrary series of HTTP requests, and what UI we
happen to provide is has no relevance to the security / access control question.

First rule: You must be logged in to create or edit a post.

In .../plugs/require_user.ex:

```
defmodule PhotoBlogWeb.Plugs.RequireUser do
  use PhotoBlogWeb, :controller

  def init(args), do: args

  def call(conn, _args) do
    if conn.assigns[:current_user] do
      conn
    else
      conn
      |> put_flash(:error, "You must log in to do that.")
      |> redirect(to: Routes.page_path(conn, :index))
      |> halt()
    end
  end
end
```

Now we use that plug in .../controllers/post_controller.ex:

```
  alias PhotoBlogWeb.Plugs
  plug Plugs.RequireUser when action in [:new, :edit, :create, :update]
```

We don't want to show a "new post" link when not logged in.

First, I want to create a helper function to check if a user is logged in that's
visible from every controller and template.

New file ```lib/photo_blog_web/helpers.ex```:

```
defmodule PhotoBlogWeb.Helpers do
  def have_current_user?(conn) do
    conn.assigns[:current_user] != nil
  end
end
```

Now we want to expose this in all controllers and templates.

In ```lib/photo_blog_web.ex```:

```
  def controller do
    quote do
      ...
      import PhotoBlogWeb.Gettext
      import PhotoBlogWeb.Helpers # Add this
  ...
  defp view_helpers do
    quote do
      ...
      import PhotoBlogWeb.Gettext
      import PhotoBlogWeb.Helpers # And this
```

Now we can update the template in .../page/index.html.eex:

```
    <%= if have_current_user?(@conn) do %>
      <p><%= link "New Post", to: Routes.post_path(@conn, :new) %></p>
    <% end %>
```

Next, we want to enforce that only the creator can edit or delete posts.


Over in .../post_controller.ex:

First, factor out fetching the post:

```
  plug :fetch_post when action in [:show, :photo, :edit, :update, :delete]
  ... 
  def fetch_post(conn, _args) do
    id = conn.params["id"]
    post = Posts.get_post!(id)
    assign(conn, :post, post)
  end
  ... 
  def show(conn, %{"id" => _id}) do
    post = conn.assigns[:post]
  ...
  # also: photo, edit, update, delete 
```

Then, enforce access in a plug:

```
  plug :require_owner when action in [:edit, :update, :delete]
  ...
  def require_owner(conn, _args) do
    user = conn.assigns[:current_user]
    post = conn.assigns[:post]

    if user.id == post.user_id do
      conn
    else
      conn
      |> put_flash(:error, "That isn't yours.")
      |> redirect(to: Routes.page_path(conn, :index))
      |> halt()
    end
  end
```

Next, we want to hide the edit link if the current user isn't
the owner.

helpers:

```
  def current_user_id?(conn, user_id) do
    user = conn.assigns[:current_user]
    user && user.id == user_id
  end
```

post/show:

```
  <%= if current_user_id?(@conn, @post.user_id) do %>
    <span><%= link "Edit", to: Routes.post_path(@conn, :edit, @post) %></span>
  <% end %>
```

## branch: 06-comments

```
$ mix phx.gen.html Comments Comment comments \
    post_id:references:posts user_id:references:users \
    body:text vote:integer
```

Edit the migration:

 * Nothing can be null
 * Vote can be default zero

Add router line.

Now we want to edit the schema file to hook up the two
relations.

Edit .../comments/comment.ex:

```
    # replaces the two :id fields
    belongs_to :post, PhotoBlog.Posts.Post
    belongs_to :user, PhotoBlog.Users.User
    ...
    |> cast(attrs, [:body, :vote, :post_id, :user_id])
    |> validate_required([:body, :vote, :post_id, :user_id])
```

Now in both .../post.ex and .../user.ex:

```
    has_many :comments, PhotoBlog.Comments.Comment
```

Add the comment form to the post/show page.

```
<h2>Add Comment</h2>

<%= if have_current_user?(@conn) do %>
  <%= render(
    PhotoBlogWeb.CommentView,
    "form.html",
    changeset: @new_comment,
    action: Routes.comment_path(@conn, :create)
  ) %>
<% end %>
```

Add a helper to helpers.ex:

```
  def current_user_id(conn) do
    user = conn.assigns[:current_user]
    user && user.id
  end
```

Over in post_controller.ex:

```
  def show(conn, %{"id" => _id}) do
    post = conn.assigns[:post]
    comm = %Comments.Comment{
      post_id: post.id,
      user_id: current_user_id(conn),
      vote: 0,
    }
    new_comment = Comments.change_comment(comm)
    render(conn, "show.html", post: post, new_comment: new_comment)
```

Bootstrapify the form in .../comments/form.html.ex:

 * Wrap each element in class="form-group"
 * Add class="form-control" for each control
 * Submit button gets class="btn btn-primary"
 * Also, add ```<%= hidden_input f, :post_id %>```
 * Also, add ```<%= hidden_input f, :user_id %>```

In comments_controller, we want to verify that the provided
user id matches the current user id.

```
  def create
    comment_params = comment_params
    |> Map.put("user_id", current_user_id(conn))
```

Next, we want to show comments.

```
  def show(conn, %{"id" => _id}) do
    post = Posts.load_comments(conn.assigns[:post])
```

Now we need to write that function, over in photo_blog/posts.ex:

```
  def load_comments(%Post{} = post) do
    Repo.preload(post, [comments: :user])
  end
```

Now let's actually show the comments on the post page.

In templates/post/show.html.eex:

```
<h2>Comments</h2>

<%= for comm <- @post.comments do %>
  <div class="card">
    <div class="card-body">
      <p>Posted by <%= comm.user.name %></p>
      <p class="card-text"><%= comm.body %></p>
    </div>
  </div>
<% end %>
```

## Finally, review the parts and associated docs
 
 * Elixir: https://elixir-lang.org/docs.html
   * https://elixirschool.com
 * Phoenix: https://www.phoenixframework.org/
   * Guides: App structure, html templates, channels
   * https://hexdocs.pm/phoenix/
   * https://hexdocs.pm/phoenix/js/
 * Ecto: https://hexdocs.pm/ecto
   * This is the library that access the RDBMS
   * https://elixirschool.com/en/lessons/ecto/basics/
 * https://www.postgresql.org/

