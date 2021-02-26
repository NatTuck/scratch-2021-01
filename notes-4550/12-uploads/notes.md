
## First: Homework Questions

## Where were we?

 - We've got a Phoenix app
 - We've got two resources: posts, users
 - We've got the ability to log in and log out.

## Bugfix: Login button

 - Need to combine Home, LiveDashboard into one column.

## Continuing 03-add-users: Add user_id to post

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

### branch: 04-photo-uploads

Update the migration in priv/repo/migrations/*post.ex:

```
    create table(:posts) do
      ...
      add :photo_hash, :text, null: false
```

Update the schema module in .../posts/post.ex:

```
  schema "posts" do
    field :body, :string
    field :photo_hash, :string
    belongs_to :user, PhotoBlog.Users.User

    timestamps()
  end

  @doc false
  def changeset(post, attrs) do
    post
    |> cast(attrs, [:body, :photo_hash, :user_id])
    |> validate_required([:body, :photo_hash, :user_id])
  end
```


To be able to test with the new field, we'll need to temporarily
add fake photos to the seeds in priv/repo/seeds.exs:

```
Repo.insert!(%Post{user_id: alice.id, photo_hash: "", body: "Alice says Hi!"})
```

And then reset the db:

```
$ mix ecto.reset
```

Update the form to include the file upload in .../post/form.html.eex:

```
<%= form_for @changeset, @action, [multipart: true], fn f -> %>
  ...
  <div class="form-group">
    <%= label f, :photo %>
    <%= file_input f, :photo, class: "form-control" %>
    <%= error_tag f, :photo %>
  </div>
```

Now we need to actually handle saving the image on upload.

```
defmodule PhotoBlog.Photos do
  def save_photo(name, path) do
    data = File.read!(path)
    hash = sha256(data)
    meta = read_meta(hash)
    save_photo(name, data, hash, meta)
  end

  def save_photo(name, data, hash, nil) do
    File.mkdir_p!(base_path(hash))
    meta = %{
      name: name,
      refs: 0,
    }
    save_photo(name, data, hash, meta)
  end

  # Note: data race
  def save_photo(name, data, hash, meta) do
    meta = Map.update!(meta, :refs, &(&1 + 1))
    File.write!(meta_path(hash), Jason.encode!(meta))
    File.write!(data_path(hash), data)
    {:ok, hash}
  end

  def load_photo(hash) do
    data = File.read!(data_path(hash))
    meta = read_meta(hash)
    {:ok, Map.get(meta, :name), data}
  end

  # TODO: drop_photo

  def read_meta(hash) do
    with {:ok, data} <- File.read(meta_path(hash)),
         {:ok, meta} <- Jason.decode(data, keys: :atoms)
    do
      meta
    else
      _ -> nil
    end
  end

  def base_path(hash) do
    Path.expand("~/.local/data/photo_blog")
    |> Path.join("#{Mix.env}")
    |> Path.join(String.slice(hash, 0, 2))
    |> Path.join(String.slice(hash, 2, 30))
  end

  def data_path(hash) do
    Path.join(base_path(hash), "photo.jpg")
  end

  def meta_path(hash) do
    Path.join(base_path(hash), "meta.json")
  end

  def sha256(data) do
    :crypto.hash(:sha256, data)
    |> Base.encode16(case: :lower)
  end
end
```

Now let's update our controller to manage images.

```
  alias PhotoBlog.Photos
  ...

  def create(conn, %{"post" => post_params}) do
    up = post_params["photo"]
    {:ok, hash} = Photos.save_photo(up.filename, up.path)
    post_params = post_params
    |> Map.put("user_id", conn.assigns[:current_user].id)
    |> Map.put("photo_hash", hash)
  ...
  
  def update(conn, %{"id" => id, "post" => post_params}) do
    post = Posts.get_post!(id)
    up = post_params["photo"]

    post_params = if up do
      # FIXME: Remove old image
      {:ok, hash} = Photos.save_photo(up.filename, up.path)
      Map.put(post_params, "photo_hash", hash)
    else
      post_params
    end
  ...
  
  # New controller function.
  def photo(conn, %{"id" => id}) do
    post = Posts.get_post!(id)
    {:ok, _name, data} = Photos.load_photo(post.photo_hash)
    conn
    |> put_resp_content_type("image/jpeg")
    |> send_resp(200, data)
  end
  
  def delete(...) do
    # FIXME: Remove old image
```

Let's add our new function to .../router.ex:

```
    get "/posts/photo/:id", PostController, :photo
```

And let's add our photo to .../post/show.html.eex;

```
<div>
  <img src="<%= Routes.post_path(@conn, :photo, @post) %>" alt="">
</div>
```

### Lecture Ended Here

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
