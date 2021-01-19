+++
title = "./13-ajax/notes.md"
+++

## First: Homework Questions

## Continuing the HuskyShop App

 * Code available at: https://github.com/NatTuck/husky_shop
 * Today starts at: 5-cleanup
 * Ratings: 6-ratings
 
## Adding Ratings to Husky Shop

### Concept

 - Any user can add a rating (1-5 stars) to any product.
 - The product displays the average rating.
 - New ratings by the same user overwrite the old.

Data model:

 - New resource: ratings
 - Fields:
   - user_id
   - product_id
   - stars: int
 - Associations:
   - belongs_to {product, user}
   - {product, user} has_many

No need to have entering a rating cause a full page load.

### Resource

Create the resource:

```
$ mix help phx.gen.json
$ mix phx.gen.json Ratings Rating ratings user_id:references:users product_id:references:products stars:integer
```

Edit the migration (no nulls, delete_all)

Add the line to a new section in the router. Note API vs. Browser vs. AJAX.

```
  ...
  pipeline :ajax do
    plug :accepts, ["json"]
    plug :fetch_session
    plug :fetch_flash
    plug HuskyShopWeb.Plugs.FetchSession # FIXME: "FetchUser"
  end
  ...
  scope "/ajax", HuskyShopWeb do
    pipe_through :ajax
    resources "/ratings", RatingController, except: [:new, :edit]
  end
```

Show new routes with /ajax prefix:

```
$ mix phx.routes
```

Run migration: ```mix ecto.migrate```

Add the assocaitions and fix validation:

ratings/rating.ex

```
  schema "ratings" do
    field :stars, :integer
    belongs_to :user, HuskyShop.Users.User
    belongs_to :product, HuskyShop.Products.Product
  ...
    |> cast(attrs, [:stars, :user_id, :product_id])
    |> validate_required([:stars, :user_id, :product_id])
```

BOTH users/user.ex and products/product.ex

```
    has_many :ratings, HuskyShop.Ratings.Rating
```

### Display: v1

First, let's preload ratings:

lib/husky_shop/products/products.ex

```
  def get_product!(id) do
    Repo.one! from p in Product,
      where: p.id = ^id,
      preload: [:ratings]
  end
```

Update product display:

lib/husky\_shop\_web/templates/product/show.html.eex

```
  <!-- in the product field list -->
  <li>
    <strong>Rating:</strong>
    <%= avg_rating(@product) %>
  
    <!-- C-x 8 RET, "black star" -->
    <%= if @current_user do %>
      <div id="rating-form">
        <select id="rating-select">
          <option value="1">★</option>
          <option value="2">★★</option>
          <option value="3">★★★</option>
          <option value="4">★★★★</option>
          <option value="5">★★★★★</option>
        </select>

        <button id="rating-button"
                data-user-id="<%= @current_user.id %>"
                data-product-id="<%= @product.id %>"
                class="btn btn-secondary">Rate!</button>
      </div>
    <% end %>
  </li>
```

Add our display helper function to the view

lib/husky\_shop\_web/views/product\_view.ex

```
  def avg_rating(product) do
    rats = Enum.map(product.ratings, &(&1.stars))
    if Enum.empty?(rats) do
      "?? (no ratings)"
    else
      mean = Enum.sum(rats) / Enum.count(rats)
      "#{mean} / 5 (#{Enum.count(rats)} ratings)"
    end
  end
```

### Submitting Ratings

The ratings form we made doesn't have a "form" tag, which means it won't
POST when we press the button. If we want it do do something, we'll have
to write the logic in JavaScript.

We'll use jQuery, which we already installed for Bootstrap.

assets/js/app.js

```
$(function () {
  $('#rating-button').click((ev) => {
    let rating = $('#rating-select').val();
    let user_id = $(ev.target).data('user-id');
    let product_id = $(ev.target).data('product-id');

    let text = JSON.stringify({
      rating: {
        user_id: user_id,
        product_id: product_id,
        stars: rating,
      },
    });

    $.ajax(rating_path, {
      method: "post",
      dataType: "json",
      contentType: "application/json; charset=UTF-8",
      data: text,
      success: (resp) => {
        $('#rating-form').text(`(your rating: ${resp.data.stars})`);
      },
    });
  });
});
```

Problem: A user can create multiple ratings.

First, let's ban that by adding a unique index to the migration.

priv/repo/migrations/201810\*ratings\*

```
  create index(:ratings, [:user_id, :product_id], unique: true)
```

husky_shop/ratings/rating.ex

```
  |> unique_constraint(:user_id, name: :ratings_user_id_product_id_index)
```

Now replace insert with upsert in ratings/ratings.ex

```
  def create_rating(attrs \\ %{}) do
    rat = %Rating{}
    |> Rating.changeset(attrs)
    |> Repo.insert(
      on_conflict: :replace_all,
      conflict_target: [:user_id, :product_id])
  end
```

Next problem: We're not updating the average.

rating_controller.ex

```
  # This goes first.
  def index(conn, %{"product_id" => p_id}) do
    ratings = Ratings.list_ratings(p_id)
    render(conn, "index.json", ratings: ratings)
  end
```

Create list_ratings/1 in husky\_shop/ratings/ratings.ex

```
  def list_ratings(product_id) do
    Repo.all from r in Rating,
      where: r.product_id == ^product_id
  end
```

Add an ID to the average in product/show:

```
    <span id="rating-avg">
      <%= avg_rating(@product) %>
    </span>
```

Let's add lodash.

```
$ npm install --save lodash
```

app.js

```
// Below bootstrap import
import _ from "lodash";
```

app.js, add a function

```
$(function () {
  function update_stars(product_id) {
    $.ajax(`${rating_path}?product_id=${product_id}`, {
      method: "get",
      dataType: "json",
      contentType: "application/json; charset=UTF-8",
      data: "",
      success: (resp) => {
        let count = resp.data.length;
        let sum = _.sum(_.map(resp.data, (rat) => rat.stars));
        $('#rating-avg').text(`${sum/count} / 5 (${count} ratings)`);
      },
    });
  }

  ...
      success: (resp) => {
        $('#rating-form').text(`(your rating: ${resp.data.stars})`);
        update_stars(product_id);
      },
```

(current state as branch: 6-ratings) 


