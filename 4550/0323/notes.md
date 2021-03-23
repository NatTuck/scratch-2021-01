

# First: Homework Questions

 - Final programming HW due on Friday
 - Then project
 - Then wrap-up HW

For HW, you need to use a salt and store it
in the DB (which the Argon2 module will do
for you).

# Today: Finishing up work on SPA example

 - Next: Building Registration!

We'll be building two of the four standard screens
for users:

 - List (done)
 - New (to do)
 - Show (not today)
 - Edit (not today)


# Deploying a split app

## Deploy both on the same hostname

 - Deploy the react app as usual as
   a static site with Nginx.
 - Deploy the Phoenix app in the same
   Nginx config file, but only do the
   reverse proxy for the "/api" path
   (and maybe the "/socket" path).
 - In this case, you don't need CORS
   headers.

## Deploy on two different hostnames.

 - Deploy the Phoenix app as usual.
   - Make sure CORS is configured right,
     specifically with URL for frontend.
 - Deploy the React app as a static website,
   just like earlier in the semester.

## In either case

 - You need to handle non-root paths on the
   frontend app.
 - Non-root paths only exist in the brower.
 - With a static deployment under Nginx, you want
   every path to be an alias for the root path
   - e.g. User requests "/users", they get the same
     data as if they requested "/"
   - This needs to be an alias, not a redirect.

