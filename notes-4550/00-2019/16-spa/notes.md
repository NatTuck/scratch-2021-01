+++
title = "./16-spa/notes.md"
+++

# First Thing

 - HW07 questions.
 - HW08 is posted; that's the last one before the final project.

## HW08

 - Rewrite your Task Tracker as an SPA
 - We'll be covering SPAs this week

# Single Page Apps

We're going to rewrite Husky Shop to be a SPA.

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

## HuskyShop SPA

github repo: https://github.com/NatTuck/husky_shop_spa

 - 0-init branch is initial generated code
 - 1-deps branch adds React and Bootstrap

Start by cloning and checking out 1-deps

## Add Resources

(ends with 2-resources)

Create an :api scope in our router:

```
  scope "/api/v1", HuskyShopWeb do
    pipe_through :api

    ...
  end
```

Here's our resources:

```
$ mix phx.gen.json Users User users email:string password_hash:string admin:boolean
$ mix phx.gen.json Products Product products name:string desc:text price:decimal inventory:integer
$ mix phx.gen.json CartItems CartItem cart_items user_id:references:users product_id:references:products
```

Add the new routes to a new :api scope.

Next, clean up the migrations:

 - Most things are ```null: false```
 - References are ```on_delete: :delete_all```
 - Add unique indexes

```
    # users.ex
    create index(:users, [:email], unique: true)
    # products.ex
    create index(:products, [:name], unique: true)
    # cart_items.ex
    create index(:cart_items, [:user_id, :product_id], unique: true)
```

Update our schemas:

```
    # User
    has_many :cart_items, HuskyShop.CartItems.CartItem
    
    |> unique_constraint(:email)
   
    # Product
    has_many :cart_items, HuskyShop.CartItems.CartItem
    
    |> unique_constraint(:name)
   
    # CartItem
    belongs_to :user, HuskyShop.Users.User
    belongs_to :product, HuskyShop.Products.Product
    
    |> cast(attrs, [:user_id, :product_id])
    |> unique_constraint(:user_id, name: :cart_items_user_id_product_id_index)
    |> validate_required([:user_id, :product_id])
```

Let's add some database seeds:

```
alias HuskyShop.Repo
alias HuskyShop.Users.User

pwhash = Argon2.hash_pwd_salt("pass1")

Repo.insert!(%User{email: "alice@example.com", admin: true, password_hash: pwhash})
Repo.insert!(%User{email: "bob@example.com", admin: false, password_hash: pwhash})

alias HuskyShop.Products.Product
Repo.insert!(%Product{name: "Rubber Duck", desc: "Yellow",
                      price: Decimal.new("4.99"), inventory: 5})
Repo.insert!(%Product{name: "Bear", desc: "500lbs; angry",
                      price: Decimal.new("44.99"), inventory: 2})
Repo.insert!(%Product{name: "Cookie", desc: "chocolate oatmeal",
                          price: Decimal.new("0.99"), inventory: 2})
Repo.insert!(%Product{name: "Donut", desc: "chocolate frosted",
                          price: Decimal.new("1.19"), inventory: 2})
```

Run ```mix ecto.reset```


## List Products

(ends with 3-list-products)

 - Our scaffolding is just a JSON API, so we need to build our app UI from
   scratch.
 - We're using React for our UI, so we need to figure out our component's
   state.

We'll start by listing our products. Which products we list potentially changes,
so we'll add our list of products to the state.

Problem: Where do we get our list of products?

### Plan A: AJAX fetch products on page load

Show ```root-v1.js```

Problem: We get three page paints:

 - "Loading react component"
 - React component with empty state
 - React component after AJAX call

This is ugly. That last paint can't happen until the page loads and an extra
HTTP roundtrip completes. We really don't want to wait on that extra HTTP
request.

### Plan B: Send the product list with the initial page

We know what data we need. Currently, it's not even going to change - we just
need to get a list of products to render on initial page load. We generate the
initial page dynamically with elixir code - we can just include the product list
there.

In page/index.html.eex:

```
<script>
 window.products = <%= raw Jason.encode!(@products) %>;
</script>
```

In page_controller

```
  def index(conn, _params) do
    products = HuskyShop.Products.list_products()
    |> Enum.map(&(Map.take(&1, [:id, :name, :desc, :price])))
    render conn, "index.html", products: products
  end
```

In root.jsx

```
  // in root_init
  let prods = window.products;
  
  // in constructor, comment out fetch_products call
```

## Simulating Pages

(ends with 4-pages)

 * We'd like to make it so that clicking on a product shows us the
   page for that product.
 * Unfortunately, we're a single-page application, so we'll need to simulate
   "pages".
 * We could do this by simply making "page" be a part of our client-side state,
   but then we'd need to manage a bunch of stuff by hand: what to display on
   each page, what happens if the user presses "back", etc.
 * Luckily, there's a library that handles this for us: React Router.

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
          <Header />
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
    <div className="col-2">
      <p><Link to={"/users"}>Users</Link></p>
    </div>
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

Problem: When do we fetch our users to add to our state?

Options:

 - Include them in the inital page.
 - Do an AJAX request on page load.
 - Do an AJAX request when we visit the users page.

When do we refetch?

 - Never?
 - Page request?

Let's try doing loads on click.

Pass the root component through to the header, and then call fetch_users on link
click:

```
... 
    <Header root={this} />
...
function Header(props) {
  let {root} = props;
  ...
    <Link to={"/users"} onClick={root.fetch_users.bind(root)}>...
```

The fetch_users method is just a copy of fetch\_products with s/products/users/.

## User Sessions

(ends with 5-sessions)

Problem: How do we handle logins? Our API scope doesn't fetch sessions, so those
don't help.

Solution: Issue the user a Phoenix token.


Add get and auth user code to users/users.ex:

```
  def get_user_by_email(email) do
    Repo.get_by(User, email: email)
  end

  def get_and_auth_user(email, password) do
    user = get_user_by_email(email)
    case Comeonin.Argon2.check_pass(user, password) do
      {:ok, user} -> user
      _else       -> nil
    end
  end
```

Session controller:

```
defmodule HuskyShopWeb.SessionController do
  use HuskyShopWeb, :controller

  def create(conn, %{"email" => email, "password" => password}) do
    with %User{} = user <- get_and_auth_user(email, pass) do
      resp = %{
        data: %{
          token: Phoenix.Token.sign(HuskyShopWeb.Endpoint, "user_id", user.id),
          user_id: user.id,
        }
      }
      conn
      |> put_resp_header("content-type", "application/json; charset=utf-8")
      |> send_resp(:created, Jason.encode!(resp))
    end
  end
end
```

 * AJAX-submit user and password on button click
 * Add token to React state.
 * The plan: Send token with future authenticated reqs like add-to-cart.

## Add To Cart

