
# Project Proposal Questions


# Current Issues in App

 - Simulate a multi-page app using React.
 - In-browser state management

## The Redux Pattern

 - We have one (in-browser) immutable state 
   object for our app.
 - That state object is made accessible
   throughout our app.
 - The state only changes in response to
   "actions", which will produce a new
   state value.

Redux plan:

 - Our immutable state value is stored in an 
   object called "the store".
 - When an event happens, we dispatch an action
   to the store, which will cause a new state
   value to be produced.
 - This new value is produced through a series
   of functions we right - called reducers - which
   take (state, action) pair and produce a new
   state value.
 - Reducers should be pure functions.

Simulating multiple pages:

 - We could just make our current screen
   be part of our state. Then our root component
   is a big if-the-else or switch.
 - Problem: There's some tricky stuff to do with
   back buttons and URL bars.
 - The react-router library handles this stuff
   pretty well.
   
Sessions in our SPA:

 - Can't use cookies, because we're using an external
   API.
 - We want to use a token.
 - We want to store that token as part of our client-side
   state.
