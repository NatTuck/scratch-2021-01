+++
title = "./20-canvas/notes.md"
+++

 - Project Questions?
 
## The Rest of the Semester

 - You're working on the final project.
 - The last two lectures are project presentations.
 - That leaves us four more lectures - we'll be doing bonus topics.
   - Today: HTML Canvas
   - We'll also look at WebAssembly, NoSQL databases, and a mystery topic.

## Today: The Canvas API

 - Normal HTML + CSS is the default way describe the visual content of a web page.
 - That gives us:
   - A dynamic layout made out of rectangles.
   - Dynamic text.
   - Dynamic selection of static images.
 - That's good enough for a wide variety of applications, including some games
   as we saw in the first project.
 - For some applications we'd rather be able to draw arbitrary things on the
   screen.
 - For that, HTML provides the Canvas API.
 - Canvas lets us chose between two basic modes: 2D and 3D
   - This gives us access to a vector drawing API similar to SVG files.
   - webgl2 - This lets us do hardware accelerated 3D graphics.
   
## Basic Canvas

 - Type 2d-canvas.html
 - This shows a way to use the canvas 2D API. There are other approaches.

## React Konva Example

```
$ git clone https://github.com/NatTuck/shapes
```

Minimal React-Konva example, in shapes.jsx:


```
...
import { Stage, Layer, Circle } from 'react-konva';

export default function start(node) {
  ReactDOM.render(<Shapes />, document.getElementById('root'));
}

class Shapes extends React.Component {
  constructor(props) {
    super(props);
  }

  render() {
    return <div>
      <Stage width={1024} height={768}>
        <Layer>
          <Circle radius={20} x={100} y={100} fill="red" />
        </Layer>
      </Stage>
    </div>;
  }
}
```

Extend it to bouncing balls:

```
import _ from 'lodash';

export default function start(node) {
  ReactDOM.render(<Shapes />, document.getElementById('root'));
}

let W = 1024;
let H = 768;
let R = 50;
let G = 2;

class Shapes extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      balls: [
        {x: 100, y: 700, dx: 0, dy: -1},
        {x: 200, y: 650, dx: 0, dy: -1},
        {x: 300, y: 750, dx: 0, dy: -1},
      ],
    };

    window.setInterval(this.tick.bind(this), 100);
  }

  tick() {
    let balls = _.map(this.state.balls, ({x, y, dx, dy}) => {
      if (y + dy < R/2) {
        dy = -dy;
      }
      else {
        dy -= G;
      }
      y += dy;
      return {x, y, dx, dy};
    });
    this.setState(_.assign({}, this.state, {balls}));
  }

  render() {
    let circles = _.map(this.state.balls, (bb, ii) =>
      <Circle key={ii} radius={R} x={bb.x} y={H - bb.y} fill="red" />);

    return <div>
      <Stage width={W} height={H}>
        <Layer>
          {circles}
        </Layer>
      </Stage>
    </div>;
  }
}
```

Now add an onClick event:

```
  // in the loop in tick
  if ((x + dx < R / 2) || (x + dx > W - (R/2))) {
    dx = -dx;
  }
  x += dx;
   
  // new method
  push(bb) {
    let balls = _.map(this.state.balls, (ball, ii) =>
      (ii == bb ? _.assign({}, ball, {dx: ball.dx + 10}) : ball));
    this.setState(_.assign({}, this.state, {balls}));
  }


  // in render
  <Circle key={ii} radius={R} x={bb.x} y={H - bb.y} fill="red"
          onClick={() => this.push(ii)} />);
```

## WebGL

Here's a fancy demo:

 - https://playcanv.as/e/p/44MRmJRU/

Now let's do the simplest thing from scratch:

 - We're going to draw a single triangle, using less than 200 lines of code.
 - This is kind of complicated - and will be boring if you've done modern
   OpenGL - but I think it's worth walking through to show that it's not so
   complicated as to be unapproachable. 
 - Type 3d canvas.

## Selected References

 - https://webgl2fundamentals.org/webgl/lessons/webgl-fundamentals.html
 - https://webgl2fundamentals.org/webgl/lessons/webgl-drawing-multiple-things.html

