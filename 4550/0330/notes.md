
# Final Project

 - You're building an app as a group.
 - You're producing a video presentation,
   to be posted publicly, as a group.
 - Last two lectures will be presentation
   Q&A.
   - There's ~20 teams.
   - We'll need to watch 10 presentations
     before each Q&A meeting - so about
     two hours of them.
   - This overlaps HW10, which should be
     pretty easy.

Upcoming schedule:

This week:
 
  - Security, 2D canvas
  - Web Assembly, 3D canvas

Next week:

  - Guest Lecture on Privacy Policies (= HW10)
  - Distributed Elixir

Week of Apr 12th:

  - Semester Wrap-Up
  - Presentation Q&A (Required)

Week of Apr 17th:

  - Presentation Q&A (Required)

That's the semester.

# Redo Assignment

 - There's a redo assignment, due at the end
   of the last week of classes.

# Project Questions

How do we use websockets in a create-react-app 
app?

https://www.npmjs.com/package/phoenix

Set up your app based on socket.js from an
existing Phoenix app.

You're not using the assets folder of the
Phoenix app, instead you're building the same
stuff that Phoenix gives you by default in 
socket.js

# Topic 1: HTTPS

What?

 - HTTPS is the HTTP protocol encapsulated
   in TLS.
 - TLS provides cryptographic encryption and 
   authentication for TCP connections. 

Why?

This gives us two things:

 - Server authentication
   - Your browser can validate that the remote
     server isn't being spoofed by some unexpected
     party, to some extent.
   - This matters a lot.
 - Resistence to snooping - preventing someone
   listening in from seeing what the contents of
   the messages being sent to and from the server.
   - Encryption is less important.

The goal of this is to make consumers feel safe
sending credit card info.

 - You don't want to send your credit card to the
   wrong people.
 - You don't want a third party snooping to be
   able to see your credit card info.

Verifying companies ("Certificate Authorities")
are approved by the browser vendor.

 - To provide a spoofed certificate, you need to
   either go through the CA approval process or
   you need to co-opt an existing CA.
   - Small criminal organizaitons aren't doing this.
   - Governments can do this pretty easy.
   - Occasionally large companies have been able to
     buy network appliances that auto-spoof certificates
     for deep packet inspection.
     - When this is publicized, the browsers tend
       to kick out the involved CAs.

Problems with using this for more critical security
than credit cards:

 - The client isn't generally authenticated.
   - Browsers support cryptographic client authentication,
     but it's rarely done.
 - Governments and large organizations can bypass
   this, so it's not providing hard security.

Disadvantages to HTTPS:

 - It breaks caching.
 - It makes it so that publishing a website 
   requires permission from one of a small
   list of authorities.

To set up HTTPS:

 - Go to letsencrypt.org
 - Follow instructions

## Topic 2: The Canvas API

Most web sites:

 - Text and images structured with HTML
   - Tends to be rectangles.
 - Styled with CSS

For some websites we want a bit more control
over exactly what is rendered.

 - Typical example: In-browser video game.

For this sort of thing, we have the ```<canvas>```
HTML tag.

This gives us two modes:

 - 2D drawing
 - 3D drawing (webgl)

## Topic 3: Web App Security

### Big Picture

Your application is on the public internet.

 - Whenever you have a machine connected to a
   public network, you're exposing an API to
   the world.
 - Anyone can send you arbitrary IP packets.
 - It is up to you to make sure that sending you
   IP packets only lets people do stuff you want
   them to be able to do.
 - For example, if you expose a /delete_db route
   that deletes your DB, your DB will be deleted.

Law: In the US, the CFAA makes some network 
requests potentially illegal.

 - If you're running a server, this doesn't
   really matter to you.
 - Not only can people ignore the law, people
   exist in countries with different laws.
 - It's best to assume that the internet is
   an automated machine that sends you whatever
   requests will be least convenient for you.

### Web App Security: First Steps
 
 - Your server should be secure outside the
   web app.
 - You should know what your routing infrastructure
   (e.g. nginx) does with all requests. 
 - If your server has multiple externally accessible
   services, make sure they're all secure.

Security is, to some extent, binary. Your system is
either secure or not.

## Potential Attackers?

### Random people, not registered for accounts in our app.

What can they do?

 - Make non-HTTP requests to our server.
 - Make arbitrary HTTP requests to paths.
 - Successfully make HTTP requests to paths
   that don't require some sort of session
   validation.

What should they be able to do?

 - Visit the landing page and other public pages.
 - Register for an account.

What shouldn't they be able to do?

 - Access or modify information that should be
   restricted to logged-in users.
 - Do *anything* that we didn't intentionally design
   as a non-authenticated action.

## Registered users

What can they do?

 - Successfully access stuff that requires a 
   session.

What should they be able to do?

 - Stuff we wanted them to do.

What should they not be able to do?

 - Stuff that only admin can do.
 - Stuff that nobody should be able to do.

## Users with special roles

Examples: Mods, Admins

... same questions again.


## Developers / System Admin / Ops

What can they do?

 - Probably, whatever they want.
 - Including deploying a new modified version of
   the app that potentially enforces new rules.
    
What should they be able to do?

 - Any task nessisary to maintain the deployed app.
 
What shouldn't they be able to do?

 - It's possible to come up with app designs where
   the intent is that developers don't have access
   to some user data.
 - For example, WhatsApp advertiz(ed?) end-to-end
   encryption.
 - Actually building an app that restricts developer
   actoins in the face of developers being able to
   push updates with modified versions is very
   difficult.

## Core idea: Your app should enforce its own rules

In an app structured like a Phoenix app, this
is generally something we think about per-route.

For each route, what are the access control rules and
are they being correctly enforced?

In Phoenix, plugs are nice for this.

These restrictoins *must* be enforced by server-side
code. Rules enforced in the browser are just UX.
