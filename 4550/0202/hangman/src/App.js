import { useState, useEffect } from 'react';
import 'milligram';

import { uniq, bad_guesses, word_view, lives_left } from './game';
import './App.css';

function SetTitle({text}) {
  useEffect(() => {
    let orig = document.title;
    document.title = text;

    // Cleanup function
    return () => {
      document.title = orig;
    };
  });

  return <div />;
}

function GameOver(props) {
  //let reset = props['reset'];
  let {reset} = props;

  // On GameOver screen,
  // set page title to "Game Over!"

  return (
    <div className="row">
      <SetTitle text="Game Over!" />
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

function App() {
  /*
    const [state, setState] = useState({
    secret: "elephant",
    guesses: [],
    });
  */
  const [secret, _setSecret] = useState("elephant");
  const [guesses, setGuesses] = useState([]);
  // fixme: guesses should be a set

  let view = word_view(secret, guesses);
  let bads = bad_guesses(secret, guesses);
  let lives = lives_left(secret, guesses);

  function guess(text) {
    let ng = uniq(guesses.concat(text));
    console.log("ng", ng);
    setGuesses(ng);
  }

  function reset() {
    console.log("do reset");
    setGuesses([]);
  }

  let body = null;

  if (lives > 0) {
    body = (
      <div>
        <div className="row">
          <div className="column">
            <p>Word: {view.join(' ')}</p>
          </div>
          <div className="column">
            <p>Lives: {lives}</p>
          </div>
        </div>
        <div className="row">
          <div className="column">
            <p>Bads: {bads.join(' ')}</p>
          </div>
        </div>
        <Controls reset={reset} guess={guess} />
      </div>
    )
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

export default App;
