
# Homework Questions?

 - Carefully read the assignment to see
   the description of players vs observers
   and the "ready" mechanism on the lobby/startup
   screen.
 - Player turns are simultaneous; guesses during
   a turn aren't revealed to other players until
   after the turn.

# CRUD App Example

## What is CRUD?

There's a whole class of apps that basically:

 - Have some important data
 - The app is a UI to look at and modify the data
   possibly with some business rules restricting
   what different users can do.
   
A good place to store the data is a relational (SQL)
database.

Our App is going to be performing CRUD operations:

 - Create
 - Retreive
 - Update
 - Delete

The Phoenix framework provides some tools to help 
build this sort of app. We have a concept of 
"resources", which we can apply CRUD operations to 
using a REST structure. Rest is a mapping of HTTP
actions to CRUD ops (e.g. Retrieve is GET).

## Our demo app: PhotoBlog

 - We have users with accounts.
 - Users can post pictures with text to their
   blog.
 - Users can view and comment on pictures.

The app source is on github at
https://github.com/NatTuck/photo_blog

## Interlude: RESTful routes

Restful routes / paths are a common structure for
modern web apps. This is especially true for
programmable APIs.

Every resource has a base path. For posts, it
was "/posts". We perform operations on the resource
and instance of it by doing HTTP requests to that
path or closely related paths.

 - Our resource is "posts".
 - Instances are stored in a "posts" table in
   our DB.
 - Base HTTP path is "/posts"
 - We support 5 operations:
   - List all posts: "GET /posts"
   - View one post (id = 10): "GET /posts/10"
   - Create a post: "POST /posts"
   - Update a post (id = 10): "PATCH /posts/10"
   - Delete a post (id = 10): "DELETE /posts/10"
 - There are two more paths for a traditional
   web UI.
   - Show form for new post: "GET /posts/new"
   - Show the form to edit a post: "GET /posts/edit/10"
   
Benefits of this structure:

 - Tradional websites where users access the paths with
   a browser:
   - Guarantees that bookmarks work.
   - Allows caching to work as expected.
   - Single page applications (e.g. the whole app is 
     one React component) access the same paths.
   - Interface exposed by server to browser code and
     external users is well defined and understood.
 - Non-browser HTTP clients
   - REST provides a useful structure for HTTP API

## User Sessions

 - We're going to treat a session as a special
   kind of resource.
 - Sessions aren't stored in the database.
 - Instead, sessions are stored in a cookie.





















