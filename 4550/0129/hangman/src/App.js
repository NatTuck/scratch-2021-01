import { useState } from 'react';
import { uniq, bad_guesses, word_view, lives_left } from './game';
import './App.css';

function GameOver({reset}) {
  return (
    <div className="App">
      <h1>Game Over!</h1>
      <p>
        <button onClick={reset}>
          Reset
        </button>
      </p>
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
  const [text, setText] = useState("");

  let view = word_view(secret, guesses);
  let bads = bad_guesses(secret, guesses);
  let lives = lives_left(secret, guesses);

  function updateText(ev) {
    let vv = ev.target.value;
    let cc = vv[vv.length - 1];
    setText(cc);
  }

  function guess() {
    let ng = uniq(guesses.concat(text));
    console.log("ng", ng);
    setGuesses(ng);
  }

  function keyPress(ev) {
    if (ev.key == "Enter") {
      guess();
    }
  }

  if (lives <= 0) {
    return <GameOver reset={() => setGuesses([])} />;
  }

  return (
    <div className="App">
      <h1>Word: {view.join(' ')}</h1>
      <h1>Bads: {bads.join(' ')}</h1>
      <h1>Lives: {lives}</h1>
      <p>
        <input type="text"
               value={text}
               onChange={updateText}
               onKeyPress={keyPress} />
        <button onClick={guess}>Guess</button>
      </p>
      <p>
        <button onClick={() => setGuesses([])}>
          Reset
        </button>
      </p>
    </div>
  );
}

export default App;
