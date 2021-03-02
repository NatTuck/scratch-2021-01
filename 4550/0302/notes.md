
# Introduce new HW

 - Database URL, for our purposes, should
   usually have a host of "localhost".
 - Setting up DB on server is the same as
   for dev machine.
   - Install postgres deb packages.
   - Create app user in postgres.
   - Run the ecto.create / ecto.migrate actions.

```
login_user@server:~$ sudo su - postgres
postgres@server:~$ createuser -d new_user
```

# Homework Questions?

 - New assignments: Events App

# Continue with PhotoBlog

 - Have the ability to see pictures on show page.
 - Next: Add pictures to feed.

## Access control

Apps usually want to enforce access rules.

 - You want to make sure the app enforces the access
   rules.
 - This must be server-side code, you don't control
   what happens in the browser.
 - Also want to alter the UI to hide actions that
   the user isn't allowed to perform.
 - Doing other in-browser validation can be good too.

Enforcement is server side.

Anything in browser is just for user experience.


## Relations between resources

belongs_to:

```
   Post belongs_to User
   because posts.user_id references users.id
```

has_many:

```
   User has_many Posts
   because posts.user_id references user.id
   and there can be more than one such post
```

has_one:

```
   User has_one FavoriteColor
   because favorite_colors.user_id references user.id
   and there cannot be more than one such favorite color 
```

```many_to_many```: Check the docs




