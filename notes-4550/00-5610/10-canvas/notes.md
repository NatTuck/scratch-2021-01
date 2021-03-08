---
layout: default
---

 - Project 1 has started. You've got 2.5 weeks left.
 - Project Questions?
 
## Multiplayer Hangman Review

Pull up the multiplayer hangman demo; walk through the usage.

 - Main screen: Select table and user name.
 - Game screen: We load the game screen and join the channel associated
   with our game table.
 - Each player has a separate channel connection, so they have a separate
   socket instance.
 - On join, the socket requests the game state from a GenServer that's
   specific to the current table.
 - When a player makes a move:
   - Push message to channel
   - Channel module pushes action to GenServer
   - GenServer initiates broadcast of updated GameServer
   - Channel module can filter output (with handle_out callback)
 - Chat
   - Chat can simply be more events on the channel. When a user chats,
     push a chat message to the channel then broadcast it.
   - Observers can chat, only players can take game actions.

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

Now let's walk through the code for the simplest WebGL example: One Triangle

 - Show 3D canvas demo.
 - Walk through code (don't type it).

Steps in the code:

 - Start at the bottom, the "load" handler.
 - Go to main.
   - Get gl2 context
   - Build our shader program. This is code that compiled in the user's
     browser then runs on their GPU.
   - For every frame, we call draw.
 - Look at the shader program
   - First part: Vertex shader. This decides where the vertexes go.
   - Second part: Fragment shader. This decides what color pixels are.
   - Our shaders are boring - they just write their input to their output.
 - Shaders, every frame:
   - The vertex shader is run for each vertex (in parallel) to decide
     positions.
   - The fragment shader is run for each pixel (in parallel) to decide
     which color the pixel is.
 - Shader build process
   - Like a C program: compile each piece and then link together to a program
 - Draw function
   - Create a vertex array - note that a 1d array is used to store 2d data.
   - Put it in an array buffer - this is a real C style array of floats.
   - Reinterpret it as an array of 2-item vectors.
   - For our fragments, we just pass in a single color.
   - Once we have our data set up, we clear the viewport and draw triangles.

## react-three-fiber

Since you're all working on Project 1, the obvious question is "How do we use a
WebGL canvas with React?"

I've got an example for that.

 - Go back to shapes code
 - Make a new branch '3d-react-canvas'
 - Pull in a dep:

```
$ npm install react-three-fiber
```

react-three-fiber is a react wrapper around THREE.js, a common utility library
that's used with WebGL.

This library is built to deal with some important tradeoffs:

 - React is pretty efficient at generating new render trees.
 - But not efficient enough to want to be calling the render method every
   single frame, as we saw in the bouncing balls demo.

The result is that React is used for large scale manipulations (e.g. which
objects appear), but constant animations can be handled in dedicated callbacks.
This still isn't quite as efficient as writing our own shaders and handling
animations there would be, but we usually don't need quite that level of
performance.

```
import React, {useRef} from 'react';
import ReactDOM from 'react-dom';
import {Canvas, useFrame} from 'react-three-fiber';
import * as THREE from 'three';

export default function start(node) {
  ReactDOM.render(<Shapes />, document.getElementById('root'));
}

class Shapes extends React.Component {
  render() {
    return (
      <Canvas style=\{\{height: "800px"\}\}>
        <Spinner x={-1} />
        <Spinner x={+1} />
      </Canvas>
    );
  }
}

function Spinner({x}) {
  const ref = useRef()
  useFrame(() => (ref.current.rotation.z += 0.01))
  return (
    <mesh
      ref={ref}
      onClick={e => console.log('click')}
      onPointerOver={e => console.log('hover')}
      onPointerOut={e => console.log('unhover')}
      position={new THREE.Vector3(x, 0, 0)}>
      <circleBufferGeometry attach="geometry" args={[2, 10]} />
      <meshBasicMaterial wireframe={true} attach="material"
                         color="hotpink" opacity={0.5} transparent />
    </mesh>
  )
}
```

## Selected References

 - https://webgl2fundamentals.org/webgl/lessons/webgl-fundamentals.html
 - https://webgl2fundamentals.org/webgl/lessons/webgl-drawing-multiple-things.html
 - https://github.com/react-spring/react-three-fiber

## Overflow

Go back to hangman and add features:

 - Every 10 seconds, the game guesses a random letter on its own.
 - Filter input for valid characters.

