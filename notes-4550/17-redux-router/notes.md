
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
import Feed from "./Feed";
...
        <Route path="/" exact>
          <Feed />
        </Route>
```


```
// TODO: Feed.js
```
