import React, { useState, useEffect } from 'react';
import 'milligram';

import { ch_join, ch_push,
         ch_login, ch_reset } from './socket';

function GameOver(props) {
  //let reset = props['reset'];
  let {reset} = props;

  // On GameOver screen,
  // set page title to "Game Over!"

  return (
    <div className="row">
      <div className="column">
        <h1>Game Over!</h1>
        <p>
          <button onClick={reset}>
            Reset
          </button>
        </p>
      </div>
    </div>
  );
}

function Controls({guess, reset}) {
  // WARNING: State in a nested component requires
  // careful thought.
  // If this component is ever unmounted (not shown
  // in a render), the state will be lost.
  // The default choice should be to put all state
  // in your root component.
  const [text, setText] = useState("");

  function updateText(ev) {
    let vv = ev.target.value;
    let cc = vv[vv.length - 1];
    setText(cc);
  }

  function keyPress(ev) {
    if (ev.key == "Enter") {
      guess(text);
    }
  }

  return (
    <div className="row">
      <div className="column">
        <p>
          <input type="text"
                 value={text}
                 onChange={updateText}
                 onKeyPress={keyPress} />
        </p>
      </div>
      <div className="column">
        <p>
          <button onClick={() => guess(text)}>Guess</button>
        </p>
      </div>
      <div className="column">
        <p>
          <button onClick={reset}>
            Reset
          </button>
        </p>
      </div>
    </div>
  );
}

function reset() {
  console.log("Time to reset");
  ch_reset();
}

function Play({state}) {
  let {word, guesses, name} = state;

  function guess(text) {
    // Inner function isn't a render function
    ch_push({letter: text});
  }

  let view = word.split('');
  let bads = [];

  // FIXME: Correct guesses shouldn't count.
  let lives = 8 - guesses.length;

  return (
    <div>
      <div className="row">
        <div className="column">
          <p>Word: {view.join(' ')}</p>
        </div>
        <div className="column">
          <p>Name: {name}</p>
        </div>
      </div>
      <div className="row">
        <div className="column">
          <p>Guesses: {guesses.join(' ')}</p>
        </div>
        <div className="column">
          <p>Lives: {lives}</p>
        </div>
      </div>
      <Controls reset={reset} guess={guess} />
    </div>
  );
}

function Login() {
  const [name, setName] = useState("");

  return (
    <div className="row">
      <div className="column">
        <input type="text"
               value={name}
               onChange={(ev) => setName(ev.target.value)} />
      </div>
      <div className="column">
        <button onClick={() => ch_login(name)}>
          Login
        </button>
      </div>
    </div>
  );
}

function Hangman() {
  // render function,
  // should be pure except setState
  const [state, setState] = useState({
    name: "",
    word: "",
    guesses: [],
  });

  useEffect(() => {
    ch_join(setState);
  });

  let body = null;

  if (state.name === "") {
    body = <Login />;
  }
  // FIXME: Correct guesses shouldn't count.
  else if (state.guesses.length < 8) {
    body = <Play state={state} />;
  }
  else {
    body = <GameOver reset={reset} />;
  }

  return (
    <div className="container">
      {body}
    </div>
  );
}

export default Hangman;
