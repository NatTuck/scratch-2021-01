import {Socket} from "phoenix";

let socket = new Socket(
  "/socket",
  {params: {token: ""}}
);
socket.connect();

let channel = socket.channel("game:1", {});

let state = {
  word: "______",
  guesses: [],
};

let callback = null;

// The server sent us a new state.
function state_update(st) {
  console.log("New state", st);
  state = st;
  if (callback) {
    callback(st);
  }
}

export function ch_join(cb) {
  callback = cb;
  callback(state);
}

export function ch_login(name) {
  channel.push("login", {name: name})
         .receive("ok", state_update)
         .receive("error", resp => {
           console.log("Unable to login", resp)
         });
}

export function ch_push(guess) {
  channel.push("guess", guess)
         .receive("ok", state_update)
         .receive("error", resp => {
           console.log("Unable to push", resp)
         });
}

export function ch_reset() {
  channel.push("reset", {})
         .receive("ok", state_update)
         .receive("error", resp => {
           console.log("Unable to push", resp)
         });
}

channel.join()
       .receive("ok", state_update)
       .receive("error", resp => {
         console.log("Unable to join", resp)
       });

channel.on("view", state_update);
