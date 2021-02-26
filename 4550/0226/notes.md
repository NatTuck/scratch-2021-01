
# Homework Questions?

 - When a second user joins a game, you want
   to make sure to preserve the game state.
 - If you copied the BackupAgent stuff, make sure
   you understand every single line of it and why
   you need it.
 - Tracking multiple users in your game state:
   - Option 1: Nothing, but then you can't show
     a list of active users in the UI
     - You still want to associate usernames
       with guesses.
   - Option 2: Store list of users in the game
     state.
   - Problem: How to manage observer vs. player?
     - Is this a flag in the channel?
     - Or is it part of the game state?
 - There is no channel before the user joins a game.
 - Syncing timer between client and server:
   - The server picks a deadline: a timestamp about
     30 seconds in the future.
   - That deadline is used to calculate the sendAfter
     delay.
   - The deadline is sent to the browser.
   - The setInterval is just for updating the display
     with time left to the deadline.
 - What to do with unused GenServer? 
   - This is a design question.
   - In the spec for this assignment, it has info it
     needs to store: the scoreboard.
   - Therefore it can live "forever" (at least until
     the app restarts).
   - For apps in general, we probably want to exit
     unused processes.
 - Make sure you track game name and user name
   separately. Your channel should be named per game.
 - How do we know which user we are in the browser code?
   - Plan A: The channel knows, and tells us as part
     of the game view.
   - Plan B: Have in-browser state that isn't entirely
     from the server.
 - The big complication in web development is
   state management. We have a distributed system,
   which nessissarily means there are multiple
   components that each have their own state.
 - For HW06, we have state in at least three different
   places:
    - Per-game state (e.g. list of guesses), stored in 
      one GenServer process per game.
    - Per-connection state (e.g. user name), stored
      in one Channel process per user connection.
    - In-browser state, stored in React components.
      This may or may not depend entirely on data
      from the server.
 - Single-channel design
    - Every user connects to the same channel.
    - Game name is in the channel state.
    - All events in all games are broadcast to the channel.
    - The handle_out callback filters so users only
      get updates for the game they're in.
    - Disadvantage: With many players and many games,
      almost all handle_out calls are useless. This
      will be our bottleneck at many users.

# Continuing PhotoBlog

## Where are we?

 - We've got a Phoenix App
 - We've got two resources: posts, users
 - We've got the ability to log in and log out

## Migrations

 - Scripts that modify the database schema


## That finishes 03-add-users branch

 - We have users.
 - We have a relationship (1 user has many posts)
 - When you create a post it's tagged with your user_id

## Next exciting feature: Photo uploads

 


