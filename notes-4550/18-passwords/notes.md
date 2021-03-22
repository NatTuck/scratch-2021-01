
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
 - These rules consistently produce a specific form of password: "p4ssW0rd17",
   where 17 is the number of times this password has been changed.

Don't do this. Instead, password requirements should look like this:

 - Your password must be at least 10 characters.
 - You'll never need to change it unless the password DB leaks.
 - Your password can't contain a common password, like "p4ssW0rd".
   
NIST password guidelines: https://pages.nist.gov/800-63-3/sp800-63b.html


# Adding Passwords to PhotoBlog: branch 06-passwords

```
defp deps do
  ...
  {:argon2_elixir, "~> 2.0"},
```

```
$ mix deps.get
```

Now let's add password hashes to our seeds file:

```
# seeds.exs
defmodule Inject do

  def user(name, pass) do
    hash = Argon2.hash_pwd_salt("pass")
    Repo.insert!(%User{name: name, password_hash: hash})
  end
end

alice = Inject.user("alice", "test1")
bob = Inject.user("bob", "test2")
```

Now let's implement authentication for sessions:

In .../users.ex:

```
  def authenticate(name, pass) do
    user = Repo.get_by(User, name: name)
    case Argon2.check_pass(user, pass) do
      {:ok, user} -> user
      _ -> nil
    end
  end
```

And in the session controller:

```
defmodule PhotoBlogWeb.SessionController do
  use PhotoBlogWeb, :controller

  def create(conn, %{"name" => name, "password" => password}) do
    user = PhotoBlog.Users.authenticate(name, password)

    if user do
      sess = %{
        user_id: user.id,
        name: user.name,
        token: Phoenix.Token.sign(conn, "user_id", user.id)
      }
      conn
      |> put_resp_header(
        "content-type",
      "application/json; charset=UTF-8")
      |> send_resp(:created, Jason.encode!(%{session: sess}))
    else
      conn
      |> put_resp_header(
        "content-type",
      "application/json; charset=UTF-8")
      |> send_resp(:unauthorized, Jason.encode!(%{error: "fail"}))
    end
  end
end
```

Try logging in with a bad password.

```
// api.js
export function api_login(name, password) {
  api_post("/session", {name, password}).then((data) => {
    console.log("login resp", data);
    if (data.session) {
      let action = {
        type: 'session/set',
        data: data.session,
      }
      store.dispatch(action);
    }
    else if (data.error) {
      let action = {
        type: 'error/set',
        data: data.error,
      };
      store.dispatch(action);
    }
  });
}
```

Let's show errors:

Update store:

```
function error(state = null, action) {
  switch (action.type) {
    case 'session/set':
      return null;
    case 'error/set':
      return action.data;
    default:
      return state;
  }
}

// add error to combineReducers in root
```

Update Nav to show error:

```
function AppNav({error}) {
  let error_row = null;

  if (error) {
    error_row = (
      <Row>
        <Col>
          <Alert variant="danger">{error}</Alert>
        </Col>
      </Row>
    );
  }

  return (
    <div>
      <Row>
        <Col>
          <Nav variant="pills">
            <Link to="/">Feed</Link>
          <Link to="/users">Users</Link>
        </Nav>
        </Col>
        <Col>
          <LoginOrInfo />
        </Col>
      </Row>
      { error_row }
    </div>
  );
}

export default connect(({error})=>({error}))(AppNav);
```

Logout button


```
// Nav.js
import store from './store';

function SessionInfo({session}) {
  function logout(ev) {
    ev.preventDefault();
    store.dispatch({ type: 'session/clear' });
  }

  return (
    <p>
      Logged in as {session.name}
      <Button onClick={logout}>Logout</Button>
    </p>
  );
}
```

```
// store.js
function session(state = null, action) {
  switch (action.type) {
    case 'session/set':
      return action.data;
    case 'session/clear':
      return null;
    default:
      return state;
  }
}
```

Allow our session to survive page loads.

To do that, we'll use LocalStorage:

 * Local storage only stores strings.
 * Local storage stores data "forever".
 * We want to explicitly implement expiration.


```
function save_session(sess) {
  let session = Object.assign({}, sess, {time: Date.now()});
  localStorage.setItem("session", JSON.stringify(session));
}

function restore_session() {
  let session = localStorage.getItem("session");
  if (!session) {
    return null;
  }
  session = JSON.parse(session);
  let age = Date.now() - session.time;
  let hours = 60 * 60 * 1000;
  if (age < 24 * hours) {
    return session;
  }
  else {
    return null;
  }
}

function session(state = restore_session(), action) {
  switch (action.type) {
    case 'session/set':
      save_session(action.data);
      return action.data;
    case 'session/clear':
      return null;
    default:
      return state;
  }
}
```


## Let's add user registrations

We're going to try to replicate the standard screens for Users:

 * list
 * new
 * show (later)
 * edit (later)

First, let's cut user_form from the store. That we can handle with useState or
something.

Now we'll refactor a whole bunch of stuff.

First, over in App.js:

```
import Nav from './Nav';
import Feed from './Feed';
import UsersList from './Users/List';
import UsersNew from './Users/New';

function App() {
  return (
    <Container>
      <Nav />
      <Switch>
        <Route path="/" exact>
          <Feed />
        </Route>
        <Route path="/users" exact>
          <UsersList />
        </Route>
        <Route path="/users/new">
          <UsersNew />
        </Route>
      </Switch>
    </Container>
  );
}
```

Now, we're making Users a directory.

```
web-ui$ mkdir src/Users
web-ui$ mv src/Users.js src/Users/List.js
web-ui$ cp src/Users/List.js src/Users/New.js
```

And, in Users/List.js:

```
import { Row, Col } from 'react-bootstrap';
import { connect } from 'react-redux';
import { Link } from 'react-router-dom';

function UsersList({users}) {
  let rows = users.map((user) => (
    <tr key={user.id}>
      <td>{user.name}</td>
      <td>[Edit]</td>
    </tr>
  ));

  return (
    <div>
      <Row>
        <Col>
          <h2>List Users</h2>
          <p>
            <Link to="/users/new">
              New User
            </Link>
          </p>
          <table className="table table-striped">
            <thead>
              <tr>
                <th>Name</th>
                <th>Actions</th>
              </tr>
            </thead>
            <tbody>
              { rows }
            </tbody>
          </table>
        </Col>
      </Row>
    </div>
  );

}

function state2props({users}) {
  return { users };
}

export default connect(state2props)(UsersList);
```

Now the create user screen.

First, create a new API function in api.js:

```
export function create_user(user) {
  return api_post("/users", {user});
}
```

Now, for the new module, Users/New.js:

```
import { Row, Col, Form, Button } from 'react-bootstrap';
import { connect } from 'react-redux';
import { useState } from 'react'
import { useHistory } from 'react-router-dom';
import pick from 'lodash/pick';

import { create_user, fetch_users } from '../api';

function UsersNew() {
  let history = useHistory();
  const [user, setUser] = useState({name: "", pass1: "", pass2: ""});

  function check_pass(p1, p2) {
    if (p1 !== p2) {
      return "Passwords don't match.";
    }

    if (p1.length < 8) {
      return "Password too short.";
    }

    return "";
  }

  function update(field, ev) {
    let u1 = Object.assign({}, user);
    u1[field] = ev.target.value;
    u1.password = u1.pass1;
    u1.pass_msg = check_pass(u1.pass1, u1.pass2);
    setUser(u1);
  }

  function onSubmit(ev) {
    ev.preventDefault();
    console.log(user);

    let data = pick(user, ['name', 'password']);
    create_user(data).then(() => {
      fetch_users();
      history.push("/users");
    });
  }

  return (
    <Row>
      <Col>
        <h2>New User</h2>
        <Form onSubmit={onSubmit}>
          <Form.Group>
            <Form.Label>Name</Form.Label>
            <Form.Control type="text"
                          onChange={(ev) => update("name", ev)}
                          value={user.name || ""} />
          </Form.Group>
          <Form.Group>
            <Form.Label>Password</Form.Label>
            <Form.Control type="password"
                          onChange={(ev) => update("pass1", ev)}
                          value={user.pass1 || ""} />
            <p>{user.pass_msg}</p>
          </Form.Group>
          <Form.Group>
            <Form.Label>Confirm Password</Form.Label>
            <Form.Control type="password"
                          onChange={(ev) => update("pass2", ev)}
                          value={user.pass2 || ""} />
          </Form.Group>
          <Button variant="primary"
                  type="submit"
                  disabled={user.pass_msg !== ""}>
            Save
          </Button>
        </Form>
      </Col>
    </Row>
  );
}

function state2props() {
  return {};
}

export default connect(state2props)(UsersNew);
```

And... we get a 422.

We've got to set up the Elixir code to handle registration with
passwords.

In users/user.ex:

```
  def changeset(user, attrs) do
    user
    |> cast(attrs, [:name])
    |> add_password_hash(attrs["password"])
    |> validate_required([:name, :password_hash])
  end

  def add_password_hash(cset, nil) do
    cset
  end

  def add_password_hash(cset, password) do
    change(cset, Argon2.add_hash(password))
  end
```

# More stuff: Photo Uploads! (branch: 07-new-post)


```
// Feed.js
import { Link } from 'react-router-dom';
...
      <h2>Feed</h2>
      <p><Link to="/posts/new">New Post</Link></p>
```

```
// App.js
import PostNew from './Posts/New';
...
        <Route path="/posts/new">
          <PostNew />
        </Route>
```

Now we need to create Posts/New.js:

```
import { Row, Col, Form, Button } from 'react-bootstrap';
import { useState } from 'react'

export default function PostsNew() {
  let [post, setPost] = useState({});

  function onSubmit(ev) {
    ev.preventDefault();
    console.log(ev);
    console.log(post);
  }

  function updatePhoto(ev) {
    let p1 = Object.assign({}, post);
    p1["photo"] = ev.target.files[0];
    setPost(p1);
  }

  function updateBody(ev) {
    let p1 = Object.assign({}, post);
    p1["body"] = ev.target.value;
    setPost(p1);
  }

  // Note: File input can't be a controlled input.
  return (
    <Row>
      <Col>
        <h2>New Post</h2>
        <Form onSubmit={onSubmit}>
          <Form.Group>
            <Form.Label>Photo</Form.Label>
            <Form.Control type="file" onChange={updatePhoto} />
          </Form.Group>
          <Form.Group>
            <Form.Label>Body</Form.Label>
            <Form.Control as="textarea"
                          rows={4}
                          onChange={updateBody}
                          value={post.body} />
          </Form.Group>
          <Button variant="primary" type="submit">
            Save
          </Button>
        </Form>
      </Col>
    </Row>
  );
}
```

Ok, now let's update it to actually do stuff:

```
import { create_post } from '../api';
...
  function onSubmit(ev) {
    ...
    create_post(post);
```

So we need create_post in api.js:

```
export function create_post(post) {
  let data = new FormData();
  data.append("post[photo]", post.photo);
  data.append("post[body]", post.body);
  fetch("http://localhost:4000/api/v1/posts", {
    method: 'POST',
    // Fetch will handle reading the file object and
    // submitting this as a multipart/form-data request.
    body: data,
  }).then((resp) => {
    console.log(resp);
  });
}
```

More stuff to do;

 * Hide "new post" unless logged in.
 * Copy over RequireAuth from old app.
 * controller: Save the photo file, get hash, save to DB.

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

