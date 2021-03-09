---
layout: default
---

# First Thing

 - HW06/HW07 questions?

## Final Project

 - The final project assignments are posted.
 - You need to have a team by Nov 12th.
   - Everyone on the team email both TAs.
 - You'll working on a team of four to build something
   reasonably big.
 - Any application is fine, as long as it includes the required
   pieces and is appropriate to present to the class.

Required pieces:

 - Server side must be build with Elixir/Phoenix and must have
   significant application logic.
 - Your app needs to be deployed to a team member's VPS.
 - You must have user accounts.
 - You must store user accounts and some other important stuff
   in a Postgres DB.
 - Your app must use some external API that requires some form
   of authentication.
   - To meet this requirement, you need to do server <-> server
     API calls. This allows your trusted server code to control
     the API access, and allows you to update your application
     state (e.g. your DB) based on reliable API responses.
   - We'll talk more about APIs and authentication next week.
 - You must have at least two additional features.
 
Additional features:

 - Use Phoenix Channels
 - Learn and use a new Elixir library.
 - Learn and use a new JavaScript library.
 - Build your app mostly as an SPA
 - Use the HTML5 location API
 - You can propose other ideas

Note that there's no requirement on your browser code. You can write it in Elm,
or in C# through WebAssembly. You probably *should* have a browser interface
just for gradability. Testing something like an iPhone app might be annoying.

# Continuing the Lens SPA rewrite.

Last time:

 * We got React, Redux, and Bootstrap set up and working together.
 * We simulated multiple pages with React-Router.
 * We created all our resources.
 * We got photos to upload, but the app breaks trying to display
   the response.

## Here's where we left off

Now we want to make sure we handle the response.

First, let's update .../views/photo_view.ex:

```
  def render("photo.json", %{photo: photo}) do
    %{id: photo.id,
      user_id: photo.user_id,
      filename: photo.filename,
      desc: photo.desc,
      data: photo.data}
  end
```

Now we should be able to submit a photo and have it work.

We submit the photo, and nothing appears to happen. We need to do something once
we get back our response. We want to handle two cases:

 * Success - redirect to photo
 * Error - display error

In order to get this to work, we either need to render a <Redirect> tag from
React-Router or we need access to the React-Router history object.

For a Redirect tag, we want a stateful component:

 * We need to be able to update the state to cause the Redirect to be
   rendered.
 * We want that state go go away *immediately* so the redirect happens
   only once.

To get the history object, we need to access the React context. Again, that
means a class component.

The Redirect tag has an advantage: If we somehow navigate away from the
page before the redirect occurs, that will prevent the redirect.

So let's build a nested stateful component.

In .../photos/new.jsx:

```
import { Redirect } from 'react-router';
...
class PhotosNew extends React.Component {
  constructor (props) {
    super(props);
    
    this.state = {
      redirect: null,
    }
  }
  
  redirect(path) {
    this.setState({redirect: path});
  }

  changed(data) {
    this.props.dispatch({
      type: 'CHANGE_NEW_PHOTO',
      data: data,
    });
  }

  file_changed(ev) {
    let input = ev.target;
    let file  = null;
    if (input.files.length > 0) {
      file = input.files[0];
    }
    this.changed({file: file});
  }

  render() {
    let {file, desc, dispatch} = this.props;
    
    if (this.state.redirect) {
      return <Redirect to={this.state.redirect} />;
    }
    
    return (
      <div>
        <h1>New Photo</h1>
        <Form.Group controlId="upload">
          <Form.Label>Upload Photo</Form.Label>
          <Form.Control type="file" onChange={(ev) => this.file_changed(ev)} />
        </Form.Group>
        <Form.Group controlId="desc">
          <Form.Label>Description</Form.Label>
          <Form.Control as="textarea" rows="3"
                        onChange={(ev) => this.changed({desc: ev.target.value}) }/>
        </Form.Group>
        <Form.Group controlId="submit">
          <Button variant="primary" 
                  onClick={() => submit_new_photo(this)}>
            Upload Photo</Button>
        </Form.Group>
      </div>
    );
  }
}

// This needs to be at the bottom now, because obviously.
export default connect(state2props)(PhotosNew);
```

Now we need to update the redirect if we have success over in .../ajax.js:

```
export function submit_new_photo(form) {
...
  reader.addEventListener("load", () => {
    post('/photos', {
      photo: {
        desc: data.desc,
        filename: data.file.name,
        data: reader.result,
        user_id: 1,
      }
    }).then((resp) => {
      console.log(resp);
      if (resp.data) {
        store.dispatch({
          type: 'ADD_PHOTOS',
          data: [resp.data],
        });
        form.redirect('/photos/' + resp.data.id);
      }
      else {
        store.dispatch({
          type: 'CHANGE_NEW_PHOTO',
          data: {errors: JSON.stringify(resp.errors)},
        });
      }
    });
  });
```

Question: If the form is stateful anyway, why not put the form state in the form component
directly?

 * Do you want partially filled forms to surivive navigating away and back?
 * If so, that data needs to be in the main app state.
 * If not, having it in the local form component state makes sense.

### Rendering errors.

We're already dispatching errors to our state, so let's render them.

First add them to the default state store.js:

```
function new_photo(st0 = {file: null, desc: "", errors: null}, action) {
```

Then add them to the form itself in new.jsx:

```
  import { Form, Button, Alert } from 'react-bootstrap';
  ... 

  render() {
    let {file, desc, errors, dispatch} = this.props;
    let error_msg = null;
    
    if (errors) {
      error_msg = <Alert variant="danger">errors</Alert>;
    }
    ...

    return (
      <div>
        <h1>New Photo</h1>
        { error_message }
```

### Showing our photo

When we submit a photo, we get redirected to the /photos/:id path. We already
got the photo data back when we submitted it, so we shouldn't need more network
requests to show it.

Our code already dispatches a ADD_PHOTOS event, so next we should handle that
in the store.

In store.js:

```
function photos(st0 = new Map(), action) {
  switch (action.type) {
    case 'ADD_PHOTOS':
      let st1 = new Map(st0);
      for (let photo of action.data) {
        st1.set(photo.id, photo);
      }
      return st1;
    default:
      return st0;
  }
}
```

Some notes:

 * map New(old_map) makes a shallow copy.
 * This lets us build up a local collection of photos mirroring the
   photos table in the DB.
 * This is dupliate state. Our data can get old and outdated. We intentionally
   want to *not* try to store this data across hard page reloads.

Now let's create our new page.

In .../page.jsx:

```
import PhotosShow from './photos/show';
...
        <Route exact path="/photos/:id" render={({props}) =>
          <PhotosShow id={props.match.params.id} />
        } />
```

And let's show our image.

In .../js/photos/show.jsx:

```
import React from 'react';
import ReactDOM from 'react-dom';

import { connect } from 'react-redux';

function state2props(state, props) {
  return {photo: state.photos.get(parseInt(props.id))};
}

function PhotosShow({photo}) {
  let image = null;

  if (!photo) {
    return (
      <div>
        <h1>Show Photo</h1>
        <p>Loading...</p>
      </div>
    );
  }

  return (
    <div>
      <h1>Show Photo</h1>
      <img src={photo.data} />
      <p>Desc: {photo.desc}</p>
    </div>
  );
}

export default connect(state2props)(PhotosShow);
```

This works for photos we just uploaded, but doesn't work for photos
already present on the server before our session. We'll need to do
an HTTP request to get those.

Editing .../photo/show.jsx:

```
import { get_photo } from '../ajax';

function state2props(state, props) {
  let id = parseInt(props.id);
  return {id: id, photo: state.photos.get(id)};
}

function PhotosShow({id, photo}) {
  let image = null;

  if (!photo) {
    get_photo(id);
```

Now we need to add the function to actually do the get.

In .../ajax.js:

```
export function get(path) {
  return fetch('/ajax' + path, {
    method: 'get',
    credentials: 'same-origin',
    headers: new Headers({
      'x-csrf-token': window.csrf_token,
      'content-type': "application/json; charset=UTF-8",
      'accept': 'application/json',
    }),
  }).then((resp) => resp.json());
}

export function get_photo(id) {
  get('/photos/'+id)
    .then((resp) => {
        store.dispatch({
          type: 'ADD_PHOTOS',
          data: [resp.data],
        });
    });
}
```

Let's implement the main photo list.

.../photos/index.jsx:

```
import React from 'react';
import ReactDOM from 'react-dom';

import { connect } from 'react-redux';
import _ from 'lodash';

import { list_photos } from '../ajax';
import PhotoCard from './card';

let PhotosList = connect(({photos}) => ({photos}))(({photos}) => {
  if (photos.size == 0) {
    list_photos();
  }

let cards = _.map([...photos], ([_, photo]) => {
    return <PhotoCard key={photo.id} photo={photo} />;
  });

  return (
    <div>
      <h1>Photos</h1>
      <div className="row">
        {cards}
      </div>
    </div>
  );
});

export default PhotosList;
```

.../photos/card.jsx:

```
import React from 'react';
import ReactDOM from 'react-dom';

import { Card } from 'react-bootstrap';

export default function PhotoCard({photo}) {
  return (
    <Card className="col-4">
      <Card.Img src={photo.data} />
      <Card.Text>
        {photo.desc}
      </Card.Text>
    </Card>
  );
}
```

Add the route in .../page.jsx:

```
import PhotosList from './photos/index';
...
      <Switch>
        <Route exact path="/">
          <PhotosList />
        </Route>
```

Finally, we need a new fetcher in .../ajax.js:

```
export function list_photos() {
   get('/photos')
    .then((resp) => {
      console.log("list_photos", resp);
      store.dispatch({
        type: 'ADD_PHOTOS',
        data: resp.data,
      });
    });
}
```


# User Registration and Sessions

 * repo: https://github.com/NatTuck/lens
 * start branch: spa3-uploads
 * end branch: spa4-user-sessions

To get users working, we need two workflows:

 * Create a user
 * Log in as a user

Creating a user is just the same translation of a form from simple HTML to React +
AJAX that we did for photos, so we're going to skip it.

Logging in is a little bit different. We have options and design questions.

Option 1: Just use Phoenix sessions and cookies.

 * The user submits the login form
 * We retrieve the user and validate the password on the server
 * We use put_session(conn, :user\_id, user.id)
 * This sends a set-cookie header to the browser in the AJAX response.
 * The browser sends the cookie on subsequent requests.
 * The logout operation needs to hit the server to remove the cookie.

This works really nicely. Phoenix and the browser handle most of the logic, and
we get sessions that survive page reloads and browser restarts.

Option 2: Use a secure token.

 * The user submits the login form
 * We retrieve the user and validate the password on the server
 * We generate a token with Phoenix.Token
 * We send the token back as part of the response
 * The in-browser code saves the token in its state.
 * We send the token on subsequent requests.
 * The server side code needs to validate the token.
 * Logout is entirely a browser operation (just forget the token).

This works nicely too, but we need to put in extra effort to save the token
across hard reloads and browser restarts.

The token option is more flexible, because it doesn't rely on our browser
frontend being HTML generated by Phoenix. We can use the same logic in a couple
interesting cases:

 * We can make our browser frontend a completely separate application, generated
   using different tools like "create-react-app".
 * We don't even need the frontend to be in the browser. Our interface could be
   an Android app.
 * We could even have two seperate interfaces (e.g. browser and mobile app) that
   use the same API to talk to the server code.

So we're going to do the token thing, since that's new.

Let's start with a session controller in .../controllers/session_controller.ex:

```
defmodule LensWeb.SessionController do
  use LensWeb, :controller

  action_fallback LensWeb.FallbackController

  alias Lens.Users

  def create(conn, %{"email" => email, "password" => password}) do
    user = Users.authenticate_user(email, password)
    if user do
      token = Phoenix.Token.sign(conn, "session", user.id)
      resp = %{token: token, user_id: user.id, user_name: user.name}
      conn
      |> put_resp_header("content-type", "application/json; charset=UTF-8")
      |> send_resp(:created, Jason.encode!(resp))
    else
      resp = %{errors: ["Authentication Failed"]}
      conn
      |> put_resp_header("content-type", "application/json; charset=UTF-8")
      |> send_resp(:unauthorized, Jason.encode!(resp))
    end
  end
end
```

We need this new controller to be a path in our .../router.ex:

```
scope "ajax" ...
  resources "/sessions", SessionController, only: [:create], singleton: true
```

Over in lib/lens/users.ex:

```
  def authenticate_user(email, password) do
    user = Repo.get_by(User, email: email)
    case Argon2.check_pass(user, password) do
      {:ok, user} -> user
      _else       -> nil
    end
  end
```

Then we need a plug to require the user be authenticated.

In lib/lens\_web/plugs/require_auth.ex:

```
defmodule LensWeb.Plugs.RequireAuth do
  import Plug.Conn

  def init(args), do: args

  def call(conn, _args) do
    token = List.first(get_req_header(conn, "x-auth"))
    case Phoenix.Token.verify(LensWeb.Endpoint, "session", token, max_age: 86400) do
      {:ok, user_id} ->
        assign(conn, :current_user, Lens.Users.get_user!(user_id))
      {:error, err} ->
        conn
        |> put_resp_header("content-type", "application/json; charset=UTF-8")
        |> send_resp(:unprocessable_entity, Jason.encode!(%{"error" => err}))
        |> halt()
    end
  end
end
```

Now let's require that a user be authenticated to upload a photo.

In .../photo_controller.ex

```
  plug LensWeb.Plugs.RequireAuth when action in [:create, :update, :delete]
```

Now we need to handle sessions from the browser code.

First, let's add another element and the associated reducer to our store.

in .../store.js:

```
function session(st0 = null, action) {
  switch (action.type) {
    case 'LOG_IN':
      return action.data;
    case 'LOG_OUT':
      return null;
    default:
      return st0;
  }
}

...

function root_reducer(st0, action) {
  console.log("root reducer", st0, action);
  let reducer = combineReducers({
    ...
    session,
```

Then we need to actually create the UI.

In .../page.jsx:

```
import { Navbar, Nav, Col } from 'react-bootstrap';
...
import Login from './login';
...
      <Navbar bg="dark" variant="dark">
        <Col md="8">
          ...
        </Col>
        <Col md="4">
          <Session />
        </Col>
      </Navbar>
...
        <Route exact path="/login">
          <Login />
        </Route>
...
let Session = connect(({session}) => ({session}))(({session, dispatch}) => {
  function logout(ev) {
    ev.preventDefault();
    dispatch({
      type: 'LOG_OUT',
    });
  }

  if (session) {
    return (
      <Nav>
        <Nav.Item>
          <p className="text-light py-2">User: {session.user_name}</p>
        </Nav.Item>
        <Nav.Item>
          <a className="nav-link" href="#" onClick={logout}>Logout</a>
        </Nav.Item>
      </Nav>
    );
  }
  else {
    return (
      <Nav>
        <Nav.Item>
          <NavLink to="/login" exact activeClassName="active" className="nav-link">
            Login
          </NavLink>
        </Nav.Item>
      </Nav>
    );
  }
});
```

Then we need to create assets/js/login.jsx:

```
import React from 'react';
import ReactDOM from 'react-dom';

import { connect } from 'react-redux';
import { Form, Button, Alert } from 'react-bootstrap';
import { Redirect } from 'react-router';

import { submit_login } from './ajax';

class Login extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      redirect: null,
    };
  }

  redirect(path) {
    this.setState({
      redirect: path,
    });
  }

  changed(data) {
    this.props.dispatch({
      type: 'CHANGE_LOGIN',
      data: data,
    });
  }

  render() {
    if (this.state.redirect) {
      return <Redirect to={this.state.redirect} />
    }

    let {email, password, errors} = this.props;
    let error_msg = null;
    if (errors) {
      error_msg = <Alert variant="danger">{ errors }</Alert>;
    }

    return (
      <div>
        <h1>Log In</h1>
        { error_msg }
        <Form.Group controlId="email">
          <Form.Label>Email</Form.Label>
          <Form.Control type="text" onChange={
            (ev) => this.changed({email: ev.target.value})} />
        </Form.Group>
        <Form.Group controlId="password">
          <Form.Label>Password</Form.Label>
          <Form.Control type="password" onChange={
            (ev) => this.changed({password: ev.target.value})} />
        </Form.Group>
        <Form.Group controlId="submit">
          <Button variant="primary" onClick={() => submit_login(this)}>
            Log in
          </Button>
        </Form.Group>
      </div>
    );
  }
}

function state2props(state) {
  return state.forms.login;
}

export default connect(state2props)(Login);
```

We need to add the form to our state in .../store.js:

```
function login(st0 = {email: "", password: "", errors: null}, action) {
  switch(action.type) {
    case 'CHANGE_LOGIN':
      return Object.assign({}, st0, action.data);
    default:
      return st0;
  }
}

function forms(st0, action) {
  let reducer = combineReducers({
    new_photo,
    login,
```


Then we need a submit_login function over in ../ajax.js:

```
export function submit_login(form) {
  let state = store.getState();
  let data = state.forms.login;

  post('/sessions', data)
    .then((resp) => {
      if (resp.data) {
        store.dispatch({
          type: 'LOG_IN',
          data: resp.data,
        });
        form.redirect('/');
      }
      else {
        store.dispatch({
          type: 'CHANGE_LOGIN',
          data: {errors: JSON.stringify(resp.errors)},
        });
      }
    });
}
```

Now we can log in, but we have the expected problem: Our session is part of our
transient in-browser state, so we lose it on page reload.

We could try to use a client-side cookie, but cookies have a size limit and
we may end up making our session object bigger.

The browser provides another way to handle persistent client side state: local
storage.

There's an object in the global variable localStorage with four useful methods:

 * localStorage.setItem("key", "value") - save a string
 * localStorage.getItem("key") - get it back
 * localStorage.removeItem("key") - remove it
 * localStorage.clear() - delete all local storage

So we can persist our session there.

In .../ajax.js:

```
export function submit_login(form) {
  ...
    .then((resp) => {
      console.log(resp);
      if (resp.token) {
        localStorage.setItem('session', JSON.stringify(resp));
```

Then we can load that value as the default in our redux store.

In .../store.js:

```
let session0 = localStorage.getItem('session');
if (session0) {
  session0 = JSON.parse(session0);
}
function session(st0 = session0, action) {
```

And handle logout in .../page.jsx:

```
let Session = connect(({session}) => ({session}))(({session, dispatch}) => {
  function logout(ev) {
    ev.preventDefault();
    localStorage.removeItem('session');
    dispatch({
      type: 'LOG_OUT',
    });
  }
```

Now we have sessions in the browser, but we still can't upload photos.

We need to pass along our token with our requests.

In .../ajax.js

```
export function post(path, body) {
  let state = store.getState();
  let token = state.session.token;
  
    headers: new Headers({
      ...
      'x-auth': token || "",
}
...
export function get(path) {
  let state = store.getState();
  let token = state.session.token;

    headers: new Headers({
      ...
      'x-auth': token || "",
```

## More stuff to do

 - Make uploaded photos actually associated with @current_user
 - User registration
 - User list page
 - Tags
 - etc.

And that's our exciting discussion of single page applications.

Questions?


