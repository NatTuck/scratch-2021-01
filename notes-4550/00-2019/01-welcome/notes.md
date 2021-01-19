+++
title = "./01-welcome/notes.md"
+++

# 5610/4550 Web Development

## draw.ironbeard.com

 - That's a web application
 - This is the sort of thing that we'll be building
 - [If stuck: w.correct_word]

## The Web

What is the web?

 - Protocols: HTTP, URLs
 - Content Standards: HTML / CSS / JS
 - Data Conventions: JSON, REST
   
What is web development?

Key Element: HTTP

(Note: You might say that a browser was the key element. I'd claim that
laving a program on a server contact a remote HTTP API is still web dev.)


## Web Devlopment

Frequently: Building web sites using "Web Technologies".

More broadly: Software development where some component talks HTTP

Example: Google Search

 - Google.com
   - Browser: Simple HTML & CSS; A touch of JavaScript
   - Web Server: C++ that does RPCs to other services.
   - Simple version: Huge cluster of search workers do Map-Reduce 
     over a copy of the web in a GFS data store.
 - GoogleBot
   - A web crawling bot
   - Is a HTTP *client*
   - Needs to *parse* HTML / CSS / some JavaScript
 - Index / Data Stoer
   - Big, custom data base and/or filesystem
   - A bunch of programs that maintain the data and indexes
   - This is still part of the "web app" that is Google Search.

Observations:

 - This is not a narrow area of programming where we can look at
   a few specific things.
 - This is general development with a constrant that specifically makes it
   complex:
   - HTTP is a network communication protocol
   - a web application is, by definition, a distributed system
     - even one browser + one server is two computers that communicate
       over a network
   - building distributed systems is really hard 
     (they tend to be theoretically impossible)
 - Perfect world pre-reqs for this class:
   - Networks + Distributed Systems
   - Network security
   - Parallel programming
   - Databases
   - System Administration
 - How do we deal with not being experts at everything?
   - Be prepared to learn new topics as needed.
   - Take advantage of high quality tools and libraries that
     were written by area experts.
   - Find and learn new tools as needed.

The overall plan:

 - In order to explore web development, we're going to focus on a concrete set of tools.
 - Our main tools will be Phoenix / Elixir / Reactjs / PostgreSQL (/ Nginx / Linux).
 - These tools are used in industry, but were selected to illustrate where I
   think web development is going rather than what's most popular right now.

 - This course requires a lot of programming.
 - This course is designated as "writing intensive in the major". It will require
   significant written English prose.

## The Syllabus

 - Course web site / syllabus: http://khoury.neu.edu/~ntuck & click 
 - Bottlenose / Piazza
 - My lecture notes
 - TA office hours
 - Schedule: 
   - HW or Project Work Every Week, due Thursday
   - First hw due *this Thursday*.
 - Assigments only: No Exams
 - No text book
 - Domain Name & VPS
   - We're doing ops ourselves...
 - Our primary resource for the semester is online documentation.
   - The official documentation is your best source.
   - If you have trouble understanding the official documentation,
     that means you need to practice that skill.
 - A properly configured editor with language support for syntax
   highlighting and indentation is mandatory.
 - Grading: Homework, two projects, no exams.
 - Assignments required: Three zeros is an F.
 - Grade Challenges
 - Plagarism

## Why our own servers?

 1. There are alternatives to administering your own server.
 2. They provide less flexibility than doing it yourself.
 3. Doing it yourself is valuable, and gives you essential information
    to evaluate the other options.
 4. We may use some stuff that doesn't play nice with PaaS providers.

## HW1: Hosting a Static Web Site

First, pull up the assignment on Bottlenose.

### Initial stuff

- Set up a VPS
  - Show screen process on vultr
  - Vultr affiliate links
  - Set an SSH key
  - (you can generate a key with "ssh-keygen -t rsa")
- I've already registered a domain and set up a server.
  - Point it (including www.) to the VPS's IP addr
  
### Set up the server
 
**First, secure the server.**

- Log in to the server (ssh root@ironbeard.com)
- Enable the software firewall

```
ufw allow 22/tcp      # important, this allows ssh
ufw allow 80/tcp      # allow normal HTTP
ufw allow 443/tcp     # allow HTTPS
ufw allow 4000/tcp    # allow the Phoenix dev server
ufw enable
```

Enable automatic upgrades (for security patches):

```
dpkg-reconfigure --priority=low unattended-upgrades
```


Next we need to create a non-root user. When working with a Linux server, the
root account is for administering the *server itself*. We want a non-root
account for working with web sites / applications.
  
 - adduser name
  - Set a good password
 - adduser name sudo
 - from your machine: ssh-copy-id name@ip
  - from your machine: test ssh login in separate terminal window
  - Test sudo: sudo su -
  
 - Disable root ssh (/etc/ssh/sshd_config; PermitRootLogin)
  - restart the ssh service: service ssh restart
  - Verify no root logins & user login still works.
 
**Next, set up the web server.**

As the root user:

    sudo apt install nginx
    cd /etc/nginx/sites-enabled
    vim default

 - Change "root" to /home/name/www/main
 
    service nginx restart

 - (as normal user) Create /home/name/www/main/index.html
 - Visit the server IP

Testing while waiting for DNS:

 - Create an entry in (your local machine's) /etc/hosts for the site.

Let's set up another web site too.

 - (as root) Save off the template below in /etc/nginx/sites-available
 - Edit the root and server name (ironbeard.com)
 - Symlink the config file to /etc/nginx/sites-enabled
   - ln -s /etc/nginx/sites-available/ironbeard.com /etc/nginx/sites-enabled
   - ls -l /etc/nginx/sites-enabled
 - Restart nginx
 - Create hosts entry and test that.

sites-available/ironbeard.com :

```
server {
        listen 80;
        listen [::]:80;

        root /home/nat/www/ironbeard.com;

        index index.html;

        server_name ironbeard.com www.ironbeard.com;

        location / {
                # First attempt to serve request as file, then
                # as directory, then fall back to displaying a 404.
                try_files $uri $uri/ =404;
        }
}
```

Add a swap file, to avoid running out of memory.

As root on your server:

```
# dd if=/dev/zero of=/swap bs=1M count=2048
# chmod 0600 /swap
# mkswap /swap
# vi /etc/fstab
/swap		swap		swap	defaults	0	0
# swapon -a
```

# Web Tech Overview

## Some Historical Web Apps

 - WebCrawler (1994) (C / CGI)
 - UBB message board (perl / CGI)
 - Slashdot (Perl - embed interpreter)
 - phpBB (PHP - embed interpreter)
 - Reddit (LISP then Python - separate server) 
 
 - OpenOffice Online
   - Client Side: JavaScript that doesn't do much.
   - Server Side: C++, including rendering
 - Gmail
   - Client-side: Java
   - Server-side: Java
 - Facebook
   - Client-side: JavaScript and React.js
   - Server-side, main application: PHP
   - Server-side, secondary services: C++, Java, Python, Erlang

## HTTP

### HTTP/1.0 (1996)

HTTP 1.0 was entirely stateless.

 - Two methods: GET, POST
 - No concept of "session".
 - Server doesn't know that two requests come from the same browser.
 - No way to have multiple sites per IP.

Example: (end the GET with two newlines)

```
$ telnet seagull.ccs.neu.edu 80
GET / HTTP/1.0
```

### HTTP/1.1

New methods: PUT, DELETE, PATCH, HEAD

Add Host header to support multiple sites.

Example: (end the GET with two newlines)

```
$ telnet seagull.ccs.neu.edu 80
GET / HTTP/1.1
Host: seagull.ccs.neu.edu

$ telnet seagull.ccs.neu.edu 80
GET / HTTP/1.1
Host: example.com

```

HTTP 1.1 is still stateless, but adds "cookies".

 - Cookies are (name, value) pairs that are sent from the server to the client in
   the HTTP response.
 - The client (browser) is supposed to send back all the cookies with future requests
   to the same site.
 - A "site" is a (host, port) pair.

### SSL / TLS

SSL1: No way to have multiple sites per IP, even with HTTP/1.1

All SSL: Insecure.

TLS2.1: Current version, probably secure if you don't configure
insecure crypto algorithms.

### HTTP/2.0

 - Think of it as a fast version of HTTP/1.1
 - Binary protocol
 - Allows multiple concurrent requests on one connection
 - Browser implementations require TLS
 
### WebSockets

 - Think of it as TCP over HTTP
 - Allows long duration bidirectional communication
 - With an open websocket, the server can send a message to the client
 - Allows real-time behaviors in web apps.

## Browser Content

HTTP just lets clients request URLs from servers.

A URL generally contains a file, possibly generated dynamically.

Some specific types of file are broadly supported by web browsers,
and those make up normal web content.

Users update browsers slowly, or don't realize that they can download
a browser that isn't Internet Explorer. You can see what fancy features
are supported in which browsers at: http://caniuse.com

### HTML

 - Most browsers support HTML3 from the 90's.
 - It's generally safe to use a subset of HTML5.
 - Which subset? No idea. Have fun.

### CSS

 - Most browers support a superset of CSS1.0
 - Specific features from CSS2 or CSS3 may have broad support.

### JavaScript

 - Most browsers support some superset of ECMAScript 3 (ES3) from 1999
 - Your browser supports ECMAScript 5 (ES5) from 2010
 - Browsers are working on support for ES6

JavaScript isn't a terrible language, but many developers prefer working
in other languages. It's common to compile to JavaScript.

 - We'll get to use ES6 compiled to ES5 later.
 - CoffeeScript
 - TypeScript
 - ClojureScript
 - Dart
 - Elm

### Images

 - PNG (Lossless compression, good for "simple" images)
 - JPEG (Lossy compression, good for photos)
 - GIF (256 colors only, but supports animations)

### Other Stuff

 - Video
   - No single format with 100% support. H.264 and WebM come closest.
 - Audio
   - MP3 should work everywhere.
   
## Server-side Programming

Client-side we run JavaScript.

Server side, we just need a program that responds to HTTP requests.

History:

 - CGI (Perl)
 - Web Server Hooks (PHP)
 - Write a Web Server, reverse proxy to it.

Additional concerns:

 - Frameworks / Languages
 - Data persistence (usually a database)

Stacks:

 - LAMP: Linux, Apache, MySQL, PHP
 - .NET: Windows, C#, MSSQL, etc
 - Java: Linux, Whatever server-side Java uses
 - MEAN: Linux, MongoDB, NodeJS - JavaScript everywhere!
 - RoR:  Linux, Ruby, Rails, Posgres/MySQL, a server?

Modern Decision Tree:

 - Microsoft?
 - Java?
 - Pick a framework and DB

Framework / Languages choices:

 - Framework: Heavy vs. Light, Opinionated vs. Flexible
 - Language: Performance
   - Native code: C, C++, Rust
   - Native code + runtime: Go, Haskell
   - Heavy VMs: Java, .NET
   - "Fast" scripting languages: JavaScript, Lua
   - Scripting languages: Perl, Python, Ruby
 - Specific features 

Database choices:

 - SQL vs. NoSQL


## Unused Notes

https://conference.libreoffice.org/2015/the-program/talks/development/
