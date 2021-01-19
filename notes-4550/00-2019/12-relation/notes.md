+++
title = "./12-relation/notes.md"
+++

## First: Homework Questions

## Continuing the HuskyShop App

 * Code available at: https://github.com/NatTuck/husky_shop
 * Today starts at: 3-users
 * Shopping cart: 4-cart
 * Cleanup: 5-cleanup

## Add Cart

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

## Cleanup

Prevent non-admin users from managing products:

Create a new plug in plugs/require_admin.ex

```
defmodule HuskyShopWeb.Plugs.RequireAdmin do
  use HuskyShopWeb, :controller

  def init(args), do: args

  def call(conn, _params) do
    user = conn.assigns[:current_user]
    if user.admin do
      conn
    else
      conn
      |> put_flash(:error, "You can't do that.")
      |> redirect(to: Routes.page_path(conn, :index))
      |> halt
    end
  end
end
```

In products_controller, add:

```
plug HuskyShopWeb.Plugs.RequireAdmin when action in [:new, :create, :update, :delete]
```

Only show product new / edit / delete buttons if user is admin.

  * Edit product/index
  * Edit product/show


Make the shopping cart link to the item.

In cart\_item/index:

```
<!-- item name -->
  <td><%= link(cart_item.product.name,
    to: Routes.product_path(@conn, :show, cart_item.product_id)) %></td>
    ...
<!-- remove show and edit links -->
```

Update redirects for cart_items to always redirect to the *product*.

In cart\_item\_controller:

```
  |> redirect(to: Routes.product_path(conn, :show, cart_item.product_id))
```

 * Create
 * Update
 * Delete


More cleanups:

 * Get rid of the new cart item and main column register buttons.
 * Shopping Cart should be called "Shopping Cart".

Add filler image:

 * Go to openclipart
 * Grab a random image
 * Put it in assets/static/images
 * Add an attribution section to the main README.md and link
   to the image source.

Replace product list with cards:

In lib/husky_shop_web/templates/product/index.html.eex

Replace the table markup with:

```
<div class="row">
  <%= for product <- @products do %>
    <div class="card col-6">
      <%= link(
        img_tag(Routes.static_path(@conn, "/images/rubber-duck.png"), 
          class: "card-img-top"),
        to: Routes.product_path(@conn, :show, product)) %>

      <div class="card-body">
        <div class="card-title">
          <%= link(product.name, to: Routes.product_path(@conn, :show, product)) %>
        </div>

        <div class="card-text">
          <p><%= product.desc %></p>

          <% if @current_user.admin do %>
            <p>
              <%= link("Edit", to: Routes.product_path(@conn, :edit, product),
                class: "btn btn-info") %>
              <%= link("Delete", to: Routes.product_path(@conn, :delete, product),
                method: :delete, data: [confirm: "Are you sure?"],
                class: "btn btn-danger") %>
            </p>
          <% end %>
        </div>
      </div>
    </div>
  <% end %>
</div>
```

(current state: 5-cleanup)

## Closing Notes

The key thing for the next few assignments is learning and using what these
libraries give you. To do that:

 * Find and review the official documentation before you need it.
 * This lets you have some idea of what's available. When you have a
   problem, you have a chance of already knowing where to find the solution.
 * For tools like Bootstrap, the only way to know that it gives you cards is
   to have looked at the list of components.
 * Phoenix, Ecto, Bootstrap, jQuery, lodash, Elixir, etc have one-liner
   solutions to accomplish many common tasks for "boring" web apps. Productivity
   with these tools means finding and using those utilities.

