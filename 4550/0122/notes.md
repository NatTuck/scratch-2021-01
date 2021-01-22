

# HW02 is up

# Web Technology

The basic thing that we do on the web is
serve web pages over HTTP.

 - How does HTTP work?
 - What can go on a web page?

## HTTP

HTTP 1.0 (1996)

 - This protocol was entirely stateless.
 - The protocol only supports one site per server.
   - Or at least one site per IP.
 - Two methods: GET, POST
 - POST submits a web form 

HTTP 1.1 (1998)

 - New methods: PUT, DELETE, PATCH, HEAD
 - Added Host header for "virtual hosting"
   - Allows multiple sites (host names) per
     server or IP address.
 - Keep alive for multiple reqs per connection
 - Cookies
   - This allows us to have persistent sessions 
     with web sites.
   - Server sends "Set-cookie: zzz" header.
   - Future requests from client to the same site send
     back a "Cookie: zzz" header.
   - The "same site" means a (host, port, protocol)
     triple.
   - This is sufficient to allow logging in
     to websites.
   - Big complaints about privacy - this mostly has
     to do with "third party cookies".

```
GET /zoo/monkey.html HTTP/1.1
Host: foo.com
=> http://foo.com/zoo/monkey.html

GET /cars/honda HTTP/1.1
Host: ebay.com
=> http://ebay.com/cars/honda
```

```
<!-- your web page -->
<img src="https://spying.facebook.com/like-button.png">
```


## SSL / TLS

This is a layer of encryption / authentication on
top of TCP. This is "https".

http://foo.com
https://bar.com

SSL1: Couldn't handle multiple sites per IP.

All of the "SSL" are insecure.

TSL2.1: Current version, probably secure if
configured correctly. 

The important property here is *authentication*, not
encryption.

## HTTP/2.0

 - Basically just HTTP/1.1
 - Binary protocol instead of text, so I can't type
   it manually.
 - It allows multiple requests concurrently.
 - Browser implementations require TLS.

## WebSockets

 - Is basically TCP over HTTP.
 - This prevents jerks from blocking our protocols
   even if they don't know what they are.
 - This can be used by in-browser programs.
 - This allows long-duration bidirectional 
   communication between servers and browser apps.
 - Specifically, servers can send messages to browser
   apps at any time.

# Web Sites

A web site is a collection of web pages.

# Web Pages

A web page is an HTML document and associated
embedded assets.

See http://caniuse.com/

 - Executable Code: JS (and others, finally)
 - Style Information: CSS
 - Images: JPG, GIF, PNG, SVG, WebP 
 - Video/Audio: H.264, WebM, MP3, WAV, MIDI, FLAC

## Browsers

 - Conceptually, the web is standardized, web pages and
   browsers are written to the standards, and therefore
   you can use any browser.
 - Historically it's never been quite like that, but
   2014-2019 or so was really good.
 - Chrome is almost entirely dominant today. Other browsers
   exist based on the Chrome rendering engine.
   - Chromium-based: Safari, Edge, Brave, etc...
   - The only exception: Firefox (<20% market share)
   - That means designers target Chrome's engine.
 - The standards are pushed forward to match whatever
   Chrome does. Other browsers are unlikely to keep up.
   - We saw Microsoft drop their new Edge engine recently.
 - Other browsers are different enough from Chrome that
   they don't support some websites.

## JavaScript standards

 - ECMAScript 5.1 from 2011 was the last "stable" version.
 - Since 6.0 there's been a new version every year or so,
   tracking Chrome/Firefox.

## CSS/HTML Standards

 - CSS3, HTML5 were the last stable standards.
 - Since then new features have been added without clear
   new version numbers tracking Chrome/Firefox.

# Web Page Structure

HTML2 document (~1996):

XML requires closing tags, SGML does not.

```
<html>
  <head>
    <title>Welcome to HTML</title>
  </head>
  <body>
    <h1>Welcome to HTML</h1>
    <p>This is a <i>very</i> simple web page.
    <p>Paragraph 2.
  </body>
</html>
```

XHTML4 was based on XML.

Current version is HTML5, which is its own thing, not XML or SGML.




