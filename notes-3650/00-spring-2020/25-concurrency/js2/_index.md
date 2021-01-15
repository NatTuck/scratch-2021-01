+++
title = "./25-concurrency/js2/notes.md"
+++

Conditions for a data race:

 - Concurrency + premption or parallelism. 
 - Shared data.
 - Mutable data.
 
## Plan A: Restricted Concurrency, No parallelism

Example Platform: JavaScript

Stuff JS programs need to handle:

Browser:

 - User clicks a button.
 - Request data from a server, do something when the response comes in.
 - Trigger a movie to start playing.

Server:

 - Accept incoming request, perform database query, send response quen query finishes.

Thoughts:

 - These things are all innately concurrent. We don't know when they'll happen.
 - These things are fast to process. Most of the time the program is waiting for
   some external event to occur.
 - We don't need parallelism - we can use a sequential event loop.

JavaScript in the browser works as follows:

 - The JavaScript runtime sits in a loop, running jobs as they occur.
 - As an HTML page loads, each "script" tag is a job. These are scheduled to run
   in order.
   - Yes, external scripts are guaranteed to run in source order.
 - These script tags can schedule both events and event handlers.





