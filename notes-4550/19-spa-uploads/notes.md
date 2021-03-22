
# Registraton; Uploads

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

# More stuff: Photo Uploads!

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

