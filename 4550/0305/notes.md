
# HW Questions: CRUD App



# PhotoBlog: Deployment


# PhotoBlog: Votes on Posts

 - Users should be able to thumbs up or
   thumbs down posts in the feed.
 - We'll be doing this using 2008-style
   async requests:
    - jQuery!

Problem: We didn't specify user_id

And we can't just specify it in our JS code, because
then people could vote for other users.

We can't just pull it out of the conn object, because
that comes from the session, and APIs normally don't
use cookies. APIs are stateless, like HTTP/1.0.

Probably the easiest solution would be to just
grab it from the cookie, but then "api" is a bad
name for the pipeline.

The correct API solution is to add some sort of
authentication token to the request.

# Datepicker tips

 - Find a datapicker library that works with
   module "import".
 - Figure out what it needs as an initial
   form field.
 - Make sure your Elixir code is outputing that.
 - Then figure out how to activate the date picker
   and do that.
 - Make sure the date picker outputs something
   the Elixir code wants.

Anything that results in a pop up calendar style
date / time picker is fine.

How you deal with timezones is a design question.
Document and implement your choice.






