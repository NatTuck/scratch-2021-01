

# CS4550: Web Dev

Instructor: Nat Tuck

Course site: https://ntuck-neu.site/

Go to: https://draw.ironbeard.com/


# What is the Web?

"A Network" - A colletion of devices that allow communication between those
devices.

Wide area network. Biggest wide area network is
the internet. 

What stuff is definitely involved when we talk
about the web?

The Web is:

 - The web is a bunch of webpages that we can 
   access with a browser.
 - The HTTP protocol

# What is web development?

Frequently: Building web sites using "Web Technologies"

More broadly: Software development where some component
talks HTTP.

# Web App Example

Google Search

 - Google.com
   - Browser: Simple HTML & CSS, Way too much JS
     so they can spy on you better.
   - Web Server: C++ that does RPC to other
     services.
   - Simple version: Huge cluster of search
     workers to do Map-Reduce over a copy of the
     web in a GFS data store.
  - GoogleBot
   - A web crawling bot
   - Is an HTTP client
   - Makes HTTP requests, parses web pages,
     stores them in the GFS data store.
  - Index / Data Store
   - Big custom database and/or filesystem.
   - A bunch of programs maintain this data and indexes
   - This is still part of the "web app" that is
     google search.
 
Web development:

 - Software development in general.
 - Constraint: Our application is always
   a distributed system.

Perfect world prereqs for this class:

 - Networks and Distributed Systems
 - Network security
 - Parallel & distributed programming
 - Databases
 - Systems Administration

# Inkfish

Log in with your Khoury account.

http://my.khoury.northeastern.edu/

Request access now. I could take forever to press the approve button.


# HTTP

1996: HTTP 1.0












   
  
 
 










