
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
    let action = {
      type: 'session/set',
      data: data,
    }
    store.dispatch(action);
  });
}

export function load_defaults() {
  fetch_posts();
  fetch_users();
}
