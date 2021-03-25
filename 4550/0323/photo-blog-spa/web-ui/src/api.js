
import store from './store';

async function api_get(path) {
  let text = await fetch(
    "http://localhost:4000/api/v1" + path, {});
  let resp = await text.json();
  return resp.data;
}

async function api_post(path, data) {
  let opts = {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(data),
  };
  let text = await fetch(
    "http://localhost:4000/api/v1" + path, opts);
  return await text.json();
}

export function fetch_users() {
  api_get("/users").then((data) => {
    let action = {
      type: 'users/set',
      data: data,
    }
    store.dispatch(action);
  });
}

export function fetch_posts() {
  api_get("/posts").then((data) => {
    let action = {
      type: 'posts/set',
      data: data,
    }
    store.dispatch(action);
  });
}

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
      }
      store.dispatch(action);
    }
  });
}

export function create_user(user) {
  return api_post("/users", {user});
}

export async function create_post(post) {
  let state = store.getState();
  let token = state?.session?.token;

  let data = new FormData();
  data.append("post[photo]", post.photo);
  data.append("post[body]", post.body);
  let opts = {
    method: 'POST',
    body: data,
    headers: {
      'x-auth': token,
    },
    // fetch will magically do the right thing
    // with our FormData:
    //  - It's going to read the file
    //  - It's going to pick correct headers
    //  - multipart-form-data
  };
  let text = await fetch(
    "http://localhost:4000/api/v1/posts", opts);
  return await text.json();
}


export function load_defaults() {
  fetch_posts();
  fetch_users();
}
