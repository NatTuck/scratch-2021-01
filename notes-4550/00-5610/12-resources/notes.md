---
layout: default
---

## Boring Apps

Project 1 has:

 - Multiple simultaneous users
 - Dynamic view updates
 - Realtime interaction
 - Ephemeral server-side state - all data is lost if the server reboots.

I think apps with those properties are especially interesting, but not all apps
need that sort of functionality.

Consider a photo sharing site:

 - Users upload photos.
 - Other users see the photos when they visit the site later.
 - Nothing needs to update dynamically. When a new photo is uploaded or a new
   comment is posted it's fine if other users don't see it until they reload the
   page.
 - State wants to be persistent. We don't want to lose photos if the server
   reboots.

If we generalize enough, there's a whole class of apps that look basically like
this:

 - We have some important data.
 - The app consists of looking at and modifying that data.
 
Other examples of this sort of app:

 - An online store
 - Most social media sites, at least before ~2015.
 - Control panels for online services
 - Online banking
 - Customer service ticket systems

Our data is important and we'll be accessing it a lot, so we need a reliable way
to store it with decent performance. Luckily, there's a standard solution: A
Relational Database Management System, aka SQL Database.

 - Our data comes as a set of well defined types.
   - e.g. A user has a name, an email address, and some other fixed fields.
 - There are defined relationships between our data.
   - e.g. A comment is posted on a photo by a user.
 - We want to be able to validate our data and the relationships.
   - Otherwise we end up with corrupted data, which is a serious problem.
 - We want to be able to query our data based on the relationships.
   - e.g. Get all comments for a photo.

Photo sharing has one edge case that's different from most apps: Some of the
data we're storing is pretty big. Relational databases *can* store big chunks of
data, but they're not the best tool for the job. Instead, we'll store the photos
themselves as files on the filesystem.

A relational database stores data as a collection of tables.

 - Each table stores records of one type with an associated schema.
   - So each record in a table is the same type of data, e.g. a user.
 - Each table has a mandatory field, a unique numeric ID called "id".
 - A table can have fields which reference records in other tables,
   this is achieved by storing the id of a field in another table.

We can treat our photo sharing site as a database (& our photo files) plus a
user interface that allows us to do CRUD operations: Create, Retreive, Update,
Delete.

What are our tables?

 - users: name, email
 - photos: user_id, filename
 - ... more later

## Creating our new app

 - Git repo: http://github.com/NatTuck/lens
 - This section finishes with: 0-setup

First, we need to create the new app and
configure the DB:

```
 $ mix phx.new lens
 $ pwgen 12 1
 Tee1atuaRufa
```

Edit the config file in config/dev.exs

```
config :lens, Lens.Repo,
  username: "lens",
  password: "Tee1atuaRufa",
  database: "lens_dev",
  ...
```

Also in config/test.exs

```
config :lens, Lens.Repo,
  username: "lens",
  password: "Tee1atuaRufa",
  database: "lens_test",
  ...
```

Next, we need to setup the database:

```
 $ sudo su - postgres
 $ createuser -d lens
 $ psql
 psql=# alter user lens with password 'Tee1atuaRufa';
 psql=# exit
 $ exit
 you@host:lens$ 
```

And then create the DB for dev mode:

```
 $ mix ecto.create
```

Now we should be able to run our app in either test or dev mode:

```
 $ mix test
 $ mix phx.server
```

## Replace the defaults with Bootstrap 4

Milligram is fine, but let's use something fancier for our photo sharing app.

First, we need to remove the defaults.

```
$ cd assets
$ rm static/images/phoenix.png css/phoenix.css
```

Install Bootstrap and deps.

```
$ npm install --save bootstrap jquery popper.js
$ npm install --save-dev node-sass sass-loader
$ npm install --save-dev node-gyp
$ npm install --save-dev fibers
$ npm install --save-dev webpack@^4.36.0
$ npm audit fix --force
```

assets/webpack.config.js

```
    // at the top
    const webpack = require('webpack');
    
    // module: rules: 
    {
      test: /\.scss$/,
      use: [MiniCssExtractPlugin.loader, 'css-loader', 'sass-loader']
    },
    
    // plugins: [
    //   ...
    new webpack.ProvidePlugin({
      $: 'jquery',
      jQuery: 'jquery',
      Popper: ['popper.js', 'default'],
    }),
```

 * move css/app.css to css/app.scss

in css/app.scss:

```
$body-bg: #BBB; // With scss, Bootstrap lets us override colors and stuff.
@import "~bootstrap/scss/bootstrap";
```

in js/app.js:

```
import css from "../css/app.scss";
```

in .../templates/layout/app.html.eex:

```
...
  <body>
    <div class="container">
      <nav class="navbar navbar-expand-sm navbar-light bg-white">
        <div class="col-4">
          <a class="navbar-brand" href="#">Lens</a>
        </div>
        <div class="col-8">
          <ul class="navbar-nav mr-auto">
            <li class="nav-item active">
              <a class="nav-link" href="#">All Photos</a>
            </li>
            <li class="nav-item">
              <a class="nav-link" href="#">Your Photos</a>
            </li>
          </ul>
        </div>
      </nav>

      <div class="row">
        <div class="col-12">
          <%= if get_flash(@conn, :info) do %>
            <p class="alert alert-info" role="alert"><%= get_flash(@conn, :info) %></p>
          <% end %>
          <%= if get_flash(@conn, :error) do %>
            <p class="alert alert-danger" role="alert">
              <%= get_flash(@conn, :error) %>
            </p>
          <% end %>
        </div>
      </div>

      <%= render @view_module, @view_template, assigns %>
    </div>

    <script ...
``` 

in .../templates/page/index.html.eex:

```
<div class="row">
  <div class="col-12">
    <h1>Welcome to the Lens Photo Sharing App</h1>
  </div>
</div>
```

### Prep for Deployment

```
$ mix release.init
```

Make sure to enable server:

In config/prod.secret.exs, scroll to bottom;

```
config :lens, LensWeb.Endpoint, server: true
```

Set hostname:

In config/prod.exs, update hostname:

```
config :lens, LensWeb.Endpoint,
  url: [host: "lens.ironbeard.com", port: 80],
  cache_static_manifest: "priv/static/cache_manifest.json"
```

Branch 0-setup ends here.

## Deploying our App

Setting up for deployment is a good thing to do early. We're going to do it
right away.

### Do a test deployment.

Important note: Passwords / secrets shown here are *examples*. Generate your own
secrets.

 - Create a new user on cs5610.ironbeard.com
 - Clone the git repo and checkout 0-setup
 - Produces branch: 1-deploys

Fetch deps:

```
$ mix deps.get
$ (cd assets && npm install)
```

Set up postgres user.

```
  $ sudo su - postgres
pg$ createuser -d lens
pg$ pwgen 12 1
some\_pass
pg$ psql
psql=# alter user lens with password 'some\_pass';
```

 - Worry about secrets.
 - There should *never* be secrets in your git repo.
 
Create an file of production env vars:

```
 $ vim .gitignore
     (add prod-env.sh)
 $ mix phx.gen.secret
some_long_secret
```

prod-env.sh:

```
#!/bin/sh

export MIX_ENV=prod
export PORT=5001
export DATABASE_URL="ecto://lens:some_pass@localhost/lens_prod"
export SECRET_KEY_BASE="some_long_secret"
```

Create the production DB:

```
$ . prod-env.sh
$ mix ecto.create
```

Create a deploy script, deploy.sh:

```
#!/bin/bash

export NODEBIN=`pwd`/assets/node_modules/.bin
export PATH="$PATH:$NODEBIN"

source ./prod-env.sh

# Build Elixir Code
mix deps.get
mix compile

# Build Assets
mkdir -p priv/static
(cd assets && npm install)
(cd assets && webpack --mode production)
mix phx.digest

# Migrate DB
mix ecto.migrate

# Generate the release
mix release
```

Create a start script, start.sh:

```
#!/bin/bash

source ./prod-env.sh

_build/prod/rel/lens/bin/lens start
```

Create a nginx config file:

../sites-available/lens.ironbeard.com:

```
upstream lens_app {
    server localhost:5001;
}

server {
    listen 80;
    listen [::]:80;

    server_name lens.ironbeard.com;

    location / {
        proxy_pass http://lens_app;
    }

    location /socket {
        proxy_pass http://lens_app;
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection "upgrade";
    }
}
```

Enable it:

```
# cd /etc/nginx/sites-enabled
# ln -s /etc/nginx/sites-available/lens.ironbeard.com .
# service nginx restart
```

Create systemd config:

```
[Unit]
Description=Lens Photo Sharing

[Service]
Type=simple
User=lens
Group=lens
Restart=on-failure
Environment=MIX_ENV=prod "PORT=5001"
Environment=LANG=en_US.UTF-8

WorkingDirectory=/home/lens/lens
ExecStart=/bin/bash /home/lens/lens/start.sh

[Install]
WantedBy=multi-user.target
```

Enable and start it:

```
# cp lens.service /etc/systemd/system
# systemctl enable lens.service
# service lens start
# service lens status
```

## Our First Resource

 * Branch: 2-users


As we discussed before, we can treat our photo sharing site as a database (& our
photo files) plus a user interface that allows us to do CRUD operations: Create,
Retreive, Update, Delete.

We use the term "resource" to describe something that has those operations,
commonly stored as a table in the database.

Phoenix gives us a code generation tool to create scaffolding for a resource.

For now, users just have an email and a name. They'll also need passwords, but
that gets most of a lecture since it takes some effort to do passwords right.
Better to just leave it out for now.

```
$ mix phx.gen.html Users User users email:string name:string
```

This generated:

 * A controller
 * Templates for four pages.
 * A context module.
 * A database migration.
 * A schema module
 * Tests

Let's look at the database migration. Both fields are required, so we should add
"null: false" to allow the database to enforce that for us.

We also need to add the suggested line to the router.

```
$ mix phx.routes
```

The "resources" line added seven routes to our application for the new resource,
four routes for the CRUD operations, two routes for forms, and an index route to
list our resource. These routes are sometimes called "RESTful" routes.

Run the migration:

```
$ mix ecto.migrate
```

Run the app and pull up the /users path. Play with the scaffold UI.

## Cleaning Up the Scaffold With Bootstrap

Visit all five templates and fix them.

Tables:

```
  <table class="table table-striped">
```

Forms:

```
  <div class="form-group">
    <%= label f, :email %>
    <%= text_input f, :email, class: "form-control" %>
    <%= error_tag f, :email %>
  </div>
```

Buttons:

```
    <%= submit "Save", class: "btn btn-primary" %>
```


### Note to Self: Plan for Business App Lectures

In class: Photo Hosting

 - Photos
 - Users
   - A user has many photos
 - Follows
   - A user has many followers (users) through follows
   - A user has many favorites (users) through follows

