import React from 'react';
import ReactDOM from 'react-dom';
import _ from "lodash";

export default function hangman_init(root) {
  ReactDOM.render(<Hangman />, root);
}

// App state for Hangman is:
// {
//    word: String             // the word to be guessed
//    guesses: List of Letters // letters guessed so far (unique)
//    lives: Int               // initial lives
// }

class Hangman extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      word: "elephant",
      guesses: [],
      lives: 10,
    };
  }

  word_letters() {
    return this.state.word.split("");
  }

  bad_guesses() {
    let goods = this.word_letters();
    let bads = [];
    this.state.guesses.forEach( (gg) => {
      if (!goods.includes(gg)) {
        bads.push(gg);
      }
    });
    return bads;
  }

  lives_left() {
    return this.state.lives - this.bad_guesses().length;
  }

  on_guess(ev) {
    let st1 = _.assign({}, this.state, {
      guesses: _.uniq(ev.target.value),
    });
    this.setState(st1);
  }

  render() {
    return (
      <div>
        <div className="row">
          <div className="column">
            <Word letters={this.word_letters()} guesses={this.state.guesses} />
          </div>
          <div className="column">
            <Lives lives={this.lives_left()} max={this.state.lives} />
          </div>
        </div>
        <div className="row">
          <div className="column">
            <Guesses guesses={this.state.guesses} />
          </div>
          <div className="column">
            <GuessInput guesses={this.state.guesses}
                        on_guess={this.on_guess.bind(this)} />
          </div>
        </div>
      </div>
    );
  }
}

function Word(params) {
  let {letters, guesses} = params;
  let skeleton = _.map(letters, (xx, ii) => {
    let text = guesses.includes(xx) ? xx : "_";
    return <span style={{padding: "1ex"}} key={ii}>{text}</span>;
  });

  return (
    <div>
      <p><b>The Word</b></p>
      <p>{skeleton}</p>
    </div>
  );
}

function Lives(params) {
  let {lives, max} = params;
  return <div>
    <p><b>Guesses Left:</b></p>
    <p>{lives} / {max}</p>
  </div>;
}

function Guesses(params) {
  return <div>
    <p><b>Letters Guessed</b></p>
    <p>{params.guesses.sort().join(' ')}</p>
  </div>;
}

function GuessInput(params) {
  let {guesses, on_guess} = params;
  return <div>
    <p><b>Type Your Guesses</b></p>
    <p><input type="text" value={guesses.join('')} onChange={on_guess} /></p>
  </div>;
}

