
# Web Dev Lecture 4: Introducing React

## Today: Build "Hangman" with React

 - Purely in browser
 - One web page

## Dev Tools

First thing: Node Package Manager

Node version manager: https://github.com/nvm-sh/nvm

## Setting up a project

https://create-react-app.dev/

Create react app gives us:

 * A JavaScript application / Web Site
 * A build system with Webpack and Babel
 * A testing setup with Jest
 * A JS webserver to run during development
   with live updates
 * etc

## JSX

We've got XML in our JavaScript.

JSX example:

```
function hello(name) {
    return (<p>Hello, <i>{name}</i>!</p>);
}
```

JavaScript that's generated:

```
function hello(name) {
    return xml_tag('p', [
        "Hello, ",
        xml_tag('i', [name]),
        "!"
    ]);
}
```

This has some advantages:

 * We can check for basic XML validity at compile time.
 * We build a data structure.
   * We can do processing on it.
   * This structure is called a Virtual DOM.
 * That lets us do fast updates.
 * When rendering the data structure to the browser,
   we never generate a string.
   * Instead, document.createElement is used.
 * When updating the exisiting page, it can compare
   the existing elements to the data structure and only
   make the optimum minimal changes.
 * That's faster, because updates are the slow thing
   in the browser.
 * That means our side of a React app can act kind of
   like a 3D rendering engine in a game.
   * Each frame is rerendered from scratch based on
     the state of the game.
   * No need to consider the previous frame at all.

## Counter App

 - Web page displays a number, starting at zero.
 - Button "+1" increases number by one.
 - Button "Reset" sets number to zero.
 
## Making the buttons work

 - Functions that generate JSX in react are called
   "render" functions.
 - Rule: Render functions must be "pure", in the
   functional programming sense.
   - Must produce a result value dependent only
     on the function arguments.
   - No side effects.
   - That includes no mutating state.
   - We also can't reference external state.
 - Exception: React provides "hooks" to do side effects
   without breaking things.
 - Rule: State values cannot be mutated.
   - New values must be created and setState should be
     used with those new values.

The useState hook returns two things:

 - Current value of the state.
 - Set function that:
   - Updates the state with a new value.
   - Triggers a re-render of the component.
 
## Building Hangman

To build a React component, it's useful to start with
two basic questions:

 * What UI do we need to render?
 * What state is needed to do that rendering?

What is Hangman?
 
 * The computer picks a secret word.
 * We display the word, with all the letters hidden.
 * The user guesses letters.
 * If the user guesses a letter in the word, all
   instances of that letter are revealed.
 * If the user guesses >7 letters not in the word,
   they lose.
 * If the whole word is revealed, the user wins.

What state do we need:

 * The secret word.
 * The user's guesses 
 * Other stuff can be derived (e.g. how many bad guesses)

Form fields and React, two options:

Uncontrolled inputs:

 * Work normally, with their own state.
 * Require very careful thought to make sure that
   doesn't conflict with React rerendering.

Controlled inputs:

 * Specify "onChange" and "value" attributes.
 * State of input is explicilty part of the state
   of the React component.
 * Therefore this will reliably work as expected
   even in weird cases.








 


   
   
   
 
 
















