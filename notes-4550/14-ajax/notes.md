
# Homework Questions: CRUD app

## Today: AJAX

## First though, deployment.

 - Do a full deployment of photo_blog as is.
 - Target: photos.ironbeard.com 

## Branch: 07-aync-votes

New feature:

 - Users can vote on posts.
 - Posts on the main page should always be ordered by the
   number of votes.

We're going to add this feature as if it were 2010, with async HTTP
requests and jQuery.

To start off, we're going to create a new resource:

```
$ mix phx.gen.json Votes Vote votes post_id:references:posts user_id:references:users votes:integer
```

This is a JSON resource, intended to be accessed by code rather than
directly by a person loading pages in a browser.

Next, we need to edit the migration.

```
    # Everything is null: false
    create index(:votes, [:post_id, :user_id], unique: true)
```

Now add this to the router; this goes in "api", not "browser".

```
  scope "/api", PhotoBlogWeb do
    pipe_through :api

    resources "/votes", VoteController, except: [:new, :edit]
  end
```

Now run the migration

```
$ mix ecto.migrate
```

Now let's add our upvote and downvote buttons.

```
    <!-- in .../page/index.html.eex -->
    <div class="card-body">
      ...
      <p>
        Votes: <%= post.score %>
        &#128077; <!-- google "thumbs up emojii unicode" -->
        &#128078;
      </p>
    <div>
```

Problem: Our post schema doesn't really have a score field.

Step 1: Update schemas

```
  schema "posts" do
    ... 
    has_many :votes, PhotoBlog.Votes.Vote

    field :score, :integer, virtual: true
```

```
  schema "votes" do
    field :votes, :integer
    belongs_to :post, PhotoBlog.Posts.Post
    belongs_to :user, PhotoBlog.Users.User
  
  ...
    |> cast(attrs, [:votes, :user_id, :post_id])
    |> validate_required([:votes, :user_id, :post_id])
```

In page controller:

```
  def index(conn, _params) do
    posts = PhotoBlog.Posts.list_posts()
    |> PhotoBlog.Posts.load_votes()
```

Now we need load_votes, over in posts.ex:

```
  def load_votes(posts) do
    posts = Repo.preload(posts, :votes)

    Enum.map posts, fn post ->
      score = post.votes
      |> Enum.map(&(&1.votes))
      |> Enum.sum()
      %{ post | score: score }
    end
  end
```

So we can display no votes, next up is the up and downvote
buttons. Back over in the form, let's make them identifiable
from JS code.

```
<div class="row posts-container">
  <%= for post <- @posts do %>
    <div class="col-sm-4 card post-card" data-post-id="<%= post.id %>">
      <% src = Routes.post_path(@conn, :photo, post) %>
      <img class="card-img-top" src="<%= src %>" alt="">
      <div class="card-body">
        <p>Posted by <%= post.user.name %></p>
        <p class="card-text"><%= post.body %></p>
        <%= link "view", to: Routes.post_path(@conn, :show, post) %>
        <p>
          Votes: <span class="post-score"><%= post.score %></span>
          <button class="upvote">&#128077;</button>
          <button class="downvote">&#128078;</button>
        </p>
      </div>
    </div>
  <% end %>
</div>
```

Now let's add some events to those buttons.


```
// New file: assets/js/voting.js

import $ from 'jquery';

function vote_done(data, status, _xhr) {
    console.log("updated", status, data);
}

function send_vote(post_id, votes) {
    let body = {vote: {post_id, votes}};

    $.ajax("/api/votes", {
        method: "post",
        dataType: "json",
        contentType: "application/json; charset=UTF-8",
        data: JSON.stringify(body),
        headers: {},
        success: vote_done,
        error: console.log,
    });
}

function setup() {
    $(".post-card").each((_ii, card) => {
        let id = $(card).data('post-id');

        $(card).find('.upvote').click(() => {
            send_vote(id, 1);
        });
        $(card).find('.downvote').click(() => {
            send_vote(id, -1);
        });
    });
}

$(setup);
```

Import from app.js:

```
import "./voting";
```

And... that doesn't work. Once we look into the response (responseText),
it turns out we didn't specify user_id. That ends up being a bigger problem
than we'd like.

 - We can't just pass in user_id in the AJAX request. That would let users 
   forge votes from other users.
 - We can't just pull user_id out of the session cookie. We're handling the
   AJAX requests as a JSON API, and those are generally stateless. Concretely,
   we're not fetching the session on the server.

An easy way to handle this would be to simply fetch our session in the API
pipeline. That works, but calling it "api" no longer really makes sense.

Instead, we're going to use an authentication token.

First we need to create the token and send it to the browser.

```
 # In .../plugs/fetch_user.ex

    if user do
      token = Phoenix.Token.sign(conn, "user_id", user.id)
      conn
      |> assign(:current_user, user)
      |> assign(:user_token, token)
    else
```

```
  <!-- in .../layouts/app.html.eex -->
  
    <script>
      window.userToken = "<%= @conn.assigns[:user_token] %>";
    </script>
  </head>
```

Then we need to send it with the request.

```
  // Add this to our AJAX call in voting.js / send_vote
  headers: {
    'x-auth': window.userToken,
  },
```

Then we can add a plug to fetch users for API requests based
on the auth header.

```
defmodule PhotoBlogWeb.Plugs.RequireAuth do
  import Plug.Conn

  def init(args), do: args

  def call(conn, _args) do
    token = Enum.at(get_req_header(conn, "x-auth"), 0)
    case Phoenix.Token.verify(conn, "user_id", token, max_age: 86400) do
      {:ok, user_id} ->
        assign(conn, :current_user, PhotoBlog.Users.get_user!(user_id))
      {:error, err} ->
        conn
        |> put_resp_header("content-type", "application/json; charset=UTF-8")
        |> send_resp(:unprocessable_entity, Jason.encode!(%{"error" => err}))
        |> halt()
    end
  end
end
```

```
  # In the router
  pipeline :api do
    plug :accepts, ["json"]
    plug PhotoBlogWeb.Plugs.RequireAuth
  end
```

```
  # In vote_controller.ex
  def create(conn, %{"vote" => vote_params}) do
    vote_params = vote_params
    |> Map.put("user_id", conn.assigns[:current_user].id)
```

So that works... once.

The new error is that once we've voted, we can't vote again. Let's
maintain that rule, but we want to allow users to change their
votes.

```
  # in votes.ex
  def create_vote(attrs \\ %{}) do
    %Vote{}
    |> Vote.changeset(attrs)
    |> Repo.insert(
      on_conflict: :replace_all,
      conflict_target: [:post_id, :user_id])
  end
```

That fixes that, next let's have the vote counts update live.

```
  # In vote_controller.ex
  def create ...
    ...
    with {:ok, %Vote{} = vote} <- Votes.create_vote(vote_params) do
      vote = Votes.load_post(vote)
```

```
  # In votes.ex
  def load_post(%Vote{} = vote) do
    vote = Repo.preload(vote, :post)
    post = PhotoBlog.Posts.load_votes(vote.post)
    %{ vote | post: post }
  end
```

```
  # In posts.ex
  def load_votes(%Post{} = post) do
    post = Repo.preload(post, :votes)
    score = post.votes
    |> Enum.map(&(&1.votes))
    |> Enum.sum()
    %{ post | score: score }
  end
```

Almost... but we're still not getting the post visible
in the response object. We'll need to update the view.

In .../views/vote_view.ex:

```
  def render("vote.json", %{vote: vote}) do
    post = PhotoBlogWeb.PostView.render_json(vote.post)
    %{
      id: vote.id,
      votes: vote.votes,
      post: post,
     }
  end
```

In .../views/post_view.ex:

```
defmodule PhotoBlogWeb.PostView do
  use PhotoBlogWeb, :view

  alias PhotoBlog.Posts.Post

  def render_json(%Post{} = post) do
    %{
      id: post.id,
      score: post.score
    }
  end
  
  def render_json(_missing), do: {}
end
```

Now we want to actually show the change.

```
function vote_done(resp, status, _xhr) {
    console.log("updated", status, resp);
    let post = resp.data.post;

    $(".post-card").each((_ii, card) => {
        let post_id = $(card).data('post-id');

        if (post_id == post.id) {
            $(card).find(".post-score").text(post.score);
        }
    });
}
```

Now we want to reorder the cards by vote count. How?

 - dom .remove()
 - Iterate through post-cards
 - .insertBefore() or .appendChild()
 - Google for these docs.
 - This would hurt way less with React.

