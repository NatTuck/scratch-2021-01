+++
title = "./11-add-users/notes.md"
+++

## First: Homework Questions

## Continuing the HuskyShop App

 * Code available at: https://github.com/NatTuck/husky_shop
 * Today starts at: 1-bootstrap
 * Prep for deploy: 2-deploy
 * Add users: 3-users
 * Shopping cart: 4-cart

## A note on deployment

 - We went over deployment before.
 - One new complication: You need to create the production database on the server.

## Part 1: Add Users

Before we can do much with our online store, we need user accounts.

```
mix phx.gen Users User users email:string admin:boolean
```

 * Add resource to router.
 * Run migration.
 * Show the new scaffold.
 * Add "table table-striped" to the index.
 * Add "form-group", "form-control", and "btn btn-primary" to the form.

We want users to be able to log into our app. One way to think about this is as
a resource: sessions.

The sessions resource is kind of weird. It doesn't have a DB table or even a
context, because the data is stored in a cookie. But it does have two basic
operations that can be handled as REST methods: create and delete.

Add our new resource to the router:

```
scope "/", HuskyShopWeb do
  ...
  resources "/sessions", SessionController, only: [:create, :delete], singleton: true
```

Create our controller module:

lib/husky\_shop\_web/controllers/session_controller.ex

```
defmodule HuskyShopWeb.SessionController do
  use HuskyShopWeb, :controller

  def create(conn, %{"email" => email}) do
    user = HuskyShop.Users.get_user_by_email(email)
    if user do
      conn
      |> put_session(:user_id, user.id)
      |> put_flash(:info, "Welcome back #{user.email}")
      |> redirect(to: Routes.page_path(conn, :index))
    else
      conn
      |> put_flash(:error, "Login failed.")
      |> redirect(to: Routes.page_path(conn, :index))
    end
  end

  def delete(conn, _params) do
    conn
    |> delete_session(:user_id)
    |> put_flash(:info, "Logged out.")
    |> redirect(to: Routes.page_path(conn, :index))
  end
end
```

Add a form to the layout:

.../templates/layout/app.html.eex

```
<!-- in the navbar, make the previous column col-4 too -->
    <div class="col-4">
      <%= if @current_user do %>
        <p class="my-3">
          User: <%= @current_user.email %> |
          <%= link("Logout", to: Routes.session_path(@conn, :delete),
            method: :delete) %>
        </p>
      <% else %>
        <%= form_for @conn, Routes.session_path(@conn, :create),
                [class: "form-inline"], fn f -> %>
          <%= text_input f, :email, class: "form-control" %>
          <%= submit "Login", class: "btn btn-secondary" %>
        <% end %>
      <% end %>
    </div>
```

Create a fetch_session plug:

lib/husky\_shop\_web/plugs/fetch_session.ex

```
defmodule HuskyShopWeb.Plugs.FetchSession do
  import Plug.Conn

  def init(args), do: args

  def call(conn, _args) do
    user = HuskyShop.Users.get_user(get_session(conn, :user_id) || -1)
    if user do
      assign(conn, :current_user, user)
    else
      assign(conn, :current_user, nil)
    end
  end
end
```

Add the plug to our router:

```
pipeline :browser do
  ...
  plug HuskyShopWeb.Plugs.FetchSession
end
```

Add two more accessors to our context:

lib/husky\_shop/users/users.ex

```
...
  # below get_user!
  def get_user(id), do: Repo.get(User, id)
  
  def get_user_by_email(email) do
    Repo.get_by(User, email: email)
  end
```

Add default accounts for testing:

priv/repo/seeds.ex

```
alias HuskyShop.Repo
alias HuskyShop.Users.User

Repo.insert!(%User{email: "alice@example.com", admin: true})
Repo.insert!(%User{email: "bob@example.com", admin: false})
```

Recreate a clean DB with:

```
mix ecto.reset
```

Create a registration page:

 * On page/index, add ```<%= link "Register", to: Routes.user_path(@conn, new) %>```
 * On user/form, remove the "admin" checkbox.
 * On user/form, change the button to say "Register".
 * In user_controller, make create set session[:user-id] and redirect to products/index

```
# user_controller
    ...
    |> put_session(:user_id, user.id)
    |> redirect(to: Routes.product_path(conn, :index))
```

(current code in branch: 3-users)

## Part 2: Add Cart

A couple of design questions:

 * Where to put cart? Session or database.
   * Session scales better...
   * But DB means a user can log in from a different browser and still have their cart.
 * One cart per user or many?
   * Carts, orders, and wish lists look pretty similar.
   * But one cart per user is lazier.

So each user has one cart, which has many cart items.

```
$ mix phx.gen.html Carts CartItem cart_items user_id:references:users product_id:references:products count:integer
```

Before we run the migration, let's edit it:

priv/repo/migrations/*\_create\_cart\_items.exs

```
  ...
  create table(:cart_items) do
    add :count, :integer, null: false
    add :user_id, references(:users, on_delete: :delete_all), null: false
    add :product_id, references(:products, on_delete: :delete_all), null: false

    timestamps()
  end
  ...
```

 * Add resource line to router
 * Run migration

Add an "add to cart" form to product page.

Copy form from cart item and modify into:

templates/product/show:

```
<%= if @current_user do %>
  <div>
    <%= form_for @item_cset, Routes.cart_item_path(@conn, :create),
               [class: "form-inline"], fn f -> %>

      <%= hidden_input f, :user_id %>
      <%= hidden_input f, :product_id %>

      <div class="form-group">
        <%= number_input f, :count, class: "form-control" %>
        <%= error_tag f, :count %>
      </div>

      <div>
        <%= submit "Add to Cart", class: "btn btn-primary" %>
      </div>
    <% end %>
  </div>
<% end %>
```

Make a changeset for the cart item in product_controller

```
alias HuskyShop.Carts
  ...
  def show(conn, %{"id" => id}) do
    product = Products.get_product!(id)
    user_id = get_session(conn, :user_id)
    item_cset = Carts.change_cart_item(%Carts.CartItem{
          user_id: user_id, product_id: product.id, count: 1})
    render(conn, "show.html", product: product, item_cset: item_cset)
  end
```

Add a cart column to the main layout.

```
      <div class="row">
        <div class="col-8">
          <%= render @view_module, @view_template, assigns %>
        </div>

        <div class="col-4">
          <%= if @current_user do %>
            <%= render HuskyShopWeb.CartItemView, "index.html", 
                  cart_items: @current_user.cart_items %>
          <% else %>
            <p><%= link "Register", to: Routes.user_path(@conn, :new) %></p>
          <% end %>
        </div>
      </div>
    </div>
```

Users don't have a cart items field. We need to tell Ecto about the relationship
between users and cart items to fix this.

lib/husky_shop/users/user.ex

```
schema "users" do
  ...
  has_many :cart_items, HuskyShop.Carts.CartItem
```

Add to cart button doesn't work, and we want each cart item to belong to
a specific user and product.

lib/husky\_shop/carts/cart_items.ex

```
  schema "cart_items" do
    field :count, :integer
    belongs_to :user, HuskyShop.Users.User
    belongs_to :product, HuskyShop.Products.Product

    timestamps()
  end

  @doc false
  def changeset(cart_item, attrs) do
    cart_item
    |> cast(attrs, [:count, :user_id, :product_id])
    |> validate_required([:count, :user_id, :product_id])
  end
```

We should probably show item names in the cart.

template/cart\_item/index:

 * add class="table table-striped"
 * add [th]Name[/th]
 * add [td]<%= cart_item.product.name %>[/td]

Update the preload to be two deep.

lib/husky\_shop/users/users.ex

```
def get_user(id) do
  ...
  preload: [cart_items: :product]
```

Lets add the last relationship. For any product, we can see what
carts it's in.

products/product.ex:

```
schema ...
  ...
  has_many :cart_items, HuskyShop.Carts.CartItem
```

Let's take a look at this preload thing.

(make sure user 2 has some cart items first)

```
$ iex -S mix
iex> alias HuskyShop.Repo
iex> import Ecto.Query
iex> alias HuskyShop.Users.User
iex> alias HuskyShop.Products.Product
iex> (from u in User, limit: 1) |> Repo.one
iex> (from u in User, where: u.id == 2) |> preload([cart_items: :product]) |> Repo.one 
iex> (from p in Product) |> preload(:cart_items) |> Repo.all
```

Core Ecto relationships:

 * belongs\_to: This table has an id field linking it to another table.
 * has\_many: The other table has an id field linking it to this one.

See the Ecto Schema documentation for more details and examples.

 * Google Ecto Schema here.
 * Show types table.
 * Show doc for belongs_to

(current state is branch: 4-cart)

