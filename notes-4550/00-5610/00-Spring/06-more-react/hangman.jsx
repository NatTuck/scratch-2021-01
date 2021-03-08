import React from 'react';
import ReactDOM from 'react-dom';

export default function hangman_init(root) {
  ReactDOM.render(<Hangman />, root);
}

// App state for Hangman is:
// {
//    word: String     // the word to be guessed
//    guesses: String  // letters guessed so far
//    max_guesses: Int // initial lives
// }
//
// A TodoItem is:
//   { name: String, done: Bool }


class Hangman extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      word: "elephant",
      guesses: "",
      max_guesses: 10,
    };
  }

  wordLetters() {
    return this.state.word.split("");
  }

  guessLetters() {
    return _.uniq(this.state.guesses.split(""));
  }

  badGuessLetters() {
    let goods = this.wordLetters();
    let bads = [];
    this.guessLetters().forEach( (gg) => {
      if (!goods.includes(gg)) {
        bads.push(gg);
      }
    });
    return bads;
  }

  guessesLeft() {
    return this.state.max_guesses - this.badGuessLetters().length;
  }

  setGuesses(ev) {
    let input = $(ev.target);
    let st1 = _.extend(this.state, {
      guesses: input.val(),
    });
    this.setState(st1);
  }

  render() {
    return (
      <div>
        <div className="row">
          <div className="column">
            <Word root={this} />
          </div>
          <div className="column">
            <Lives root={this} />
          </div>
        </div>
        <div className="row">
          <div className="column">
            <Guesses root={this} />
          </div>
          <div className="column">
            <GuessInput guess={this.setGuesses.bind(this)} />
          </div>
        </div>
      </div>
    );
  }
}

function Word(params) {
  let root = params.root;

  let guesses = root.guessLetters();
  let letters = _.map(root.wordLetters(), (xx, ii) => {
    let text = guesses.includes(xx) ? xx : "_";
    return <span style={{padding: "1ex"}} key={ii}>{text}</span>;
  });

  return (
    <div>
      <p><b>The Word</b></p>
      <p>{letters}</p>
    </div>
  );
}

function Lives(params) {
  return <div>
    <p><b>Guesses Left:</b></p>
    <p>{params.root.guessesLeft()}</p>
  </div>;
}

function Guesses(params) {
  return <div>
    <p><b>Letters Guessed</b></p>
    <p>{params.root.guessLetters().join(" ")}</p>
  </div>;
}

function GuessInput(params) {
  return <div>
    <p><b>Type Your Guesses</b></p>
    <p><input type="text" onChange={params.guess} /></p>
  </div>;
}
