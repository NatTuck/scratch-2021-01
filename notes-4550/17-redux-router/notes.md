
# Homework Questions?

## Branch: 03-redux-state

 - Currently we have our main in-browser state spread between multiple
   components.
 - In the game assignment, we had a single in-browser state in our
   root component. That had tradeoffs;
   - Advantages: Easier to manage the state itself.
   - Disadvantage: Need to thread state and update functions into
     nested components.

A popular solution for React apps is to factor the state out into a separate
module where a single state and state updates are handled explicitly. A commonly
used library is Redux, which provides a couple of very simple functions and an
associated structure to support this plan.

```
web-ui$ npm install redux react-redux
```

Create a new file, store.js:

```
import { createStore, combineReducers } from 'redux';

function users(state = [], action) {
    switch (action.type) {
    case 'users/set':
        return action.data;
    default:
        return state;
    }
}

function user_form(state = {}, action) {
    switch (action.type) {
    case 'user_form/set':
        return action.data;
    default:
        return state
    }
}

function root_reducer(state, action) {
    console.log("root_reducer", state, action);
    let reducer = combineReducers({
        users, user_form
    });
    return reducer(state, action);
}

let store = createStore(root_reducer);
export default store;
```

Now add/update the following in index.js:

```
import { Provider } from 'react-redux';
...
import store from './store';
import { load_defaults } from './api';
...
ReactDOM.render(
  <React.StrictMode>
    <Provider store={store}>
      <App />
    </Provider>
  </React.StrictMode>,
  document.getElementById('root')
);

load_defaults();
```

Move over fetchUsers from App.js to api.js, which ends up like this:

```
import store from './store';

export async function api_get(path) {
    let text = await fetch("http://localhost:4000/api/v1" + path, {});
    let resp = await text.json();
    return resp.data;
}

export function fetch_users() {
    api_get("/users").then((data) => store.dispatch({
        type: 'users/set',
        data: data,
    }));
}

export function load_defaults() {
    fetch_users();
}
```

That lets App.js turn into this:

```
import { Container } from 'react-bootstrap';

import "./App.scss";
import Users from "./Users";

function App() {
  return (
    <Container>
      <Users />
    </Container>
  );
}

export default App;
```

And update Users.js like this:

```
// Remove the useState import.
import { connect } from 'react-redux';

...
function Users({users, user_form, dispatch}) {
  // No useState
  function setUser(user) {
    dispatch({type: 'user_form/set', data: user});
  }
 
          <h2>Edit User</h2>
          <UserForm user={user_form} setUser={setUser} />
...
export default connect(({users, user_form}) => ({users, user_form}))(Users);
```

## Branch: 04-router

We want to simulate multiple pages in our application. We could do this by
adding an explicit "current page" field to our state, but there end up being a
bunch of stuff we'd need to do to create a good multi-page simulation. For
example, the back button and URL bar should behave correctly.

The "react-router" module handles all of this for us.

```
web-ui$ npm install react-router-dom
```

Now let's update our app to have multiple pages:

```
// index.js
import { BrowserRouter as Router } from 'react-router-dom';
...
    <Provider store={store}>
      <Router>
        <App />
      </Router>

```

```
// App.js
import { Switch, Route } from 'react-router-dom';
...
import Nav from "./Nav";
...
    <Container>
      <Nav />
      <Switch>
        <Route path="/" exact>
          <h1>PhotoBlog Feed</h1>
        </Route>
        <Route path="/users">
          <Users />
        </Route>
      </Switch>
    </Container>
```

```
// Nav.js
import { Nav } from 'react-bootstrap';
import { NavLink } from 'react-router-dom';

function Link({to, children}) {
  return (
    <Nav.Item>
      <NavLink to={to} exact className="nav-link" activeClassName="active">
        {children}
      </NavLink>
    </Nav.Item>
  );
}

export default function AppNav() {
  return (
    <Nav variant="pills">
      <Link to="/">Feed</Link>
      <Link to="/users">Users</Link>
    </Nav>
  );
}
```

Ok, time to add our Feed!

```
// App.js
import Feed from "./Feed";
...
        <Route path="/" exact>
          <Feed />
        </Route>
```

And the new file:

```
// Feed.js
import { Row, Col, Card } from 'react-bootstrap';
import { connect } from 'react-redux';

function photo_path(post) {
  return post.photo_hash;
}

function Post({post}) {
  return (
    <Col>
      <Card>
        <Card.Img variant="top" src={phot o_path(post)} />
        <Card.Text>
          Posted by {post.user.name}<br />
          {post.body}
        </Card.Text>
      </Card>
    </Col>
  );
}

function Feed({posts}) {
  let cards = posts.map((post) => <Post post={post} key={post.id} />);
  return (
    <Row>
      { cards }
    </Row>
  );
}

export default connect(({posts}) => ({posts}))(Feed);
```

For that to work, we need to fetch photos.

```
// api.js
...
export function fetch_posts() {
  api_get("/posts").then((data) => store.dispatch({
    type: 'posts/set',
    data: data,
  }));
}

export function load_defaults() {
  fetch_users();
  fetch_posts();
}
```

And update the store to handle the new event.

```
function posts(state = [], action) {
  switch (action.type) {
    case 'posts/set':
      return action.data;
    default:
      return state;
  }
}

function root_reducer(state, action) {
...
  let reducer = combineReducers({
    users, user_form, posts
  });
```

One problem: no posts in the DB. Let's add some to the seeds file.

  * Copy over photos to priv/photos
  
Now let's rewrite that photos module from the old version, and see if
we can fix that Mix.env nonsense from before.

```
# In config/config.exs

config :photo_blog,
  mix_env: "#{Mix.env()}"
```

Let's just copy this over from the latest commit
in the old git repo.

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
    # HERE's THE FIX
    Path.expand("~/.local/data/photo_blog")
    |> Path.join(Application.get_env(:photo_blog, :mix_env))
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

Copy over the seeds file from the old version:

 * Make sure to add password_hash: "" to users.

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

alice = Repo.insert!(%User{name: "alice", password_hash: ""})
bob = Repo.insert!(%User{name: "bob", password_hash: ""})

elephant = Inject.photo("elephant.jpg")
moon = Inject.photo("moon.jpg")

Repo.insert!(%Post{user_id: alice.id, photo_hash: elephant, body: "Alice says Hi!"})
Repo.insert!(%Post{user_id: bob.id, photo_hash: moon, body: "Bob says garblarg!"})
```

Looks like we need to preload and render post.user.

```
# posts.ex
  def list_posts do
    Repo.all(Post)
    |> Repo.preload(:user)
  end

  def get_post!(id) do
    Repo.get!(Post, id)
    |> Repo.preload(:user)
  end
```

```
defmodule PhotoBlogWeb.PostView do
  ...
  alias PhotoBlogWeb.UserView
  ...
  def render("post.json", %{post: post}) do
    %{
      id: post.id,
      body: post.body,
      photo_hash: post.photo_hash,
      user: render_one(post.user, UserView, "user.json"),
    }
  end
```

Next problem: We need to actually show photos.

```
defmodule PhotoBlogWeb.PageController do
  use PhotoBlogWeb, :controller

  alias PhotoBlog.Photos

  def index(conn, _params) do
    posts = PhotoBlog.Posts.list_posts()
    |> PhotoBlog.Posts.load_votes()
    render(conn, "index.html", posts: posts)
  end


  def photo(conn, %{"hash" => hash}) do
    {:ok, _name, data} = Photos.load_photo(hash)
    conn
    |> put_resp_content_type("image/jpeg")
    |> send_resp(200, data)
  end
end
```

Add that stuff to the Phoenix router:

```
  scope "/", PhotoBlogWeb do
    get "/", PageController, :index
    get "/photos/:hash", PageController, :photo
  end
```

```
// Feed.js
function photo_path(post) {
  return "http://localhost:4000/photos/" + post.photo_hash;
}
```

## Branch: 05-sessions

 * Sessions are just one route: POST to create a session
 * state.session is { name, user_id, token }
 * To log out, just null out that part of the state

```
  # router.ex
  scope "/api/v1", PhotoBlogWeb do
    ...
    resources "/session", SessionController, only: [:create]
```

```
defmodule PhotoBlogWeb.PostController do
  use PhotoBlogWeb, :controller

  def create(conn, %{"name" => name, "password" => password}) do
    user = PhotoBlog.Users.get_user_by_name!(name)
    # TODO: Verify password
    sess = %{
      user_id: user.id,
      name: user.name,
      token: Phoenix.Token.sign(conn, "user_id", user.id),
    }
    conn
    |> put_resp_header("content-type", "application/json; charset=UTF-8")
    |> send_resp(:created, Jason.encode!(sess))
  end
end
```

TODO:

 * Add store session component, default null
 * Add conditional display of login form / session info.
 * Create api.js#get_session
 * Add https://github.com/riverrun/argon2_elixir
 
