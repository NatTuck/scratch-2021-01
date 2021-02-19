
# Homework Questions?

What does useEffect do?

 - This is for side effects needed to
   render.
 - For example, this helps in building react
   components wrapping non-react libraries.
 - This should not effect your react state.

# Multiplayer Hangman

 - All done.
 - Ready for HW06.

# New topic: CRUD Apps

Hangman and Bulls & Cows have:

 - Multiple interacting concurrent users
 - Dynamic view updates
 - Real time interaction
 - Ephemeral state - if the server reboots, we lose
   the game state

Applications like that are especially interesting, but
the majority of apps don't need them.

Consider an online store:

 - Two kinds of users: Customers and Staff
 - No need for interacting concurrent users
   or real-time anything.
 - Things do change, but we don't need to push
   view updates. "Add to cart" can just fail if
   an item is out of stock.
 - State must be persistent, stored reliably.
 - If the server reboots and we lose orders,
   the store loses money.

If we generalize enough, there are a whole class of
apps that look like this:

 - We have some important data.
 - The app consists of looking at and modifying that
   data.

We have a really good way of storing persistent 
data: A relational ("SQL") database.

 - Our data comes as a set of well-defined types.
   - e.g. A product has a name, description, and price.
 - There are defined relationships between our data.
   - e.g. A "cart item" may describe a product added
     to a shopping cart by a user.
 - We want to be able to validate our data and the
   relationships.
   - Otherwise we end up with corrupted / junk data.
   - If someone tries to add a product with 
     { name: "", description: 5, price: "toadstool" }
     we want our app to just say no.
   - Similarly, if someone tries to add product #3759 to
     their shopping cart and there is no such product,
     that should fail.
 - We want to be able to query our data based on the
   relationships.
   - e.g. Get all items in Alice's shopping cart.

A relational database stores data as a collection
of tables. 

 - Each table stores records of one type with
   an associated schema.
   - e.g. We have a table "users" containing all
     our user records.
 - (in our usage) Each table has a mandatory field,
   a unique numeric ID called "id".
 - A table can have fields which reference records in
   other tables by storing the ID of the other-table
   record.
   - e.g. We have a table ```help_requests``` with a
     ```user_id``` field containing the ID of the user
     who made the request.

We can treat an online store as a database plus a user
interface that allows users to do CRUD operations:
Create, Retrieve, Update, Delete

What are we selling in our online store?

 - Shirts, Shoes, other clothing items 

In an online store, what are our tables?

(Everything else being equal, we'd rather not
have any duplicate data if we can factor it out
into a single table.)

 - users
 - products {name, price, description, brandId}
 - stockItems { productId, year, size, color, count }
 - orders 
 - orderItem { orderId, stockItemId, count }
 - brands

From the web app perspective, things we can apply CRUD
operations to are called "resources". 

The Phoenix Framework provids some tools to help us build
resource centric (probalby CRUD) apps, using an application
structure called REST.

REST core idea:

 - CRUD operatoins map to HTTP verbs.
 - Create is PUT
 - Retrieve is GET
 - Update is PATCH
 - Delete is DELETE







