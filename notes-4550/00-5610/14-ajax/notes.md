---
layout: default
---

## Project Questions?

 - The project is due this Thursday.
 - You've put in a lot of work.
 - How many people have done a deploy?
 - The rest of you should do that right after class today. Deploys can take
   time, and you don't want that time to be late hours.

## Where are we?

 - We're building a photo sharing app: Lens

Last time, we:

 - Added a photos resource.
 - Implemented photo upload.
 - Implemented user sessions.

To finish up from last time we need:

 - Actually showing photos.

## Showing Photos

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

## Adding photos to user page

 * Repo: https://github.com/NatTuck/lens
 * Start branch: 3-photos
 * End branch: 4-user-page

Associations:

 - In our migrations, we tell the database which fields in tables
   reference the id column in other tables.
 - We use this mechanism to model relationships between tables.
 - In Elixir, we interact with tables using Ecto schemas.
 - If we tell Ecto about these relationships too, it can build nested
   structures in response to DB queries.

Let's have our user pages display all photos the user has uploaded.

 - Photos can already find their users, using the belongs_to relationship.
   - The photos table has a reference to users.
   - This is a 1:1 link.
 - For users to find their photos, we need a has_many relationship.
   - To find a user's photos, we need to query the photos tables
     for the photo->user reference.
   - This is a one to many link.

lib/lens/users/user.ex:

```
  schema "users" do
    ...
    has_many :photos, Lens.Photos.Photo
    # implicitly looks for a user_id field in photos table
```

This adds a "photos" field to our User struct, potentially containing a
list of photos.

Now let's add photos to the user page.

```
<h2>Photos</h2>

<p><%= link("New Photo", to: Routes.photo_path(@conn, :new)) %></p>

<div class="row">
  <%= for photo <- @user.photos do %>
    <div class="card col-4">
      <%= img_tag(Routes.photo_path(@conn, :file, photo), class: "card-img-top") %>
      <div class="card-body">
        <p><%= photo.desc %></p>
      </div>
    </div>
  <% end %>
</div>

<p>
  <span><%= link "Edit", to: Routes.user_path(@conn, :edit, @user) %></span>
  <span><%= link "Back", to: Routes.user_path(@conn, :index) %></span>
</p>
```

We can load up /users/1 and get a Ecto.Association.NotLoaded error.

Ecto doesn't auto-load associations - that would require extra DB queries even
when we aren't using the linked data. Instead, the default is to fill in all
assocation fields with an Association.NotLoaded struct.

.../user_controller.ex:

```
  def show(conn, %{"id" => id}) do
    user = Users.get_user_with_photos!(id)
    render(conn, "show.html", user: user)
  end
```

.../users.ex:

```
  def get_user_with_photos!(id) do
    Repo.one from uu in User,
      where: uu.id == ^id,
      preload: [:photos]
  end
```

Demo this working:

 * Upload a couple more photos.
 * Show /users/2 where there are no photos.
 * Show /photos

Bug: Photo leaks

 - When we delete a photo, we don't delete the file.
 - We should do that.
 - What about when we delete a user?
 - What if we crash between saving the photo and creating the DB entry?

Crash solution: upload log

 - Create a photo_logs table
 - Record creation becomes:
   - Generate UUID
   - Create photo_logs record with UUID
   - Save file
   - Create photos record
   - Delete photo_logs record
 - Any time later we can scan the photo_logs table. Files that
   exist but don't have a corresponding photos record can be deleted.
 - This can be a process on startup or run periodically.
   - Need to check log timestamp. For log entries older than a minute or
     so it's safe to assume that the create process would have finished.
 - Entries in the photos table are always consistent with files on disk.
 - We also need to log deletes.
   - Log delete
   - Delete photos record
   - Delete file
   - Delete log entry

You can safely ignore this problem for assignments in this class, but this is
the sort of problem that matters a lot in production.

## More Complicated Associations

 * Start branch: 5-cleanups
 * End branch: 6-tags

For this next part, I'm going to start with a cleaned up version of Lens. I
moved some stuff around and added styling to make it feel less like
autogenerated scaffolds and more like a photo sharing app. You can see the
changes by diffing 4-user-page and 5-cleanups on github.

We're going to add tags.

 * Tags are an arbitrary single word, no longer than 12 characters.
 * Any user can add or remove a tag from any photo.
 * Pages:
   - index: List all tags and usage counts.
   - show: Show all photos with a given tag.

Currently our application is like a web app from 2002: Every action causes a new
page to be loaded. In more modern apps, it's common to be able to perform
actions *without* a page reload.

We'll implement creation and deletion of tags this way: no page loads required.
This is going to require some JavaScript code to do HTTP requests.

Let's start by creating a new resource: tags.

```
$ mix phx.gen.html Tags Tag tags name:string
```

This resource isn't anything new. Let's clean it up:

 * In migration: Tag name can't be null
 * Add to router
 * Run the migration

Now we need an interesting relation: many-to-many

 * A photo can have many tags.
 * A tag can have many photos.

We can't solve that by adding a reference column to either table. We'll need a
new table - a join table - to store the {tag\_id, photo\_id} pairs.

Let's create that as a resource:

```
$ mix phx.gen.json Tags PhotoTag photo_tags photo_id:references:photos tag_id:references:tags
```

This command does the same stuff as the "phx.gen.html" generator, except:

 - There are no HTML templates.
 - The controller is different.
 - The view module has code in it.

Note that the output tells us to put the router line in the "api" scope rather
than the "browser" scope.

Normal process:

 * Edit the migration to make name non-null.
   * Delete photo_tags when either end is deleted.
   * Tags don't get deleted when unused.
 * Open up the router.

Add a new pipeline and add in the route:

```
  pipeline :ajax do
    plug :accepts, ["json"]
    plug :fetch_session
    plug LensWeb.Plugs.FetchCurrentUser
    plug :protect_from_forgery
    plug :put_secure_browser_headers
  end

  ...

  scope "/ajax", LensWeb do
    pipe_through :ajax

    resources "/photo_tags", PhotoTagController, except: [:new, :edit]
  end
```

The provided "api" scope isn't appropriate, becuase it's intended for use by
non-browser clients that don't carry session cookies. We want session cookies,
and we want the security mechanisms like "pretect\_from\_forgery", we just don't
need to fetch the flash, and we're going to return json rather than html.

Now we have our resources, so let's add the associations.

In .../photo.ex:

```
  schema ...
    has_many :photo_tags, Lens.Tags.PhotoTag
    has_many :tags, through: [:photo_tags, :tag]
```

In .../tag.ex:

```
  schema ...
    has_many :photo_tags, Lens.Tags.PhotoTag
    has_many :photos, through: [:photo_tags, :photo]
```

In .../photo_tag.ex:

```
  schema "photo_tags" do
    # We replace the fields with associations:
    belongs_to :photo, Lens.Photos.Photo
    belongs_to :tag, Lens.Tags.Tag
```

While we're in .../photo_tag.ex:

```
  def changeset(photo_tag, attrs) do
    photo_tag
    |> cast(attrs, [:photo_id, :tag_id])
    |> validate_required([:photo_id, :tag_id])
  end
```

Add a tags UI to our photo card in ...photos/card.html.eex:

```
    <div class="photo-tags" data-photo-id="<%= @photo.id %>">
      <p class="tag-list">Tags:
        <%= for tag <- @photo.tags do %>
          <span class="badge badge-secondary">
            <%= tag.name %>
          </span>
        <% end %>
      </p>
      <div class="row">
        <div class="col">
          <input type="text" class="form-control">
        </div>
        <div class="col">
          <button class="btn btn-secondary add-tag-btn">Add Tag</button>
        </div>
      </div>
    </div>
```

Now we need photos to always preload tags:

In .../photos.ex:

```
   def recent_photos ...
      preload: [:user, :photo_tags, :tags]
      
   def get_photo! ...
      preload: [:user, :photo_tags, :tags]
```

In .../users.ex:

```
  def get_user_with_photos!(id) do
      ...
      preload: [photos: [:user, :tags, :photo_tags]]
```

Now we need the in-browser logic to actually add tags.

Because we're going to make POST requests and we have protect\_from\_forgery
enabled, we need to give our JS access to a CSRF token.

In .../layout/app.html.eex:

```
    <script>
     window.csrf_token = "<%= Phoenix.Controller.get_csrf_token() %>";
    </script>
  </head>
```

In .../app.js:

```
import $ from 'jquery';
import _ from 'lodash';
...
  
function add_tag(photo_id, tag) {
  let data = {photo_tag: {photo_id: photo_id, tag_name: tag}};
  $.ajax("/ajax/photo_tags", {
    method: "post",
    dataType: "json",
    contentType: "application/json; charset=UTF-8",
    data: JSON.stringify(data),
    headers: {
      'x-csrf-token': window.csrf_token,
    },
    success: (resp) => {
      console.log("Added tag", data);
    },
    error: (resp) => {
      console.log(resp);
      console.log(resp.responseText);
    }
  });
}

function init_tags() {
  $('.photo-tags').each((_, base) => {
    let photo_id = $(base).data('photo-id');
    let button = $(base).find('.add-tag-btn');
    let input = $(base).find('input');

    let submit = (_ev) => {
      let tag = input.val();
      add_tag(photo_id, tag);
    };

    button.on("click", submit);
    input.on("keypress", (ev) => {
      if (ev.key == "Enter") {
        submit(ev);
      }
    });
  });
}
```

This almost works. 

 * Unfortunately, we're passing a tag name rather than a tag ID. 
 * We need to find the tag id before we create the photo_tag.
 * What if the tag doesn't exist?

First, tags should be unique.

```
$ mix ecto.gen.migration make_tags_unique
```

Add a unique index in the new migration:

```
def change do
  create unique_index(:tags, [:name])
  create unique_index(:photo_tags, [:photo_id, :tag_id])
end
```

```
$ mix ecto.migrate
```

Tell ecto about it in .../tags/tag.ex:

```
def changeset(cset) do
    ...
    |> unique_constraint(:name)
```

And in .../tags/photo_tag.ex:

```
def changeset(cset) do
    ...
    |> unique_constraint(:tag_id, name: :photo_tags_photo_id_tag_id_index)
```

Now let's modify the tag create function so that it doesn't
create duplicates and instead returns an existing row if it
exists.

In .../tags.ex

```
  def create_tag(attrs \\ %{}) do
    {:ok, tag} = %Tag{}
    |> Tag.changeset(attrs)
    |> Repo.insert(
      on_conflict: :nothing,
      conflict_target: :name
    )
    tag = Repo.get_by!(Tag, name: tag.name)
    {:ok, tag}
  end
```

Still in .../tags.ex

```
  def create_photo_tag(), do: create_photo_tag(%{})

  def create_photo_tag(attrs = %{"tag_name" => tag_name}) do
    {:ok, tag} = create_tag(%{"name" => tag_name})
    attrs
    |> Map.delete("tag_name")
    |> Map.put("tag_id", tag.id)
    |> create_photo_tag
  end

  def create_photo_tag(attrs) do
    {:ok, pt} = %PhotoTag{}
    |> PhotoTag.changeset(attrs)
    |> Repo.insert(on_conflict: :nothing)
    pt = Repo.get_by!(PhotoTag, tag_id: attrs["tag_id"], photo_id: attrs["photo_id"])
    {:ok, pt}
  end
```

Next, let's make sure we return tag names.

Still in .../tags.ex

```
  def get_photo_tag!(id) do
    Repo.get!(PhotoTag, id)
    |> Repo.preload(:tag)
  end
```

Now in .../photo\_tag_controller.ex

```
  def create(conn, %{"photo_tag" => photo_tag_params}) do
    with {:ok, %PhotoTag{} = photo_tag} <- Tags.create_photo_tag(photo_tag_params) do
      # Refetch photo_tag
      photo_tag = Tags.get_photo_tag!(photo_tag.id)
```

Now we need to update the tag view to return the relevent data:

Create a new file .../templates/tag/tag.html.eex:

```
<span class="badge badge-secondary"
      data-tag-id="<%= @pt.tag.id %>"
      data-photo-tag-id="<%= @pt.id %>">
  <%= @pt.tag.name %>
  <a href="#" class="badge badge-secondary tag-delete">&times;</a>
</span>
```

.../views/photo\_tag\_view.ex

```
  def render("photo_tag.json", %{photo_tag: photo_tag}) do
    tag_name = if Ecto.assoc_loaded?(Map.get(photo_tag, :tag)) do
      photo_tag.tag.name
    else
      nil
    end
   
    html = render(LensWeb.TagView, "tag.html", pt: photo_tag)
    |> Phoenix.HTML.safe_to_string
    
    %{
      id: photo_tag.id,
      photo_id: photo_tag.photo_id,
      tag_id: photo_tag.tag_id,
      tag_name: tag_name,
      html: html,
    }
  end
```

Now we can add tags to photos and we get the correct data back in the response.
Next we need to add the tag to the display back in the browser.


Update the photo card in .../photos/card.html.eex:

```
  <!-- replace the badge span with a render -->
  <%= for pt <- @photo.photo_tags do %>
    <%= render LensWeb.TagView, "tag.html", Map.put(assigns, :pt, pt) %>
  <% end %>
```

Update our .../app.js:

```
function show_tag(photo_id, tag) {
  let cards = $('.photo-tags');
  let base = _.find(cards, (xx) => $(xx).data('photo-id') == photo_id);
  let list = $(base).find('.tag-list');

  for (let pp of list.find('.badge')) {
    if ($(pp).data('tag-id') == tag.tag_id) {
      return;
    }
  }

  list.append(tag.html);

  console.log("shown", photo_id, tag);
}
...
function add_tag(photo_id, tag) {
   ...
   success: (resp) => {
      console.log(resp);
      show_tag(photo_id, resp.data);
    },
```

Adding tags now works. Next is deleting tags.

In .../app.js:

```
function show_tag(photo_id, tag) {
  ...

  list.append(tag.html);

  list = $(base).find('.tag-list');

  for (let pp of list.find('.badge')) {
    if ($(pp).data('tag-id') == tag.tag_id) {
      $(pp).find('.tag-delete').on("click", remove_click);
    }
  }

...

function init_tags() {
  $('.photo-tags').each((_, base) => {
    ...
    $(base).find('.tag-delete').on("click", remove_click);
  });
}

...

function remove_click(ev) {
  ev.preventDefault();
 
  let tag = $(ev.target).parent();
  let photo_tag_id = tag.data('photo-tag-id');

  $.ajax("/ajax/photo_tags/"+photo_tag_id, {
    method: "delete",
    dataType: "json",
    contentType: "application/json; charset=UTF-8",
    data: "",
    headers: {
      'x-csrf-token': window.csrf_token,
    },
    success: (resp) => {
      console.log("tag removed", resp);
      tag.remove();
    },
    error: (resp) => {
      console.log(resp);
      console.log(resp.responseText);
    }
  });
}
```

# Associations Review

wheel belong\_to car: The wheel table has a car_id field. Given a wheel, we can
preload the car.

car has\_many wheels: The wheel table has a car_id field. Given a car, we can
preload all the wheels.

car has\_one roof: The roof table has a car_id field. Given a car, we can get
the roof. The roof will belong\_to the car.

has\_many through: This allows indirect or many\_to\_many relationships, with a
lot of options.

# Deep Preloads

Let's take a look at another application: Inkfish.

 * Clone the inkfish repo.

Grade belongs-to sub belongs-to assignment belongs-to bucket belongs-to course.

 * Load up my HW01 submission on Inkfish.
 * Show the Feedback grade.
 * Scroll down to the course footer.

So to display a grade, we need the course struct. Let's look at the code for
that.

apps/inkfish\_web/lib/inkfish\_web/grades.ex:

 * Find get\_grade!
 * Explain the preload: line.
 * Preload operates as a series of queries by default.
   * Get the grade
   * Find the grade column for the grade
   * Find the sub for the grade
   * Find the reg for the sub
   * Find the user for the reg
   * Find the team for the sub
   * Find the regs for the team
   * Find the users for the regs
 * In this example I'm doing explicit joins, with the join
   results bound to names that are referenced in the preload.
   * Inner join means we need to always find something.
   * Left (outer) join means we may get multiple rows back.
   * Too many outer joins can mean an explosion in result size.
 * This allows Ecto to do one complex query instead of 9 simpler ones.
 * This *should* be faster, but you'd need to profile to be sure.
 * In general, I don't recommend explicit joins. It's an optimization, and
   optimizations before profiling are a bad idea.
 * This query was definitely easier to write in Ecto than in raw SQL.

Performance:

 * grade/show on my sub takes ~130ms to load
 * sub/show takes ~7ms, with nearly as complex a DB query
 * Why?
 * grade/show loads the entire submission from disk and sends
   it as part of the payload. Disk I/O isn't the fastest.

Overflow:

 - Switch to staff mode and grade my HW01 submission.
 - Adding and editing comments uses $.ajax like the tags example.
 - The left column is a react component. The code viewer isn't. This doesn't
   result in the cleanest code.

