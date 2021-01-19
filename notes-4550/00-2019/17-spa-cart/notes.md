+++
title = "./17-spa-cart/notes.md"
+++

# First Thing

 - HW08 Questions?
 - Project Questions?

## Filling out our SPA

 * start: 3-list-products-fixed
 * end: 1901-5-users
 
Lecture ends at 1:25. Goal is to cover three topics:

 * Simulating sessions with Phoenix.Token
 * Adding a list users page; simulating pages with React Router
 * Adding items to the shopping cart.

### Single-page "Sessions"

(git branch: 1901-4-sessions)

 - We could use cookies / Phoenix sessions as shown in the product
   ratings example.
 - Problem 1: We need to solve cross-site scripting security issues
   and we can't use the Phoenix form helpers since we're not submitting
   forms.
 - Problem 2: This would prevent our API from being used by things other
   than our web interface unless the other clients simulate cookie-based
   sessions.
 - Solution: Phoenix.Token
   - When the user logs in, we issue them a cryptographically signed token.
   - The token is valid for a limited time.
   - Logout is just the client throwing away the token.
   - We include the token with all API actions that require authentication.

Step 1: Upgrade to the latest Argon2 password hash library.

In mix.exs:

```
      {:argon2_elixir, "~> 2.0"},
      # Make sure "comeonin" doesn't show up here.
      # Remove mix.lock before running mix deps.get
```

Step 2: Create an authentication function in the Users context module.

In users/users.ex:

```
  @doc """
  Authenticates a user.

  Returns {:ok, user} on success, or {:error, msg} on failure.
  """
  def authenticate_user(email, password) do
    Repo.get_by(User, email: email)
    |> Argon2.check_pass(password)
  end
```

Step 3: Create an AuthController

 * Show auth_controller.ex
 
Add the new case to the fallback controller:

```
  def call(conn, {:error, "invalid password"}) do
    conn
    |> put_resp_header("content-type", "application/json; charset=UTF-8")
    |> send_resp(:unprocessable_entity, Jason.encode!(%{error: "auth failed"})
  end
```

Add it to the router:

router.ex:

```
  scope "/api/v1", HuskyShopWeb do
    ...
    post "/auth", AuthController, :authenticate
```

Step 4: Add a plug to check for an X-Auth header.

\_web/plugs/require\_auth.ex

 * (show require_auth.ex)

Now we can send authenticated AJAX requests by adding an X-Auth header with our
token in it.

```
$.ajax(..., {
  ...
  headers: {"X-Auth": token},
});
```


Step 5: Make the login form an AJAX request.

Bring in root-auth1.jsx as root.jsx, point out:

 * Session and login_form in state.
 * login method
 * update login form method
 * Args to header in render
 * New header component

## Simulating Pages

(1901-5-users)

 * We want multiple pages.
 * We could store the current page in the state, but that gets annoying.
 * Alternate plan: Use a library called react-router to implictly add
   pages to our state.

```
assets$ npm install --save react-router-dom
```

See the react-router docs for more examples - it's not that much stuff.

Add routes to our app in root.jsx:

```
import { Link, BrowserRouter as Router, Route } from 'react-router-dom';
...
class Root ...
  constructor(props) {
    ... 
    this.state = {
      users: [],
  ...
  render() {
    return <div>
      <Router>
        <div>
          <Header ... />
          <Route path="/" exact={true} render={() =>
            <ProductList products={this.state.products} />
          } />
          <Route path="/users" exact={true} render={() =>
            <UserList users={this.state.users} />
          } />
        </div>
      </Router>
    </div>;
  }
...
function Header(_props) {
  return <div className="row my-2">
    <div className="col-4">
      <h1><Link to={"/"}>Husky Shop</Link></h1>
    </div>
    <div className="col-4">
      <p>
        <Link to={"/"}>Products</Link> |
        <Link to={"/users"}>Users</Link>
      </p>
    </div>
    <div className="col-4">
      ...
...
function UserList(props) {
  let rows = _.map(props.users, (uu) => <User key={uu.id} user={uu} />);
  return <div className="row">
    <div className="col-12">
      <table className="table table-striped">
        <thead>
          <tr>
            <th>email</th>
            <th>admin?</th>
          </tr>
        </thead>
        <tbody>
          {rows}
        </tbody>
      </table>
    </div>
  </div>;
}

function User(props) {
  let {user} = props;
  return <tr>
    <td>{user.name}</td>
    <td>{user.admin ? "yes" : "no"}</td>
  </tr>;
}
```

Add a fetch_users method to the root component:

```
  fetch_users() {
    $.ajax("/api/v1/users", {
      method: "get",
      dataType: "json",
      contentType: "application/json; charset=UTF-8",
      data: "",
      success: (resp) => {
        let state1 = _.assign({}, this.state, { users: resp.data });
        this.setState(state1);
      }
    });
  }
```

Call the fetch_users method when "users" link clicked:

```
function Header(props) {
    // Need to pass this in from the Root#render
    let {root} = props;
    ...
    <div className="col-4">
      <h1><Link to={"/"} onClick={root.fetch_products.bind(root)}>Husky Shop</Link></h1>
    </div>
    <div className="col-2">
      <p><Link to={"/users"} onClick={root.fetch_users.bind(root)}>Users</Link></p>
    </div>
```

Problem: Directly loading /users leads to no users shown.

Solutions:

 - Load all resources in Phoenix template.
 - Have separate Phoenix pages to preload different resources.
 - Just call fetch_users in the root constructor. The JS code is probably
   cached anyway since this isn't the root path, and the extra fetch when
   loading other paths is acceptable.
 - Fetch from JS conditionally based on router path.

### Shopping Cart (no auth)

(branch: Refer to 6-cart-items)

Let's create our shopping cart.

In root.jsx:

 * ```import Cart from './cart'```
 * Add ```cart: []``` to state
 * In Root#render, split below the header into col-8 main and col-4 cart.

```
 <div className="row">
   <div className="col-8">
     // routes
   </div>
   <div className="col-4">
      <Cart root={this} cart={this.state.cart} />
   </div>
 </div>
```

**cart.jsx:**

```
import React from 'react';
import _ from 'lodash';

export default function Cart(props) {
  let {root, cart} = props;
  let items = _.map(cart, (item) => <CartItem key={item.id} item={item} root={root} />);
  return <div>
    <h2>Shopping Cart</h2>
    <ul>
      {items}
    </ul>
    <button className="btn btn-primary">Check Out</button>
  </div>;
}

function CartItem(props) {
  let {root, item} = props;
  return <li>
    {item.count} - {item.product.name} (
    <button className="btn btn-default"
             onClick={() => root.remove_cart_item(item.id)}>remove</button>)
  </li>;
}
```

Add to cart button

```
  // root, Product:
  // need to thread through root
      <p className="form-inline">
        <input className="form-control" style={\{width: "8ex"}}
               type="number" defaultValue="1" id={"item-count-" + product.id} />
        <button className="btn btn-primary"
                onClick={() => root.add_to_cart(product.id)}>
          Add to Cart
        </button>
      </p>
```

Add to cart method in Root

```
  send_post(path, req, on_success) {
    $.ajax(path, {
      method: "post",
      dataType: "json",
      contentType: "application/json; charset=UTF-8",
      data: JSON.stringify(req),
      headers: {"X-Auth": this.state.session.token},
      success: on_success,
    });
  }

  add_to_cart(product_id) {
    let user_id = this.state.session.user_id;
    let count = $('#item-count-' + product_id).val();
    this.send_post(
      "/api/v1/cart_items",
      {product_id, user_id, count},
      (resp) => {
        let cart1 = _.concat(this.state.cart, [resp.data]);
        let state1 = _.assign({}, this.state, { cart: cart1 });
        this.setState(state1);
      }
    );
  }
```

Need to fix changeset function in cart_item schema:

```
  def changeset(cart_item, attrs) do
    cart_item
    |> cast(attrs, [:count, :user_id, :product_id])
    |> unique_constraint(:user_id, name: :cart_items_user_id_product_id_index)
    |> validate_required([:count, :user_id, :product_id])
  end
```

Need to fetch updated cart items:

 * Create by default returns the item created.
 * We're missing some fields, let's add them.

cart\_item\_view.ex:

```
  def render("cart_item.json", %{cart_item: cart_item}) do
    product = HuskyShopWeb.ProductView.render(
      "product.json", %{product: cart_item.product})
    %{
      id: cart_item.id,
      count: cart_item.count,
      product: product,
      user_id: cart_item.user_id,
    }
  end
```

We need to preload the products field.

In the cart\_items context:

```
  def get_cart_item!(id) do
    Repo.one! from ci in CartItem,
      where: ci.id == ^id,
      preload: [:product]
  end
```

Fetch the full item in cart\_item\_controller / create:

```
  def create(conn, %{"cart_item" => cart_item_params}) do
  ...
      cart_item = CartItems.get_cart_item!(cart_item.id)
```

Problem: Don't see initial cart items.

Solution: Fetch on initial load.

```
  fetch_path(path, on_success) {
    $.ajax(path, {
      method: "get",
      dataType: "json",
      contentType: "application/json; charset=UTF-8",
      data: "",
      success: on_success,
    });
  }

  fetch_cart() {
    this.fetch_path(
      "/api/v1/cart_items",
      (resp) => {
        let state1 = _.assign({}, this.state, { cart: resp.data });
        this.setState(state1);
      }
    );
  }
```

We need to preload for list_cart\_items().

```
  def list_cart_items do
    Repo.all from ci in CartItem,
      preload: [:product]
  end
```

Remove cart item method in root:

```
  remove_cart_item(id) {
    $.ajax("/api/v1/cart_items/" + id, {
      method: "delete",
      dataType: "json",
      contentType: "application/json; charset=UTF-8",
      data: "",
      success: (_resp) => {
        let cart1 = _.filter(this.state.cart, (item) => item.id != id);
        let state1 = _.assign({}, this.state, { cart: cart1 });
        this.setState(state1);
      }
    });
  }
```

Validating the token for auth.

```
# This goes at the top of CartItemController
plug HuskyShopWeb.Plugs.RequireAuth when action in [:create, :update, :delete]
```

### Reminder: Forms as State

Problem: If we enter a count for an item in the add to cart form, change pages,
and change back, we lose the state.

Solution: Remember to explicitly add the value of all forms to the React state.

Our state for add-to-cart forms should be a map of (product_id => count).

JavaScript objects don't support integer keys, so we can't use them. ES6 adds a
Map type, which supports integer keys.

```
   constructor(...
     // New addition to state
     add_cart_forms: new Map(),
     ...
  
     // Fetch products in constructor 
     this.fetch_products();
     ...
 
   fetch_products() {
    this.fetch_path(
      "/api/v1/products",
      (resp) => {
        // Shallow copy old map.
        let counts1 = new Map(this.state.add_cart_forms);
        
        _.each(resp.data, (product) => {
          if (!counts1.has(product.id)) {
            counts1.set(product.id, 1);
          }
        });

        let state1 = _.assign({}, this.state, {
          products: resp.data,
          add_cart_forms: counts1,
        });
        this.setState(state1);
      }
    );
  }
  
  update_add_cart_count(product_id, count) {
    let counts1 = new Map(this.state.add_cart_forms);
    counts1.set(product_id, count);
    let state1 = _.assign({}, this.state, { add_cart_forms: counts1 });
    this.setState(state1);
  }
```

Move product list over to product_list.jsx

 * Export default
 * Import from root
 * Pass through add_cart\_forms as counts to ProductList

Thread through counts:

```
export default function ProductList(props) {
  let {root, products, counts} = props;
  let prods = _.map(products, (pp) =>
    <Product key={pp.id} product={pp} root={root} count={counts.get(pp.id)||1} />);
...
  
function Product(props) {
  let {root, product, count} = props;
  let changed = (ev) => {
    root.update_add_cart_count(product.id, ev.target.value);
  };
  
  ...
  <input className="form-control" style={\{width: "8ex"}}
         type="number" value={count} onChange={changed} />
```


### More Features

 * Show single item.
   - Pull up react-router-dom docs for Route
   - ```<Route path="/products/:id" ... ```


## Next Time: Redux

