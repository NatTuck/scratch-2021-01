import React from 'react';
import ReactDOM from 'react-dom';
import _ from "lodash";

export default function hangman_init(root, channel) {
  ReactDOM.render(<Hangman channel={channel} />, root);
}

// Client-Side state for Hangman is:
// {
//    skel:  List of letters and _ indicating where good guesses go.
//    goods: Set of letters, good guesses
//    bads:  Set of letters, bad guesses
//    lives: Int               // initial lives
// }

class Hangman extends React.Component {
  constructor(props) {
    super(props);

    this.channel = props.channel;
    this.state = {
      skel: [],
      goods: [],
      bads: [],
      lives: 10,
    };

    this.channel
        .join()
        .receive("ok", this.got_view.bind(this))
        .receive("error", resp => { console.log("Unable to join", resp); });
  }

  got_view(view) {
    console.log("new view", view);
    this.setState(view.game);
  }

  lives_left() {
    return this.state.lives - this.state.bads.length;
  }

  on_guess(ev) {
    this.channel.push("guess", { letter: ev.target.value.substr(-1) })
        .receive("ok", this.got_view.bind(this));
  }

  guesses() {
    return _.concat(this.state.goods, this.state.bads);
  }

  render() {
    return (
      <div>
        <div className="row">
          <div className="column">
            <Word skeleton={this.state.skel} />
          </div>
          <div className="column">
            <Lives lives={this.lives_left()} max={this.state.lives} />
          </div>
        </div>
        <div className="row">
          <div className="column">
            <Guesses guesses={this.state.bads} />
          </div>
          <div className="column">
            <GuessInput guesses={this.guesses()}
                        on_guess={this.on_guess.bind(this)} />
          </div>
        </div>
      </div>
    );
  }
}

function Word(params) {
  let {skeleton} = params;
  return (
    <div>
      <p><b>The Word</b></p>
      <p>{skeleton.join(" ")}</p>
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

