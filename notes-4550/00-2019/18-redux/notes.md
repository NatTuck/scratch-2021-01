+++
title = "./18-redux/notes.md"
+++

## Questions:

 - Upcoming assignments: Project 2, Redo or Enhance
 - Today's topic: Redux
 - HW08 Questions?

# Redux

 * start branch: 1901-redux-start
 * end branch:   1901-redux-done
 
## Starting point

I've done a little bit of cleanup to the app so we're working with an
easier to navigate program with some more stuff in it.

 - Components are split out into multiple modules.
 - I've added "add to cart" functionality.
 - Add to cart forms are part of the state, and we need to store
   \# of items in a (product\_id => count) map so we don't lose it
   on page changes.
 - Note that we're shallow-copying the map on update with new Map(old_map).
 - We always log in as Alice on load (this is ugly - should optimize sending
   initial state with cookie session).
 - Various feature fixes using stuff we've already seen (e.g. upsert for cart).
 

## The State Problem

One ongoing narritive in this class has been the importance of explicitly and
intentionally managing state in a distributed system.

This is a hard problem in general, and will be a source of a variety of bugs -
some of which will require significant redesigns - even when you're careful with
it.

One way of minimizing this problem is to have a single centralized state for the
whole application. In a traditional web app, this is the database. Going
further, the state should be "normalized", which is a formalization of the idea
of a "single source of truth". 

Having a single source of truth means that each piece of informaton should be
stored in exactly one place - and every access of update should synchronously
access that single copy. Benefits:

 * There are no extra copies of the data to get out of sync.
 * Consistency rules between multiple pieces of can be enforced in one place,
   and both pieces of data are stored in that place.

Unfortunately, having every access or update to any part of a distributed
application's state hit a single database can be slow. So we're frequently
forced to compomise the ideal. There are a couple of standard ways to do that:

 * Move some data out of the database. We still have a single source of truth,
   it's just not in the database.
   * e.g. React state in a user's browser
   * e.g. A GenServer
 * Caching. Create another copy of the data closer to where it will be used.
   * e.g. The list of products in the React state in our SPA.
   * Problem: Caches frequently contain old data. If you allow writes to caches,
     that can lead to data inconsistency / loss.
 * Accept the problems of distributed data management and have multiple sources
   of truth somehow. This path leads to madness and/or a PhD.

In our single-page HuskyShop we have our state split into two pieces: The
database and a single in-browser SPA application state in our root React
component.

Here's our in-browser application state:

```
    this.state = {
      // Two forms
      login_form: { email: "", password, "" },
      add_item_forms: new Map(), // { product_id => count }
      
      // Three local copies of server-side data.
      // These act as caches
      products: props.products, // List of Product
      users: [], // List of User
      cart: [], // List of CartItem 
      
      // One piece of browser-only, non-form state:
      session: null, // { token, user_id }
    }
```

 * Various events can update this state.
 * It's part of our root React component, so all of the update
   logic ends up there as well.
 * As our app gets more complicated, this will be a mess.
 * We could split our state into multiple stateful (class) React components,
   but that gives up the advantages of having a single centralized state in
   the browser (we already have two states, that's one more than we wanted).
 * One (possibly overkill) solution to this problem is Redux, which is based
   on the idea that centralizing state and state changes is a good idea even
   in a non-distributed app.

## Redux: Concept

 * Redux is primarily a set of patterns we can follow in our code
   to manage state.
 * Those patterns are supported by a couple of simple libraries containing a
   couple of useful functions.
 * We keep our single state object, but it moves out of our root react
   component.
 * Redux gives us an object called the store, which manages a single, deeply
   nested, immutable state.

To change our Redux state:

 * We create actions, simple JS value objects, which describe the event
   that's causing the state to change.
 * We write a single function - the "root reducer" - which goes 
   (old state, action) => new state.
 
Doing things this way gives us some advantages:

 * We have a clear, predefined structure for state updates.
 * Apparently Redux lets do time travel while debugging.
 
## Adding Redux to HuskyShop 

Let's add our new libraries:

```
assets$ npm install --save redux react-redux deep-freeze
```

Write **store.js**, including logic to update add cart form counts:

```
import { createStore, combineReducers } from 'redux';
import deepFreeze from 'deep-freeze';

/*
  Application state layout
  {
    // Session
    session: null, // { token, user_id }
    
    // DB Caches
    products: props.products, // List of Product
    users: [], // List of User
    cart: [], // List of CartItem 
    
    // Forms
    login_form: { email: "", password, "" },
    add_item_forms: new Map(), // { product_id => count }
  }
*/

// For each component of the state:
//  * Function with the same name
//  * Default is the default value of that component

function products(state = [], action) {
  return state;
}

function users(state = [], action) {
  return state;
}

function cart(state = [], action) {
  return state;
}

function session(state = null, action) {
  return state;
}

let login_form0 = {email: "", password: ""};
function login_form(state = login_form0, action) {
  return state;
}

function add_item_forms(state = new Map(), action) {
  switch (action.type) {
  case 'UPDATE_ADD_CART_FORM':
    let state1 = new Map(state);
    state1.set(action.product_id, action.count);
    return state1;
  default:
    return state;
  }
}

function root_reducer(state0, action) {
  console.log("reducer", state0, action);

  let reducer = combineReducers({products, users, cart, session, 
                                 login_form, add_item_forms});
  let state1 = reducer(state0, action);

  console.log("reducer1", state1);

  return deepFreeze(state1);
}

let store = createStore(root_reducer);
export default store;
```

Update app.js:

```
import root_init from "./root";
import store from './store';

$(() => {
  let node = $('#root')[0];
  root_init(node, store);
});
```

Update root.jsx:

```
import { Provider } from 'react-redux';
...
export default function root_init(node, store) {
  ReactDOM.render(
    <Provider store={store}>
      <Root products={window.products} />
    </Provider>, node);
}
```

Update product_list.jsx to use Redux for form state:

```
import React from 'react';
import { connect } from 'react-redux'; // <=
import _ from 'lodash';

function ProductList(props) {
  let {products, counts, dispatch} = props;
  let prods = _.map(products, (pp) =>
    <Product key={pp.id} dispatch={dispatch}          // <=
             product={pp} count={counts.get(pp.id)} />
  );
  return <div className="row">
    {prods}
  </div>;
}

function Product(props) {
  let {product, root, count, dispatch} = props;
  function count_changed(ev) {  // <=
    let action = {
      type: 'UPDATE_ADD_CART_FORM',
      product_id: product.id,
      count: ev.target.value,
    };
    dispatch(action);
  }
  return <div className="card col-4">
    <div className="card-body">
      <h2 className="card-title">{product.name}</h2>
      <p className="card-text">
        {product.desc} <br />
        price: {product.price}
      </p>
      <p className="form-inline">
        <input className="form-control" value={count||1} type="number"
               style={\{width: "8ex"}} onChange={count_changed} />
        <button className="btn btn-primary"
                onClick={() => root.add_to_cart(product.id)}>
          Add to Cart
        </button>
      </p>
    </div>
  </div>;
}

function state2props(state) { // <=
  console.log("rerender", state);
  return {
    //products: state.products,
    counts: state.add_item_forms,
  };
}

// Export result of curried function call.
export default connect(state2props)(ProductList); // <=
```

That's the basic pattern. Now that we have it, we can move the rest of our state
management out of our root React component.

That includes all the AJAX logic. Let's bring that out to its own file.

**api.js**:

```
import store from './store';

class TheServer {
  fetch_path(path, callback) {
    $.ajax(path, {
      method: "get",
      dataType: "json",
      contentType: "application/json; charset=UTF-8",
      data: "",
      success: callback,
    });
  }

  fetch_products() {
    this.fetch_path(
      "/api/v1/products",
      (resp) => {
        store.dispatch({
          type: 'PRODUCT_LIST',
          data: resp.data,
        });
      }
    );
  }

  fetch_users() {
    this.fetch_path(
      "/api/v1/users",
      (resp) => {
        store.dispatch({
          type: 'USER_LIST',
          data: resp.data,
        });
      }
    );
  }

  fetch_cart() {
    // TODO: Pass user_id to server
    this.fetch_path(
      "/api/v1/cart_items",
      (resp) => {
        store.dispatch({
          type: 'CART_LIST',
          data: resp.data,
        });
      }
    );
  }

  send_post(path, data, callback) {
    $.ajax(path, {
      method: "post",
      dataType: "json",
      contentType: "application/json; charset=UTF-8",
      data: JSON.stringify(data),
      success: callback,
    });
  }

  create_session(email, password) {
    this.send_post(
      "/api/v1/auth",
      {email, password},
      (resp) => {
        store.dispatch({
          type: 'NEW_SESSION',
          data: resp.data,
        });
      }
    );
  }

  add_to_cart(product_id) {
    let state = store.getState();
    let user_id = state.session.user_id;
    let count = state.add_item_forms.get(product_id) || 1;
    console.log("add to cart", state);
    this.send_post(
      "/api/v1/cart_items",
      {cart_item: {product_id, user_id, count}},
      (resp) => {
        this.fetch_cart();
      },
    );
  }
}

export default new TheServer();
```

To hook up the API, in root.jsx:

```
import api from './api';
...
class Root extends React.Component {
  constructor(props) {
    super(props);

    api.create_session("bob@example.com", "pass1");
    api.fetch_products();
    api.fetch_users();
    api.fetch_cart();
  }

  render() {
    return <div>
      <Router>
        <div>
          <Header />
          <div className="row">
            <div className="col-8">
              <Route path="/" exact={true} render={() =>
                <ProductList />
              } />
              <Route path="/users" exact={true} render={() =>
                <UserList />
              } />
            </div>
            <div className="col-4">
              <Cart />
            </div>
          </div>
        </div>
      </Router>
    </div>;
  }
}
```

Update the header:

```
import React from 'react';
import _ from 'lodash';
import { Link } from 'react-router-dom';
import { connect } from 'react-redux';

function Header(props) {
  let {session} = props;
  let session_info;
  if (session == null) {
    session_info = <div className="form-inline my-2">
      <input type="email" placeholder="email" />
      <input type="password" placeholder="password" />
      <button className="btn btn-secondary">Login</button>
    </div>;
  }
  else {
    session_info = <div className="my-2">
      <p>Logged in as {session.user_id}</p>
    </div>
  }

  return <div className="row my-2">
    <div className="col-4">
      <h1>Husky Shop</h1>
    </div>
    <div className="col-4">
      <p>
        <Link to={"/"}>Products</Link> &nbsp; | &nbsp;
        <Link to={"/users"}>Users</Link>
      </p>
    </div>
    <div className="col-4">
      {session_info}
    </div>
  </div>;
}

function state2props(state) {
  return { session: state.session };
}

export default connect(state2props)(Header);
```

Handle the NEW\_SESSION action in our reducer:

```
function session(state = null, action) {
  switch (action.type) {
  case 'NEW_SESSION':
    return action.data;
  default:
    return state;
  }
}
```

Update **user\_list.jsx**:

```
import { connect } from 'react-redux';
...
function UserList(props) { // no export default
...
export default connect((state) => {return {users: state.users};})(UserList);
```

Handle the USER\_LIST (and PRODUCT\_LIST) action in our reducer (**store.js**):

```
function products(state = [], action) {
  switch (action.type) {
  case 'PRODUCT_LIST':
    return action.data;
  default:
    return state;
  }
}

function users(state = [], action) {
  switch (action.type) {
  case 'USER_LIST':
    return action.data;
  default:
    return state;
  }
}
```

Update **product\_list.jsx**:

```
function state2props(state) {
  console.log("rerender", state);
  return {
    products: state.products,  // <=
    counts: state.add_item_forms,
  };
}
```

Let's fix the rest of **store.js**:

```
function cart(state = [], action) {
  switch (action.type) {
  case 'CART_LIST':
    return action.data;
  default:
    return state;
  }
}
```

Let's make add-to-cart work, in **product\_list.js**:

```
import api from './api';
...
        <button className="btn btn-primary"
                onClick={() => api.add_to_cart(product.id)}>
```

And in **cart.jsx**:

```
import { connect } from 'react-redux';
...
// replaces export default Cart:
export default connect((state) => {return {cart: state.cart};})((props) => {
  ...
});
```

Add delete cart item, in **cart.jsx**:

```
import api from './api';
...
    <td><button className="btn btn-default"
                onClick={() => api.delete_cart_item(item.id)}>Remove</button></td>
```

In **api.js**:

```
  delete_cart_item(id) {
    $.ajax('/api/v1/cart_items/' + id, {
      method: "delete",
      dataType: "json",
      contentType: "application/json; charset=UTF-8",
      data: "",
      success: (resp) => {
        store.dispatch({
          type: 'CART_DELETE',
          cart_item_id: id,
        });
      }
    });
  }
```

In **store.js**:

```
function cart(state = [], action) {
  switch (action.type) {
  case 'CART_LIST':
    return action.data;
  case 'CART_DELETE':
    return _.filter(state, (item) => item.id != action.cart_item_id);
  default:
    return state;
  }
}
```

## Cleanup

 * Delete the root react component state.
 * Delete all references to root={this} passed to components.
 * Make sure user list works.

