
## First: Homework Questions

## Part 1: Multiplayer Hangman

First, some basic cleanup:

 - rm game.js
 - Get rid of SetTitle

### Multiplayer:

First, start supervisor in application.ex:

```
    children = [
      ...
      Hangman.GameSup,
```

Need to specify username:

```
export function ch_login(name) {
  channel.push("login", {name: name})
         .receive("ok", state_update)
         .receive("error", resp => {
           console.log("Unable to push", resp)
         });
}
```

```
// Hangman.js
function Play({state}) {
  let {name, word, guesses} = state;

  let view = word.split('');
  let bads = [];

  function guess(text) {
    // Inner function isn't a render function
    ch_push({letter: text});
  }

  function reset() {
    console.log("Time to reset");
    ch_reset();
  }

  return (
    <div>
      <div className="row">
        <div className="column">
          <p>Word: {view.join(' ')}</p>
        </div>
        <div className="column">
          <p>Name: {name}</p>
        </div>
      </div>
      <div className="row">
        <div className="column">
          <p>Guesses: {guesses.join(' ')}</p>
        </div>
      </div>
      <Controls reset={reset} guess={guess} />
    </div>
  );
}

function Login() {
  const [name, setName] = useState("");

  return (
    <div className="row">
      <div className="column">
        <input type="text"
               value={name}
               onChange={(ev) => setName(ev.target.value)} />
      </div>
      <div className="column">
        <button onClick={() => ch_login(name)}>Join</button>
      </div>
    </div>
  );
}

function Hangman() {
  // render function,
  // should be pure except setState
  const [state, setState] = useState({
    name: "",
    word: "",
    guesses: [],
  });

  console.log("render state", state);

  useEffect(() => {
    ch_join(setState);
  });

  let body = null;

  if (state.name === "") {
    body = <Login />
  }
  else if (state.guesses.length < 8) {
    body = <Play state={state} />;
  }
  else {
    body = <GameOver reset={reset} />;
  }

  return (
    <div className="container">
      {body}
    </div>
  );
}
```

```
  # game.ex
  def view(st, user) do
    ...
    %{
      word: word,
      guesses: MapSet.to_list(st.guesses),
      name: user
    }
  end
```

```
  # game_channel.ex
  alias Hangman.Game
  alias Hangman.GameServer

  @impl true
  def join("game:" <> name, payload, socket) do
    if authorized?(payload) do
      GameServer.start(name)
      view = GameServer.peek(name)
      |> Game.view("")
      socket = assign(socket, :name, name)
      {:ok, view, socket}
    else
      {:error, %{reason: "unauthorized"}}
    end
  end

  @impl true
  def handle_in("login", %{"name" => user}, socket) do
    socket = assign(socket, :user, user)
    name = socket.assigns[:name]
    view = GameServer.peek(name)
    |> Game.view(user)
    {:reply, {:ok, view}, socket}
  end

  @impl true
  def handle_in("guess", %{"letter" => ll}, socket) do
    user = socket.assigns[:user]
    name = socket.assigns[:name]
    view = GameServer.guess(name, ll)
    |> Game.view(user)
    broadcast(socket, "view", view)
    {:reply, {:ok, view}, socket}
  end

  @impl true
  def handle_in("reset", _, socket) do
    user = socket.assigns[:user]
    name = socket.assigns[:name]
    view = GameServer.reset(name)
    |> Game.view(user)
    broadcast(socket, "view", view)
    {:reply, {:ok, view}, socket}
  end
```

Ok, let's test that with two windows.

Great, except no change on other player's actions.

```
    # game_channel.ex
    # handle_in("guess"
    # handle_in("reset"
    broadcast(socket, "view", view)
``` 

```
  def reset(name) do
    GenServer.call(reg(name), {:reset, name})
  end

  def handle_call({:reset, name}, _from, game) do
    game = Game.new
    BackupAgent.put(name, game)
    {:reply, game, game}
  end
```

Now we need to handle that message.

```
// bottom of socket.js
channel.on("view", state_update);
```



## Part 2: Introducing CRUD

Hangman and Bulls & Cows have:

  - Multiple interacting concurrent users
  - Dynamic view updates
  - Real time interaction
  - Ephemeral state - if the server reboots, we lose game state

I think applications with those properties are especially interesting, but
many apps don't have them.

Consider an online store:

 - Two kinds of users: Customer & Staff
 - No need for interacting concurrent users or real-time anything
 - Nothing needs to update dynamically. Things occasionally change, but
   there's no need to push updated product descriptions to users while they're
   reading them.
 - State needs to be persistent. If the server reboots, we don't want to lose
   our record of products or orders.

If we generalize enough, there's a whole class of apps that look basically like this:

 - We have some important data.
 - The app consists of looking at and modifying that data.

We have a really good way of storing data: A relational database

 - Our data comes as a set of well defined types.
   - e.g. a product has a name, description, and price
 - There are defined relationships between our data.
   - e.g. a "cart item" describes a product added to a shopping cart by a user.
 - We want to be able to validate our data and the relationships.
   - Otherwise we may end up with corrupted data, which is a serious problem.
 - We want to be able to query our data based on the relationships.
   - Get the total price for products in the shopping cart for user Alice.

A relational database stores data as a collection of tables.

 - Each table stores records of one type with an associated schema.
   - So each record in a table is the same type of data, e.g. a user.
 - Each table has a mandatory field, a unique numeric ID called "id".
 - A table can have fields which reference records in other tables,
   this is achieved by storing the id of a field in another table.
 
We can treat an online store as a database plus a user interface that
allows users to do CRUD operations: Create, Retrieve, Update, Delete.

What are our tables?

 - products: name, description, price, inventory
 - users: email
 - cart\_items: user\_id, product\_id, count

Things that we can apply CRUD operations to are commonly called "resources". The
Phoenix framework provides some tools to help us build resource-centric apps using
an app structure called REST.

## Starting the HuskyShop App

 * Code available at: https://github.com/NatTuck/husky\_shop
 * Generated code + DB config: 0-generated
 * Add bootstrap: 1-bootstrap
 * (next class) Prep for deploy: 2-deploy


Create the app:

```
### confirm Phoenix version is 1.4.0
$ mix help phx.new 

### create our app
$ mix phx.new husky_shop
```

Set up the database:

```
$ pwgen 12 1
<password>
$ sudo su - postgres
pg$ createuser -d husky_shop
pg$ psql
postgres=# alter user husky_shop with password '<password>';
pg$ ^D
```

config/dev.exs and config/test.exs

```
...
config :husky_shop, HuskyShop.Repo,
  username: "husky_shop",
  password: "<password>",
  database: "husky_shop_dev",
  hostname: "localhost",
  pool_size: 10
```

Create the DB and run stock tests.

```
$ mix ecto.create
$ mix test
```

Create our first resource: Products

```
$ mix help phx.gen.html
$ mix phx.gen.html Products Product products name:string desc:text price:decimal inventory:integer
$ mix ecto.migrate
```

Add the resources line to the router as instructed.

(branch at this point: 0-generated)

Show the scaffold in action:

 * Create
 * View
 * Edit
 * View
 * Create
 * List
 * Delete

Look through the generated resource files:

 * lib/husky\_shop/products/product.ex - schema
   * Defines the data type.
   * Validates new items before inserting into DB.
 * lib/husky\_shop/products/products.ex - context
   * Exposes the DB CRUD operations as functions.
   * This is an API that our web interface uses.
 * lib/husky\_shop\_web/controllers/product\_controller.ex
   * Handles incoming requests.
   * Delegates to context to actually manipulate database.
   * Delegates to view / template to display HTTP response.
 * lib/husky\_shop\_web/views/product\_view.ex
   * Just default code, which delegates to template.
 * lib/husky\_shop\_web/templates/product/show.html.eex
   * Each controller action has a template.

Run "mix phx.routes".

## RESTful Routes

The structure that Phoenix gives us for mapping HTTP routes to CRUD actions is
called REST. In REST, each resource has a path on the server, and we access
operations on the resource by sending HTTP requests to that path.

REST Example with a relational DB.

 - Our resource is "products".
 - Instances are stored in a "products" table in our DB.
 - The associated HTTP path is "/products".
 - We support 5 operations.
   - List all products: "GET /products"
   - View one product (id = 10): "GET /products/10"
   - Create a product: "POST /products"
   - Update a product (id = 10): "PATCH /products/10"
   - Delete a product (id = 10): "DELETE /products/10"
 - There are two more paths for a traditional web UI.
   - Show the form for a new product: "GET /products/new"
   - Show the form to edit a product (id = 10): "GET /products/edit/10"

This isn't the only way to structure a web app, but it's pretty common today.

This is useful for:

 * Traditional web sites where users directly access these paths with
   their browser.
   * Guarantees that bookmarks work.
   * Allows caching to work as expected.
 * "Single Page Applications", where some JavaScript code - maybe a React UI -
   actually accesses the paths.
   * Interface exposed by server to in-browser code is well defined.
 * Non-browser HTTP clients.
   * REST provides a structure for an HTTP API

## Adding Bootstrap 4

Milligram is fine, but let's use something fancier for Husky Shop.

First, we need to remove the defaults.

```
$ cd assets
$ rm static/images/phoenix.png css/phoenix.css
```

Install Bootstrap and deps.

```
$ npm install --save bootstrap jquery popper.js
$ npm install --save-dev node-sass sass-loader
```

assets/webpack.config.js

```
// module: rules: 
  {
    test: /\.scss$/,
    use: [MiniCssExtractPlugin.loader, 'css-loader', 'sass-loader']
  },
```

 * move css/app.css to css/app.scss

in css/app.scss:

```
$body-bg: #BBB; // With scss, Bootstrap lets us override colors and stuff.
@import "~bootstrap/scss/bootstrap";
```

in js/app.js:

```
import jQuery from 'jquery';
window.jQuery = window.$ = jQuery; // Bootstrap requires a global "$" object.
import "bootstrap";
```

in .../templates/layout/app.html.eex:

```
...
  <body>
    <div class="container">
      <nav class="navbar navbar-expand-sm navbar-light bg-white">
        <div class="col-4">
          <a class="navbar-brand" href="#">Husky Shop</a>
        </div>
        <div class="col-8">
          <ul class="navbar-nav mr-auto">
            <li class="nav-item active">
              <a class="nav-link" href="#">Home</a>
            </li>
            <li class="nav-item">
              <a class="nav-link" href="#">Products</a>
            </li>
          </ul>
        </div>
      </nav>

      <div class="row">
        <div class="col-12">
          <%= if get_flash(@conn, :info) do %>
            <p class="alert alert-info" role="alert"><%= get_flash(@conn, :info) %></p>
          <% end %>
          <%= if get_flash(@conn, :error) do %>
            <p class="alert alert-danger" role="alert">
              <%= get_flash(@conn, :error) %>
            </p>
          <% end %>
        </div>
      </div>

      <%= render @view_module, @view_template, assigns %>
    </div>

    <script ...
``` 

in .../templates/page/index.html.eex:

```
<div class="row">
  <div class="col-12">
    <h1>Welcome to Husky Shop</h1>
  </div>
</div>
```

in .../templates/product/index.html.eex

```
...
<table class="table table-striped">
...
        <%= link "Show", to: Routes.product_path(@conn, :show, product),
            class: "btn btn-secondary" %>
        <%= link "Edit", to: Routes.product_path(@conn, :edit, product),
            class: "btn btn-info" %>
        <%= link "Delete", to: Routes.product_path(@conn, :delete, product),
            method: :delete, data: [confirm: "Are you sure?"],
            class: "btn btn-danger" %>
...
<%= link "New Product", to: Routes.product_path(@conn, :new), class: 'btn btn-primary' %>
...
```

in .../templates/product/form.html.eex

 * Put each input in a ```<div class="form-group">```
 * Add class="form-control" to each input control.
 * Add class="btn btn-primary" to the submit button.

We should bootstrapify the rest of the templates, but you can look at doing that
on your own.

(branch at this point: 1-bootstrap)
