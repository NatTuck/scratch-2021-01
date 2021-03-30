import { useState, useEffect } from 'react';
import { Stage, Layer, Circle } from 'react-konva';
import _ from 'lodash';

let W = 1024;
let H = 768;
let R = 50;
let G = 2;

let state0 = {
  balls: [
    {x: 100, y: 700, dx: 0, dy: -1},
    {x: 200, y: 650, dx: 0, dy: -1},
    {x: 300, y: 750, dx: 0, dy: -1},
  ],
};

let on_tick = null;

function App() {
  const [state, setState] = useState(state0);

  function tick() {
    let balls = _.map(
      state.balls,
      ({x, y, dx, dy}) => {
        if ((x + dx < R/2) || (x + dx > W - (R/2))) {
          dx = -dx;
        }
        x += dx;
        if (y + dy < R/2) {
          dy = -dy;
        }
        else {
          dy -= G;
        }
        y += dy;
        return {x, y, dx, dy};
      }
    );
    setState(_.assign({}, state, {balls}));
  }

  function push(bb) {
    let balls = _.map(
      state.balls,
      (ball, ii) => (
        (ii == bb ?
         _.assign({}, ball, {dx: ball.dx + 10}) :
         ball)
      )
    )
    setState(_.assign({}, state, {balls}));
  }

  useEffect(() => {
    on_tick = tick;
  });

  let circles = _.map(
    state.balls,
    (bb, ii) => (
      <Circle key={ii} radius={R} x={bb.x}
              y={H - bb.y} fill="red"
              onClick={() => push(ii)} />
    )
  );

  return (
    <div>
      <Stage width={1024} height={768}>
        <Layer>
          { circles }
        </Layer>
      </Stage>
    </div>
  );
}

setInterval(() => {
  if (on_tick) {
    on_tick();
  }
}, 100);

export default App;
