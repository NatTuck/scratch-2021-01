---
layout: default
---

## New assignment

 - Show assignment
 - Show timesheets app specs
 - "Read the spec, complete the described prototype"
 - We'll do logins with passwords. Hopefully we can get through the rest of tags
   and cover passwords today.

## Where are we?

 - We're building a photo sharing app: Lens

Last time, we:

 - Added tags and photo_tags resources.
 - Set up associations for these resources.
 - Displayed tags on photos.
 - Setup adding tags to photos using an AJAX request.

To finish up from last time we need:

 - Displaying the new tag when added.
 - Removing tags.

For the tags feature, our branches are:

 * repo: https://github.com/NatTuck/lens
 * Start branch: 5-cleanups
 * End branch: 6-tags
 
# Generate new tag HTML on server

Now we need to update the tag view to return the relevent data:

Create a new file .../templates/tag/tag.html.eex:

```
<span class="badge badge-secondary"
      data-tag-id="<%= @pt.tag.id %>"
      data-photo-tag-id="<%= @pt.id %>">
  <%= @pt.tag.name %>
  <a href="#" class="badge badge-secondary tag-delete">&times;</a>
</span>
```

.../views/photo\_tag\_view.ex

```
  def render("photo_tag.json", %{photo_tag: photo_tag}) do
    tag_name = if Ecto.assoc_loaded?(Map.get(photo_tag, :tag)) do
      photo_tag.tag.name
    else
      nil
    end
   
    html = render(LensWeb.TagView, "tag.html", pt: photo_tag)
    |> Phoenix.HTML.safe_to_string
    
    %{
      id: photo_tag.id,
      photo_id: photo_tag.photo_id,
      tag_id: photo_tag.tag_id,
      tag_name: tag_name,
      html: html,
    }
  end
```

Now we can add tags to photos and we get the correct data back in the response.
Next we need to add the tag to the display back in the browser.


Update the photo card in .../photos/card.html.eex:

```
  <!-- replace the badge span with a render -->
  <%= for pt <- @photo.photo_tags do %>
    <%= render LensWeb.TagView, "tag.html", Map.put(assigns, :pt, pt) %>
  <% end %>
```

Update our .../app.js:

```
function show_tag(photo_id, tag) {
  let cards = $('.photo-tags');
  let base = _.find(cards, (xx) => $(xx).data('photo-id') == photo_id);
  let list = $(base).find('.tag-list');

  for (let pp of list.find('.badge')) {
    if ($(pp).data('tag-id') == tag.tag_id) {
      return;
    }
  }

  list.append(tag.html);

  console.log("shown", photo_id, tag);
}
...
function add_tag(photo_id, tag) {
   ...
   success: (resp) => {
      console.log(resp);
      show_tag(photo_id, resp.data);
    },
```

Adding tags now works. Next is deleting tags.

In .../app.js:

```
function show_tag(photo_id, tag) {
  ...

  list.append(tag.html);

  list = $(base).find('.tag-list');

  for (let pp of list.find('.badge')) {
    if ($(pp).data('tag-id') == tag.tag_id) {
      $(pp).find('.tag-delete').on("click", remove_click);
    }
  }

...

function init_tags() {
  $('.photo-tags').each((_, base) => {
    ...
    $(base).find('.tag-delete').on("click", remove_click);
  });
}

...

function remove_click(ev) {
  ev.preventDefault();
 
  let tag = $(ev.target).parent();
  let photo_tag_id = tag.data('photo-tag-id');

  $.ajax("/ajax/photo_tags/"+photo_tag_id, {
    method: "delete",
    dataType: "json",
    contentType: "application/json; charset=UTF-8",
    data: "",
    headers: {
      'x-csrf-token': window.csrf_token,
    },
    success: (resp) => {
      console.log("tag removed", resp);
      tag.remove();
    },
    error: (resp) => {
      console.log(resp);
      console.log(resp.responseText);
    }
  });
}
```

## Missing functionality

 - List tags.
 - List photos by tag.

This functionality is left as an exercise for the student.

## Associations Review

wheel belong\_to car: The wheel table has a car_id field. Given a wheel, we can
preload the car.

car has\_many wheels: The wheel table has a car_id field. Given a car, we can
preload all the wheels.

car has\_one roof: The roof table has a car_id field. Given a car, we can get
the roof. The roof will belong\_to the car.

has\_many through: This allows indirect or many\_to\_many relationships, with a
lot of options.

# Passwords

## Problem: User Authentication

 - Lots of apps have user accounts.
 - We'd like our user accounts to correspond to actual people.
 - Specifically, if Alice makes an account, then some other user - Mallory -
   shouldn't be able to control that account.
 - When a user logs in - creating a session - we want to authenticate that
   they're the same person who created the account.
 - Sadly, that's impossible.
 - But we can do some stuff that's kind of similar.

Authentication ideas:

 - Link to an already authenticated account - e.g. Google, Facebook, GitHub.
   - Still not identifying a person.
   - But we can transfer most of the identity properties the alice123 Google
     account had to our Alice account.
 - Link to an email address or phone number
   - Same as above.
 - Use a cryptographic public key.
   - Still not a person.
   - But we can guarantee that only a person or machine with access to the 
     appropriate private key can authenticate as the linked user.
 - Use a password.
   - Like cryptographic keys, but way worse.

## Solution: Passwords

 - Users select a password when they register.
 - We assume anyone who knows the password is that person.
 - How long a password do we need to require?

### Threat 1: Online Attacks

A person (or program) guesses passwords and tries to log in to the website with
them.

 - Attacker might be able to try a thousand passwords per second.
 - Assuming a password has random lowercase letters, how long does
   it take to guess a 4 character password? (5 minutes)
 - A 6 character password? (2 days)
 - An 8 character password? (3 years)

Solving online attacks is simple: Limit users to 20 login attempts per hour.

### Threat 2: Offline Attacks

The user sends us a password when they register, and we need to be able to
check for that same password when they later log in.

Bad Plan A: Store the password in our database.

What happens when the attacker has a copy of our database?

 - They have all the passwords and can log in as anyone.
 - People tend to reuse passwords on different sites, so the attacker can
   probably log into everyone's steam accounts and steal all their trading
   cards.

Bad Plan B: Store a cryptographic hash (e.g. SHA256) in the DB.

 - Now the attacker needs to brute force the hashes before they
   have any passwords.
 - This is basically the same problem as mining Bitcoin - a GPU
   can test about a billion hashes per second.
 - Assuming passwords are random lowercase letters:
 - An 8 character password? (2 minutes)
 - A 10 character password? (17 hours)
 - A 12 character password? (1 year)
 
 12 character passwords look OK here, but unfortunately real passwords don't
 tend to be random lowercase letters. They tend to be words, maybe with the
 first letter capitalized and numbers tacked on the end.
 
Bad Plan C: Store hashes from a password hashing function (e.g. argon2)

 - This makes time to test a password tunable.
 - You can get back to the 100/second rate for an online attack.
 - Except: The attacker can pre-calculate the hashes.
   - This is called building a "rainbow table".
   - They hash all their guesses up front, and then can compare
     the hashes with the ones in your DB.

Good Plan: Password hashing function + salt

 - Hash the password + a random number (the salt)
 - Store the salt with the hash
 - Now rainbow tables don't work - you'd need to hash every password
   for every possible salt.
   
## Password Requirements?

 - You've all seen web sites with password rules:
   - You must have an uppercase letter, a lowercase letter, a number, an arabic character.
   - Your password must be at least six letters long.
   - You must change your password every week.
   - You can't reuse passwords.
 - These rules reliabiliy produce a specific form of password: "p4ssW0rd17", where 17
   is the number of times this password has been changed.

Don't do this. Instead, password requirements should look like this:

 - Your password must be at least 10 characters.
 - You'll never need to change it unless the password DB leaks.
 - Your password can't contain a common password, like "p4ssW0rd".
   
NIST password guidelines: https://pages.nist.gov/800-63-3/sp800-63b.html


## Add Passwords to Lens
 
 * Start branch: 6-tags
 * End branch: 7-passwords

We need to store a password hash in the users table.

Create a migration:

```
$ mix ecto.gen.migration add_password_hash
```

Edit the migration;

```
  def change do
    alter table("users") do
      add :password_hash, :string, default: "", null: false
    end
  end
```

```
$ mix ecto.migrate
```

Now we need to install a module for secure password hashing.

in mix.exs:

```
defp deps do
  ...
  {:argon2_elixir, "~> 2.0"},
```

```
$ mix deps.get
```

Now we need to set up registration.

In .../templates/user/form.html.eex:

```
  <div class="form-group">
    <%= label f, :password %>
    <%= password_input f, :password, class: "form-control" %>
    <%= error_tag f, :password %>
  </div>

  <div class="form-group">
    <%= label f, :password_confirmation %>
    <%= password_input f, :password_confirmation, class: "form-control" %>
    <%= error_tag f, :password_confirmation %>
  </div>
```

In .../users/user.ex:

```
  schema "users" do
    ...
    field :password_hash, :string
   
    ...
    field :password, :string, virtual: true
    field :password_confirmation, :string, virtual: true
  ...

  def changeset(user, attrs) do
    user
    |> cast(attrs, [:email, :name, :password, :password_confirmation])
    |> validate_confirmation(:password)
    |> validate_length(:password, min: 8) # too short
    |> hash_password()
    |> validate_required([:email, :name, :password_hash])
  end

  def hash_password(cset) do
    pw = get_change(cset, :password)
    if pw do
      change(cset, Argon2.add_hash(pw))
    else
      cset
    end
  end
```

In .../session_controller.ex:

```
  def create(conn, %{"email" => email, "password" => password}) do
    user = Lens.Users.authenticate(email, password)
```

In .../users.ex

```
  def authenticate(email, pass) do
    user = Repo.get_by(User, email: email)
    case Argon2.check_pass(user, pass) do
      {:ok, user} -> user
      _ -> nil
    end
  end
```

Before we can test logins, we need to fix a big in .../layouts/app.html.eex:

```
  <%= if @current_user do %>
    <%= nav_link(@conn, "Your Photos",
      Routes.user_path(@conn, :show, @current_user)) %>
  <% end %>
```

Update the login form in .../session/new.html.eex:

```
<h2>Log In</h2>
...
  <div class="form-group">
    <%= label f, :password %>
    <%= password_input f, :password, class: "form-control" %>
    <%= error_tag f, :password %>
  </div>
```

Update the create user action in .../user_controller.ex:

```
  def create(...) do
    ...
    |> redirect(to: Routes.page_path(conn, :index))
```


# Deep Preloads

Let's take a look at another application: Inkfish.

 * Clone the inkfish repo.

Grade belongs-to sub belongs-to assignment belongs-to bucket belongs-to course.

 * Load up my HW01 submission on Inkfish.
 * Show the Feedback grade.
 * Scroll down to the course footer.

So to display a grade, we need the course struct. Let's look at the code for
that.

apps/inkfish\_web/lib/inkfish\_web/grades.ex:

 * Find get\_grade!
 * Explain the preload: line.
 * Preload operates as a series of queries by default.
   * Get the grade
   * Find the grade column for the grade
   * Find the sub for the grade
   * Find the reg for the sub
   * Find the user for the reg
   * Find the team for the sub
   * Find the regs for the team
   * Find the users for the regs
 * In this example I'm doing explicit joins, with the join
   results bound to names that are referenced in the preload.
   * Inner join means we need to always find something.
   * Left (outer) join means we may get multiple rows back.
   * Too many outer joins can mean an explosion in result size.
 * This allows Ecto to do one complex query instead of 9 simpler ones.
 * This *should* be faster, but you'd need to profile to be sure.
 * In general, I don't recommend explicit joins. It's an optimization, and
   optimizations before profiling are a bad idea.
 * This query was definitely easier to write in Ecto than in raw SQL.

Performance:

 * grade/show on my sub takes ~130ms to load
 * sub/show takes ~7ms, with nearly as complex a DB query
 * Why?
 * grade/show loads the entire submission from disk and sends
   it as part of the payload. Disk I/O isn't the fastest.

Overflow:

 - Switch to staff mode and grade my HW01 submission.
 - Adding and editing comments uses $.ajax like the tags example.
 - The left column is a react component. The code viewer isn't. This doesn't
   result in the cleanest code.

