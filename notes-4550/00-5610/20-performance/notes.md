---
layout: default
---

## First Thing

Project questions?

# Tuning Web Application Performance

We're going to look at application performance from three directions:

 - User Experience: Slow websites are awful.
 - Hosting Cost: Once your application is hardware-constrained, cost optimally
   scales linearly. If you want to have twice as much CPU or RAM, you pay twice
   as much money. Paying less money is usually preferable.
 - Scaling: Once you get beyond what one server can do, you want to 
   scale linearly by adding more servers. This takes effort.

## Performance: User Experience

Slow websites are awful. This has a concrete business impact: studies show that
the "bounce rate", or the number of people who give up on waiting for a website
to load and just close the tab, increases by something like 1% per 100ms of load
time.

For internal apps users will, to some extent, just complain about it and be
slightly less efficient at their jobs. For public sites, a bounce is a potential
lost customer which directly means lost money.

As a general rule, we *want* the delay on user actions to be under 100ms. That
means both:

 - Initial load of the application from clicking a link to the site to the page
   appearing to be fully loaded and responding to clicks.
 - Any response to user action, like clicking an internal link.

What contributes to that delay?

 - Network latency to request the page
   - Use servers near your users
 - Time for server to generate the page
   - Measure then consider optimizing
 - Transmission time for the HTML
   - Measure then consider optimizing
 - Parse time of the HTML document
   - Measure then consider optimizing
 - Network latency to request assets (css, js, images)
   - Use servers near your users
 - Transmission time for assets
   - Measure then consider optimizing
 - Parse time for CSS / JS.
   - Measure then consider optimizing
 - Rendering time
   - Measure then consider optimizing

How do we visualize this:

 - Show Firefox dev tools network tab.
 - Throttle to DSL, disable cache, load Inkfish.
 - Looks like JS load time dominates.
 - Now load northeastern.edu
 - Mostly giant images.
 - Images don't actually block page usage.
 - That's a lot of code for the creepy zoom effect on image hover.

Alternate tools:

 - google 'google pagespeed'
 - Try that on inkfish.ccs.neu.edu
 - Same answer, but gives two concrete recommendations:
 - Rec 1: Enable text compression for the JS.
   - That's pretty easy. There's probabably a flag on the
     static asset thing in Phoenix or we can make nginx do it.
 - Rec 2: Defer the 95% of the CSS that isn't used on the
   landing page.
   - This sounds annoying, but would be worthwhile if performance
     mattered more.

Even with those changes, it looks like we're not going to reliably get 100ms on
first page load with nothing cached. But we still want to keep it under a few
seconds.

Following the standard structure for a modern application using an up-front
JavaScript bundle, our first page load is going to be dominated by downloading
assets. That means:

 - We want to keep those assets reasonably small. Targeting DSL speeds, that
   means at most a megabyte or two. For slow cellular connections, we're limited
   to maybe a few hundred KB.
 - Be careful with images and videos. Either having big ones or a lot of them
   will slow things down.

For subsequent page loads, JavaScript and CSS are already cached, so other
considerations matter more.

 - For a traditional app like Inkfish, the time for the server to generate
   the page matters a lot.
 - Phoenix is pretty fast - usually bottlnecked by the database - but it's still
   possible to write slow code.
 - You can see page generation times in your log output.
 
We still have the same specific targets:

 * We want a response to user actions within 100ms.
 * If we can't hit 100ms, we at least want to hit 500ms.
 * If it's going to take longer than that, we probably want to
   show a progress indicator.
 * First page load can probably get away with taking as much as three seconds,
   but even then we'd prefer to show a progress indicator within 500ms.

Let's look at a slow example:

 - Pull up the production Inkfish logs.
 - Load the grading page for my HW03.
 - Most page loads take under 10ms of server time, but this one takes 300+ ms.
 - Why? Reading several dozen files from disk.
 - If this gets any slower, I'll want to make the file loading asynchronous
   and add a loading bar.

### How to optimize programs

Premature optimization causes problems. Try to write your programs in the
simplest way that works and isn't obviously bad. Only move to more complicated
solutions for performance when it's obviously nessisary.

 - When you're writing the program, try not to do anything that's obviously
   going to be really slow / waste resources. Pick reasonable algorithms.
 - Then wait for it to actually use more resources (e.g. time, RAM, disk space,
   network bandwidth) than you want.
 - Once you know it's inefficient, measure to find out *what* is inefficient.
 - If your code is slow, *profile* it to find out which part of the
   code is actually slow.
 - Once you know the problem, try to solve it by optimizing that component.
 - Finally, measure to make sure your optimization actually improves what
   you were trying to fix.

### CDNs

One of the components of load time that's especially hard to deal with is
network latency. Due to the speed of light, the *only* way to reduce this is to
have servers physically closer to the user.

For high traffic sites, data bandwidth costs can become an issue too. It can be
possible to get cheaper prices if you send data directly to the target network.
But only a few really crazy companies (e.g. Google, Netflix) can afford to
straight up add servers to every network.

An important observation is that data for most sites can be separated into two
categories:

 - Static data (e.g. images, js, css): Big, rarely changes
 - Dynamic data (e.g. HTML): Tends to be generated by the app for each request

The dynamic data tends to show up in one request, while the remainder of the
data tends to be static assets. Therefore, we can get much of the benefit of
using servers closer to the user my moving just the static assets closer to the
user.

Content Delivery Networks provide the service of hosting static content at a
variety of data centers all over the world, with routing set up so that requests
will go to the data center closest to the user. This can be an effective way to
optimize load times for some web sites.

Some CDNs provide a service where they host commonly used open source JS and CSS
files at no charge. This is frequently the primary method documented for using
those libraries, since it allows the communities to avoid technical support
issues from people having trouble with their hosting setup.

CDN providers will also provide proxy service, where you don't need to
explicitly seperate out your dynamic and static content. They'll figure out
what's static automatically (if your app is sending proper headers) and cache
it.

If a CDN is already proxying your site, it can also provide protection against
bandwidth-based denial of service attacks, including DDOS.

### Denial of Service

(didn't get to this section last time)

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
you'll want to get a DDOS protection service from some network provider (such as
a CDN).

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


## Performance: Hosting Costs

For this class you've been working on a $5/month virtual private server.

That's worked fine for hosting 6 assignment applications and a game project
simultaneously, with almost no users.

The same server setup should keep working fine even for significant real-world
produciton apps. With a little bit of attention to efficiency, you should easily
be able to host a complex app that handles thousands of users - something like
my.northeastern.edu or the Northeastern Blackboard instance, for example.

But at some point a fraction of a CPU core and a gig of RAM isn't enough. When
that happens, it's pretty easy to scale up.

 - Vultr VPSes max out at 16 CPUs + 64GB for $320/month.
 - Next step up is dedicated servers. Prices are similar per resource,
   but the brackets start higher.
 - Next step up is getting a custom server and colocating it. You can easily get
   a 128-core server with 1TB of RAM for \$30k - or \$1000/month - and
   colocation fees for a single server are a couple hundred bucks a month.
 - You can go a little bigger on a single server - up to ~500 cores and 50TB of
   RAM, but that starts to get expensive - maybe half a million dollars.

So things scale smoothly up from the cheap VPS by a factor of 100 or so. If you
can handle 1000 users on the VPS, you can get a server that will handle 10,000
users for 100x the price.

But beyond that, single server scaling gets expensive. If you go all in, you
might get another factor of 10 performance for 100 times the price.

That leaves two choices:

 - Optimize your app.
 - Add more servers and distribute the work.

This is an interesting tradeoff. 

 - Developers cost on the order of \$100k/year, so an extra developer costs
   about the same as 10 high-end commodity servers.
 - Once you have more than 10 servers, spending a developer-month on a 10%
   optimization is a good deal.

This produces a predictable outcome when it comes to tool choices. Startup
companies tend to focus on tools that allow rapid application development at the
cost of performance - Ruby on Rails is my favorite example here. This makes
sense - performance only matters when you run out of it and computers are pretty
fast.

Very large companies tend to rewrite their applications with techniques that
take a lot of developer time for better performance - custom C++ apps are the
extreme case, that's how Google does it. With tens of thousands of servers,
doubling developer time isn't a problem for them and a 1% performance gain
can save them hundreds of servers.

## Scaling: Multiple Servers

Web applications are innately distributed systems, but moving to multiple
servers makes this even more true.

### Stateless Webservers

Traditional web application archetecture:

 - One DB server (or two for redundancy)
 - N web servers.

Traditionally we want our webserver processes to be stateless. If the only side
effects or communication they do is to the DB server, then we can scale pretty
much transparently by just adding more of them, and we can load balance between
them arbitrarily (e.g. sending each request to a random server works fine).

The bottleneck in this kind of scaling is the DB server. Relational database
systems work best on a single server. There are some mechanisms that allow
multiple servers - such as read-only replicas and live mirroring, but those only
give a small constant factor improvement. Outside of specific niche tools, you
generally can't have 10 database servers working together to provide the same
behavior as 1 server at 10x the transaction throughput.

Luckily, you can scale pretty far with one DB server.

If you really need scalable persistent data storage - this is a problem that
Google and Amazon have and not many others - then it's worth looking into the
whole variety of options that are NoSQL datastores. We'll probably have a
lecture on that later in the semester.

### Adding Non-DB State

This structure runs into trouble when we have communication between server
processes outside the database or if we have state outside the database. We can
deal with that by adding additional communication and state servers.

 - One DB server
 - One message queue server (e.g. rabbitmq)
 - One transient state server (e.g. redis)
 - N web servers

This lets us build applications like Project 1 where we have state outside the
database and direct communication between multiple users.

Unfortunately, this makes the message queue and transient state servers
additional bottlenecks. These scale better than the DB to multiple servers, so
you can add more.

One of the big advantages of the Erlang platform is that solutions to the
inter-server communication and transient state problems are built in. Erlang
processes communicate by message passing, and message passing works great across
different physical servers. So the basic strategies we used for Project 1 like
GenServers would only need slight changes for a cluster.

As the application gets more complicated, the single layer archetecture can be
extended by adding an application layer:

 - One DB server
 - N application servers
 - M web servers

The application servers may be split up by type (e.g. search, game servers,
message queues, internal caches, whatever).

This split is kind of like the Web vs. Non-Web directories under lib in a
Phoenix application. Requests come in to the web servers, application actions
are forwarded to the app servers, and the app servers do DB queries. Responses
progress in the other direction. In this model, inter-server communication tends
to happen at the application layer.

Case Study: Stack Overflow

 - Requests to stack overflow come in to one of two load balancers.
 - These forward requests on to one of 11 front-end web servers running a
   C# code on IIS/Windows.
 - These construct the response, doing requests to four differnet kinds
   of support servers during the process.
   - Search requests go to one of three ElasticSearch servers.
   - Cache requests and inter-node communication goes to one of two Redis
     servers.
   - Tags are managed by a cluster of three servers.
   - Database requests go to a single MSSQL server (a second replica server
     available for failover).

Load up a StackOverflow page, search for "perl segfault".

 - The webserver got the page and concurrently dispatched requests to:
   - An ElasticSearch server for the search results.
   - The Redis cache for the "Hot Network Questions" sidebar.
 - Once the search results came back, it issued concurrent requests to:
   - The database for the first 15 search results.
   - A tag server for the tags associated with those 15 search results.
 - Once those results came back, the webserver rendered the page and sent
   the response.

In a smaller scale application this all could have been done with database
queries, but by adding the extra servers they reduced the load on their database
to one simple query (to get the search result records) for the request instead
of a simple query (for the sidebar) and a complex query (for the search results
and tags).

The local network requests to multiple servers increases the time for a single
response, but decreases the time for many concurrent responses by spreading the
load across several servers.

Most of the servers are for redundency and to make performance analysis easier.
For example, they claim the CPU usage on their Redis servers is below 2%, and
that the site actually runs fine with only one of the webservers up.
Additionally, many of the resources are also shared with other StackExchange
sites.

So they could probably run Stack Overflow fine on 6 servers:

 - One load balancer.
 - Two webservers.
 - One ElasticSearch server.
 - One Redis + Tags server.
 - One MSSQL DB server.

Why Microsoft? Jeff Atwood likes Microsoft, so they use it. This could equally
have been Java + Postgres, node.js + MySQL, or the stack we've been using. Ruby,
Python, or PHP might require twice the web servers - but maybe not because this
setup is probably I/O limited rather than CPU/RAM limited.

If this were a custom C++ or Rust (maybe Go, OCaml, or Haskell) app they could
probabably host the whole thing on one server. They don't because buying and
hosting a couple racks of servers isn't a problem.

## Multi-Homing

StackOverflow's basic server archetecture fits in a single rack in a datacenter
in New Jersey, but they have two backup racks: One right next to the first, and
a second one in Colorado.

The primary reason for this sort of replication is reliability. 

 * If someone trips over the power cord to rack 1, the service is still up. It
   fails over to rack 2.
 * If a meteor hits the datacenter in New Jersey, the service is still up. It
   fails over to Colorado.
 * For StackOverflow, that's probably good enough. They don't need to stay
   online in the face of two meteors - although they probably also have offsite
   backups to a third location so even with two meteors they can come back
   online as soon as they buy more servers.

But for bigger or more performance critical applications, multi-homing can also
be used for performance.

 * ping stanford.edu
 * ping google.com

The google.com server we're hitting isn't in California - it's in New York - but
if we were at Stanford it would be.

For applications like Google Search and Amazon.com, it's worth having local
replicas for performance. This isn't just local caching, they need to be able to
service complete requests from multiple data centers.

This produces a problem: they can no longer have a single authorative version of
their application state. Straightforward use of SQL databases no longer works.
Here's a simple example to illustrate the problem:

 - You're logged in to Amazon here in Boston.
 - You gave a friend your Amazon password, and they're logged in from Sydney,
   Australia.
 - You submit the form to set a default shipping address.
 - Within 50ms, your friend does the same.
 - Amazon's servers are fast and distributed to be faster. We'll say the
   servers are also in Boston and Sydney.
 - You each get a response ("success") within 50ms of submitting the form.
 - Network latency from Sydney to Boston is ~200ms.
 - What's your default shipping address now?

We'll talk about this problem and how it can be handled more next week, but for
now I'll just say it's a hard problem with no perfect answer.

Solving the problem is hard, but there are some good ways to avoid it:

 * Plan A: Don't do replicas for performance.
   - Your app probably isn't acutally used globally, so put the servers
     near the users.
 * Plan B: Have your replicas be independent.
   - Game servers frequently do this. If someone from the US wants to play with
     someone from Australia, they need to pick the US server or the Austrlia
     server and one of them is going to get a ton of lag.

## Links

 - https://developers.google.com/speed/pagespeed/insights/
 - https://addons.mozilla.org/en-US/firefox/addon/load-time/
 - https://nickcraver.com/blog/2016/02/17/stack-overflow-the-architecture-2016-edition/

