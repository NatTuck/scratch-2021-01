
# Homework Questions?

# Single Page App

We've seen:

 - An app with logic fully in browser.
 - A single screen app with server-side logic.
 - A traditional multi-page web app with minimal
   in-browser logic.

But that leaves out one of the more popular patterns today:

 - A single page load
 - Core application logic on the server
 - Extensive in-browser code to fully control the UI / UX.

So we're going to completely write our PhotoBlog app using a Phoenix app on the
server exposing a JSON API and separate single-page React UI.

# Getting Started

```
$ mkdir photo-blog-spa && cd photo-blog-spa
$ mix phx.new photo_blog --no-webpack && mv photo_blog server
$ npx create-react-app photo-blog && mv photo-blog web-ui
$ rm -rf web-ui/.git
$ git init . && git add -A . && git commit -m 'created apps'
```

Repository: https://github.com/NatTuck/photo-blog-spa

## Branch: 01-create-resources

```
server$ mix phx.gen.json Users User users name:string password_hash:string
```

 - Migration: All non-null
 - Add to router
 - Update API path to "/api/v1"

```
server$ mix phx.gen.json Posts Post posts body:text photo_hash:string \
    user_id:references:users
```

 - Migration: all non-null
 - Schema:
   - belongs_to :user
   - Add :user_id to changeset
 - User schema: has_many :posts
 - Add to router

```
server$ mix phx.gen.json Comments Comment comments body:text \
    user_id:references:users post_id:references:posts
```
 
 - Migration: all non-null
 - Schemas:
   - belongs to user, post
   - Add user-id, post-id to changeset
   - user has many comments
   - post has many comments
 - Add to router
 
```
server$ mix phx.gen.json Votes Vote votes value:integer \
    user_id:references:users post_id:references:posts
```

 - Migration: all non-null
 - ```create index(:votes, [:user_id, :post_id])```
 - Schemas:
   - belongs to user, post
   - Add user-id, post-id to changeset
   - user has many votes
   - post has many votes

```
# votes.ex
def create_vote(...
    |> Repo.insert(
      on_conflict: :replace_all,
      conflict_target: [:post_id, :user_id])
```

That's our core resources, so:

```
$ mix ecto.migrate
```

Add some seeds for users:

```
# priv/repo/seeds.exs

alias PhotoBlog.Repo
alias PhotoBlog.Users.User

alice = Repo.insert!(%User{name: "alice", password_hash: ""})
bob = Repo.insert!(%User{name: "bob", password_hash: ""})
```

Update the DB:

```
$ mix ecto.reset
```

Now take a look at ```http://localhost:4000/api/v1/users```.

Next step: Get the data into our frontend app.

Replace web-ui/src/App.js with:

```
import { useState, useEffect } from 'react';

async function fetchUsers() {
  let text = await fetch("http://localhost:4000/api/v1/users", {});
  let resp = await text.json();
  return resp.data;
}

function App() {
  const [users, setUsers] = useState([]);

  useEffect(() => {
    if (users.length === 0) {
      fetchUsers().then((xs) => setUsers(xs));
    }
  }, [users]);

  return (
    <div>
      <ul>
        {users.map((uu) => (<li key={uu.id}>{uu.name}</li>))}
      </ul>
    </div>
  );
}

export default App;
```

That... didn't work. Log is complaining about CORS.

The rule here is that the *browser* will disallow HTTP requests from JavaScript
to our API unless our API is on the same host or the API host sends an
access-control-allow-origin header that matches the host that the JS code came
from.

In this case:

 - The Phoenix app providing the API is on http://localhost:4000/
 - The React app calling the API os on http://localhost:3000/
 - So the Phoenix app needs to send an accept-orign header for 
   "http://localhost:3000/"

Let's add a module to send that CORS header.

```
# in mix.exs
  defp deps do
    [
      ...
      {:cors_plug, "~> 2.0"},
```

Then let's add the plug we got to our app.

In .../endpoint.ex:

```
  ...
  plug CORSPlug
  # Goes right above
  plug Plug.MethodOverride
```

Now we'll configure it for development:

```
# in config/dev.exs
config :cors_plug,
  origin: ["http://localhost:3000"],
  max_age: 86400,
  methods: ["GET", "POST", "PATCH", "DELETE"]
```

## Branch: 02-simple-user-page

This UI isn't great, let's bootstrap it first.

```
web-ui$ npm install node-sass bootstrap react-bootstrap lodash
web-ui$ mv src/App.css src/App.scss
```

In App.js: ```include "./App.scss";```

Replace src/App.scss:

```
@import "~bootstrap/scss/bootstrap";
```

Now we can create a Users component to manage users.

Let's start with a listing:

```
import { useState } from 'react';
import { Row, Col, Form, Button } from 'react-bootstrap';
import capitalize from 'lodash/capitalize';

function Field({user, setUser, field}) {
  function update(ev) {
    let tmp = Object.assign({}, user);
    tmp[field] = ev.target.value;
    setUser(tmp);
  }

  return (
    <Form.Group>
      <Form.Label>{capitalize(field)}</Form.Label>
      <Form.Control type="text" onChange={update} value={user[field]||""} />
    </Form.Group>
  );
}

function UserForm({user, setUser}) {
  function onSubmit(ev) {
    ev.preventDefault();
    console.log(ev);
    console.log(user);
  }

  return (
    <Form onSubmit={onSubmit}>
      <Field user={user} setUser={setUser} field="name" />
      <Button variant="primary" type="submit">
        Save
      </Button>
    </Form>
  );
}

export default function Users({users}) {
  let [user, setUser] = useState({});

  let rows = users.map((user) => (
    <tr key={user.id}>
      <td>{user.name}</td>
      <td>
        <Button variant="secondary"
                onClick={() => setUser(user)}>
          Edit
        </Button>
      </td>
    </tr>
  ));

  return (
    <div>
      <Row>
        <Col>
          <h2>List Users</h2>
          <p>
            <Button variant="secondary"
                    onClick={() => setUser({})}>
              New User
            </Button>
          </p>
          <table className="table table-striped">
            <thead>
              <tr>
                <th>Name</th>
                <th>Actions</th>
              </tr>
            </thead>
            <tbody>
              { rows }
            </tbody>
          </table>
        </Col>
      </Row>
      <Row>
        <Col>
          <h2>Edit User</h2>
          <UserForm user={user} setUser={setUser} />
        </Col>
      </Row>
    </div>
  );
}
```

To include this in App.js:

```
import Users from "./Users";
...
  return (
    <Container>
      <Users users={users} />
    </Container>
  );
```

At this point we could continue with this setup, but we've got some
complications that we'd like to clean up:

 - We might be able to get away with putting list/show/new/edit functionality
   all on one screen for users. But we can't show every operation for everything
   at once, and probably should default to replicating the multi-page
   experience.
 - Having the in-browser application state spread around a bunch of components
   would be hard to maintain. We could use a better state management pattern.
 - We'd like to write some clean functions to expose the server API
   functionality.

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
