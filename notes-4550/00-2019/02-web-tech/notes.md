+++
title = "./02-web-tech/notes.md"
+++

## Homework Questions

 - HW01 questions?
 - HW02 is up.
 - Show the assignment.
 - Show the solution running.

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
are supported in which browsers at: http://caniuse.com/

### Browsers

  - Best: Firefox / Chrome - TAs will use these to grade work in this class.
  - OK: Edge and Safari
  - Bad: Internet Explorer 10/11
  - Awful: IE7

### HTML and CSS

 - These used to be versioned, but after HTML5 and CSS3, the standards body
   has been adding features without a clear version bump.
 - New features won't work in old browsers.

### JavaScript

 - EMCAScript 5.1 from 2011 is widely supported.
 - From 2016, they're releasing a new revision every year.
 
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


## Web Page Structure

### HTML2 document:

```
<html>
  <head>
    <title>Welcome to HTML</title>
  </head>
  <body>
    <p>This is a <i>very</i> simple web page.
  </body>
</html>
```

 - HTML is a format that allows us to describe a structured document.
 - Encodes a tree of elements.
 - Each element is encoded by a tag. Different tags have different semantic meanings.
 - HTML alone doesn't fully specify how a document is rendered - rendering
   can be altered by CSS.
 - For rendering, we can think of each element as a rectangular portion of
   the web page. (Pull up dev tools, highlight the "p" vs the "i" tags.

Broad bucketing of tags:

 - Non-displayed elements (metadata, broad structure): html, head, title, body
 - Block elements: p   (Text goes in a block)
 - Inline elements: i  (Inline elements go in text and mark up parts of it)
 
### HTML5 document:

page.html:

```
<!doctype html>
<!-- simple doctype selects HTML 5 -->
<html lang="en">
  <!--
       The lang attribute is important for font selection, especially
       for non-European languages. 
  -->
  <head>
    <meta charset="utf-8">
    <!-- First line of head is meta charset="utf-8". This prevents annoying bugs later. -->

    <title>Hello, Web</title>

    <link rel="stylesheet" href="style.css">
    <!--
         external styles go in the head
         styles should be external by default, leaving HTML for structure.
    -->
  </head>
  <body>
    <h1>Hello, Web</h1>

    <p>This is a simple web page.</p>

    <p class="red-text">This is red.</p>

    <p id="data-dst">&nbsp;</p>

    <div id="the-box">
      <p>In the box p1</p>
      <p>In the box p2
    </div>
  </body>
</html>
```

style.css:

```
h1 { text-decoration: underline }

.red-text { color: red }

#the-box {
    border: thin solid green;
    width: 40%;
    padding: 1rem;
}
```

## Now with JavaScript

 - page.html
 - code.js

## Extra Stuff

 - Columns with Floats
 - Columns with Flex

