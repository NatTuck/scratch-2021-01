+++
title = "./14-multi-and-passwords/notes.md"
+++

# First: Homework Questions?

# Next HW: Project Proposal

# Suggested Products

start: 6-ratings; this feature: 7-suggested

 - Amazon runs a complex algorithm to do this.
 - We're going to be lazy and just hard code our list of related products in the
   DB.

A product has many related products.

 - Problem: Many to many relation
 - Solution: We need a join table.

Table "suggestions":

 - id
 - product\_id
 - suggested\_id

Do we need to store any other information about our (product, product) pair? Is
the join table just storing a list associated with a product, or are the records
in the table themselves a useful resource with their own fields?

For example, the ratings table would be a simple join table between users and
products if it didn't have a "stars" field.

```
$ mix ecto.gen.migration create_suggestions
```

* Edit the migration

```
  def change do
    create table("suggestions") do
      add :product_id, references(:products, on_delete: :delete_all), null: false
      add :suggested_id, references(:products, on_delete: :delete_all), null: false
    end

    create index(:suggestions, [:product_id])
  end
```

 * mix ecto.migrate

Hook up our relation.

lib/husky\_shop/products/product.ex

```
  many_to_many :suggestions, HuskyShop.Products.Product,
    join_through: "suggestions",
    join_keys: [product_id: :id, suggested_id: :id]
```

Add some suggestions to our seeds file.

priv/repo/seeds.exs

```
c = Repo.insert!(%Product{name: "Cookie", desc: "chocolate oatmeal",
                          price: Decimal.new("0.99"), inventory: 2})
d = Repo.insert!(%Product{name: "Donut", desc: "chocolate frosted",
                          price: Decimal.new("1.19"), inventory: 2})
Repo.insert!(%Product{name: "Pie", desc: "banana cream",
                      price: Decimal.new("5.99"), inventory: 2,
                      suggestions: [c,d] })
```

Preload in get query.

```
  def get_product!(id) do
    Repo.one! from p in Product,
      where: p.id == ^id,
      preload: [:ratings, :suggestions]
  end
```

Show suggestions on show page.

```
<div>
  <h2>Suggestions</h2>
  <ul>
    <%= Enum.map @product.suggestions, fn sg -> %>
      <li><%= link sg.name, to: Routes.product_path(@conn, :show, sg.id) %></li>
    <% end %>
  </ul>
</div>
```

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

## Example: Adding Passwords to HuskyShop

(didn't get to this part - it's in the next lecture)

start branch: 7-suggested; end branch: 8-passwords

Add the comeonin library and Argon2 password hash algo:

```
  # mix.exs deps
  {:comeonin, "~> 4.1"},
  {:argon2_elixir, "~> 1.3"},
```

Fetch deps; create migration:

```
$ mix deps.get
$ mix ecto.gen.migration add_passwords
```

In the new migration file:
```
  def change do
    alter table("users") do
      add :password_hash, :string
      add :pw_tries, :integer, null: false, default: 0
      add :pw_last_try, :utc_datetime
    end
  end
```

Migrate!

```
$ mix ecto.migrate
```

**Show changes to user schema file**.

Add passwords to the seeds file:

```
pwhash = Argon2.hash_pwd_salt("pass1")

Repo.insert!(%User{email: "alice@example.com", admin: true, password_hash: pwhash})
Repo.insert!(%User{email: "bob@example.com", admin: false, password_hash: pwhash})
```

Reload from seeds:

```
$ mix ecto.reset
```

Update the user form:

```
   <div class="form-group">
    <%= label f, :password, class: "control-label" %>
    <%= password_input f, :password, class: "form-control" %>
    <%= error_tag f, :password %>
  </div>

  <div class="form-group">
    <%= label f, :password_confirmation, class: "control-label" %>
    <%= password_input f, :password_confirmation, class: "form-control" %>
    <%= error_tag f, :password_confirmation %>
  </div>
```

Update the login form (app layout):

```
<%= form_for @conn, Routes.session_path(@conn, :create),
         [class: "form-inline"], fn f -> %>
  <%= text_input(f, :email, class: "form-control col-4",
    placeholder: "email") %>
  <%= password_input(f, :password, class: "form-control col-4",
    placeholder: "pass") %>
  <%= submit "Login", class: "btn btn-secondary" %>
<% end %>
```

Validate the password on login (session\_controller):

```
  def create(conn, %{"email" => email, "password" => password}) do
    user = get_and_auth_user(email, password)
    ...
  end
  
  # TODO: Move to user.ex
  def get_and_auth_user(email, password) do
    user = HuskyShop.Users.get_user_by_email(email)
    case Comeonin.Argon2.check_pass(user, password) do
      {:ok, user} -> user
      _else       -> nil
    end
  end

  # TODO: Throttle Atttempts
```



