---
layout: default
---

# First Thing

 - HW06 questions.
 - HW07 is posted; that's the last one before the final project.

## HW07

 - Rewrite your Timesheet app as an SPA
 - We'll be covering SPAs this week

## Project 2

 - HW07 is our last assignment before the final project.
 - You'll be writing an app that:
   - Is reasonably complex.
   - Uses an external API
     - API must be authenticated
     - Your app must have a reason to query the API during
       normal operation.
   - Uses a postgres database.
     - You need to store something useful in the DB.
 - Team Size: Up to 4
   - You get to pick your team
   - There is no backup plan to finding a team.
 - Start looking for a team.
 - Start thinking about app ideas.

# Single Page Apps

We're going to rewrite Lens to be a SPA.

Core properties of SPAs:

 - There's only one full page load. Apparent page loads are
   simulated in JavaScript.
 - All client <-> server communication after that first page
   load is (tradionally) by AJAX reqs or (sometimes now) over
   a websocket.

Why?

 - SPA style provides better control over the details of the user
   experience rather than leaving it to the browser.
 - SPAs can feel faster, especially due to applicion-aware caching
   of data.

Why not?

 - SPAs are generally slower on first load than normal webpages.
 - SPA frameworks exist, but the culture of SPA development tends to
   favor flexibility, so you frequently have to do a bunch of stuff by hand.
 - It's easy to break the back button and other stock browser UI - some tools
   don't give correct behavior by default. This makes sense, since it can be unclear
   what the correct behavior even *should be*.
 - There may not be Phoenix-style scaffolding that gives you error handling for
   free.

## Lens: Deps and Routes

 * git repo: https://github.com/NatTuck/lens
 * start branch: 1-deploys
 * end branch: spa1-setup

First, kill the awful ugly grey background in assets/css/app.scss.

Install a bunch of deps:

```
assets$ npm install --save react-router-dom react react-dom redux
assets$ npm install --save react-redux react-bootstrap deep-freeze-strict
assets$ npm install --save-dev @babel/preset-react
```

Add React support to the webpack config:

```
  module: {
    rules: [
      {
        test: /\.(js|jsx)$/,
        exclude: /node_modules/,
        use: {
          loader: 'babel-loader'
          options: {
            presets: ['@babel/preset-env', '@babel/preset-react']
          },
        },
      },
      ...
  },
  devtool: 'cheap-module-source-map',
  resolve: {
    extensions: ['.js', '.jsx', '.css', '.scss'],
  },
```

Let's create a simple React component just to demonstrate we're configured
correctly.

We're going to use React-Router to simulate pages.

 * Pages are handled entirely in the browser.
 * That means that our current path is part of our application state.
   * With React-Router, we store that state in the URL bar rather than
     in the root component state or the Redux store.
   * That's OK, but it's important to keep in mind.
 * We use React-Router and the path to decide which components to render -
   generally a component for each application page.
 * React-Router correctly handles some browser behaviors - like the Back
   button - that would otherwise break when simulating pages.

assets/js/page.jsx:

```
import React from 'react';
import ReactDOM from 'react-dom';
import { BrowserRouter as Router, Switch, Route, NavLink, Link } from 'react-router-dom';
import { Navbar, Nav } from 'react-bootstrap';

export default function init_page(root) {
  ReactDOM.render(<Page />, root);
}

function Page(props) {
  return (
    <Router>
      <Navbar bg="dark" variant="dark">
        <Nav>
          <Nav.Item>
            <NavLink to="/" exact activeClassName="active" className="nav-link">
              Home
            </NavLink>
          </Nav.Item>
          <Nav.Item>
            <NavLink to="/users" exact activeClassName="active" className="nav-link">
              Users
            </NavLink>
          </Nav.Item>
        </Nav>
      </Navbar>

      <Switch>
        <Route exact path="/">
          <h1>Home</h1>
        </Route>

        <Route exact path="/users">
          <h1>Users</h1>
        </Route>
      </Switch>
    </Router>
  );
}
```

.../app.js:

```
import init_page from './page';

window.addEventListener("load", () => {
  let root = document.getElementById('root');
  init_page(root);
});
```

.../templates/page/index.html.eex:

```
<div id="root">
  React app loading...
</div>
```

.../templates/layout/app.html.eex:

 * Remove the nav thing.

Problem: Page reloads 

 * Reloads break unless Phoenix knows about our paths.
 * Example: Click "users" then reload the page.
 * Solution: Add our paths to the Phoenix router, just loading
   our index page.
 * React-Router will figure out what to do from there.

.../router.ex:

```
    # Wildcard path must come *last*, including after
    # other scopes.
    get "/*path", PageController, :index
```


## Lens: Resources

branch: spa2-resources

Resources (all created with "mix phx.gen.json")

 * Users
 * Photos
   * We're going to put the photo in the DB.
   * No .../file path
   * No problems with atomicity and on-disk files.
   * Everything is base64 encoded, so 133% size.
   * Definitely a worse design.
 * Tags
 * Photo_Tags
 * Sessions
   * Only need :create; logging out happens client side.

Aside from photos and sessions, each of our resources will just have
the normal REST paths for AJAX interaction.

 * Branch: spa2-resources 
 * Just check out the branch and walk through changes.

For each resource:

 * Created with mix phx.gen.json
 * Edited migration to set null and on_delete correctly and add
   any unique indexes.
 * Added the route line to the "ajax" scope.
 * Edited schema to add associations and list correct fields in changeset.

Add some seeded users, with passwords.

 * Show priv/repo/seeds.exs
 * Show user.ex for password validation.

Add the dedup logic for tags and photo_tags.

Now we've got basic server side logic. We'll need some tweaks later, but this
should work for now.

## Lens: Photo Uploads

start branch: spa2-resources
end branch: spa3-uploads

First, we need to add a page to our application for uploading.

Add a route in page.jsx:

```
import PhotosNew from './photos/new';
...
  
  <Nav.Item>
    <NavLink to="/photos/new" exact activeClassName="active" className="nav-link">
      New Photo
    </NavLink>
  </Nav.Item>
...
  <Route exact path="/photos/new">
    <PhotosNew />
  </Route>
```

Now add our new template in .../js/photos/new.jsx:

```
import React from 'react';
import ReactDOM from 'react-dom';

import { Form, Button } from 'react-bootstrap';

export default function PhotosNew(props) {
  return (
    <div>
      <h1>New Photo</h1>
      <Form.Group controlId="upload">
        <Form.Label>Upload Photo</Form.Label>
        <Form.Control type="file" />
      </Form.Group>
      <Form.Group controlId="desc">
        <Form.Label>Description</Form.Label>
        <Form.Control as="textarea" rows="3" />
      </Form.Group>
      <Form.Group controlId="submit">
        <Button variant="primary">Upload Photo</Button>
      </Form.Group>
    </div>
  );
}
```

Problem: Need to be able to submit form.

This is React, so we should handle forms as follows:

 * Form content is part of our state.
   * Update state on field change.
   * Fields get value from state.
 * When we submit, we pull data from our state.

That means we need to get state.

## Lens: Client-Side State

Previously with React we've put our client side state in our root React
component. This solution doesn't scale as our app gets more complicated.

A common solution to handle state in more complex applications is a library
called Redux.

With Redux:

 - We take our single state object and move it out of our single React component
   into an object called the store.
 - We build code folling a pattern for updating that state based on events.
 - We're still required to keep our state immutable, but the redux pattern
   makes it more obvious how to do this.
 - The react-redux library feeds pieces of our state into our React components.

Let's set up Redux so we can handle our photo upload form.

assets/js/store.js:

```
import { createStore, combineReducers } from 'redux';
import deepFreeze from 'deep-freeze-strict';

/* Structure of store data:
 * {
 *   forms: {
 *     new_photo: {...},
 *     edit_photo: {...},
 *     new_user: {...},
 *     edit_user: {...},
 *   },
 *   users: Map.new(
 *     1 => {id: 1, name: "Alice", email: "alice@example.com"},
 *     ...
 *   ),
 *   photos: Map.new(
 *     1 => {id: 1, data: "...", desc: "...", tags: [...]},
 *     ...
 *   ),
 * }
 */

function new_photo(st0 = {file: null, desc: ""}, action) {
  switch (action.type) {
    case 'CHANGE_NEW_PHOTO':
      return Object.assign({}, st0, action.data);
    default:
      return st0;
  }
}

function forms(st0, action) {
  let reducer = combineReducers({
    new_photo,
  });
  return reducer(st0, action);
}

function users(st0 = Map.new(), action) {
  return st0;
}

function photos(st0 = Map.new(), action) {
  return st0;
}

function root_reducer(st0, action) {
  console.log("root reducer", st0, action);
  let reducer = combineReducers({
    forms,
    users,
    photos,
  });
  return deepFreeze(reducer(st0, action));
}

let store = createStore(root_reducer);
export default store;
```

Now we need to hook up Redux with React using React-Redux.

In .../page.jsx:

```
import { Provider, connect } from 'react-redux';
...
import store from './store';
...
export default function init_page(root) {
  let tree = (
    <Provider store={store}>
      <Page />
    </Provider>
  );
  ReactDOM.render(tree, root);
}
```

The Provider component allows us to access our Redux store from anywhere under
it in the tree. Since we have it as our root, that means anywhere in our JSX
code.

## Uploading a Photo

Now we can set up our form. Back in .../photos/new.jsx:

```
import { connect } from 'react-redux';

...

export default connect(state2props)(PhotosNew);

function state2props(state) {
  return state.forms.new_photo;
}

function PhotosNew({file, desc, dispatch}) {
```

So in our react component, we get three things:

 * The two fields of our form state.
 * One of them is a "file" object.
 * A "dispatch" function, which lets us send events to
   our redux store.

Let's hook up our form to the state:

```
function PhotosNew({file, desc, dispatch}) {
  function changed(data) {
    dispatch({
      type: 'CHANGE_NEW_PHOTO',
      data: data,
    });
  }

  function file_changed(ev) {
    let input = ev.target;
    let file  = null;
    if (input.files.length > 0) {
      file = input.files[0];
    }
    changed({file: file});
  }
  
  ...
        
    <Form.Control type="file" onChange={file_changed} />
  ...
    <Form.Control as="textarea" rows="3"
                  onChange={(ev) => changed({desc: ev.target.value}) }/>
```

Now we need to handle sending the photo to the server.

In the form:

```
import { submit_new_photo } from '../ajax';
  ... 
  <Button variant="primary" onClick={submit_new_photo}>Upload Photo</Button>
```

We lost our csrf token, so let's add that back (in .../layouts/app.html.eex):

```
    <script>
     window.csrf_token = "<%= Phoenix.Controller.get_csrf_token() %>";
    </script>
  </head>
```

Now we need to write our HTTP request function, in assets/js/ajax.js:

```
import store from './store';

export function post(path, body) {
   return fetch('/ajax' + path, {
    method: 'post',
    credentials: 'same-origin',
    headers: new Headers({
      'x-csrf-token': window.csrf_token,
      'content-type': "application/json; charset=UTF-8",
      'accept': 'application/json',
    }),
    body: JSON.stringify(body),
  }).then((resp) => resp.json());
}

export function submit_new_photo(done) {
  let state = store.getState();
  console.log("state", state);
  let data = state.forms.new_photo;

  if (data.file == null) {
    return;
  }

  let reader = new FileReader();
  reader.addEventListener("load", () => {
    post('/photos', {
      photo: {
        desc: data.desc,
        filename: data.file.name,
        data: reader.result,
        user_id: 1,
      }
    }).then((resp) => {
      console.log(resp)
    });
  });

  reader.readAsDataURL(data.file);
}
```

Then we want to make sure we handle the response.

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


## User Registration and Sessions

... Next time.

