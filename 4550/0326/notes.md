
# First: HW09 Questions

When you're using JS display libraries in a
React app, you want to use React-specific 
libraries. A datepicker should be a react
datepicker.

Library for password dictionary check: good idea

How to handle storing copies of DB state in the 
browser?

 - At minimum, your redux store needs copies of
   any data that's needed to render the current
   view.
 - Any other data is a tradeoff between performance
   and being up-to-date.
 - You need to occasionally request new data,
   and requesting data on page view is OK. A
   refresh button is an acceptable plan.

# Today: Automated tests

Automated tests provide huge value in two
specific cases:

 - When you're adding functionality, tests make
   sure that you implemented it correctly.
 - When you're making changes, tests help avoid
   having new functionality break old functionality.

Tests are generally written as code in a separate
file from the app logic.

For the setup we're doing in this class, we need two
separate sets of tests:

 - Elixir tests to test Elixir code.
 - JavaScript tests to test JavaScript code.

# Different kinds of tests for a server-side app

 * Simple unit tests
 * Context module unit tests
   - DB access is side effects

## Context module unit tests

Phoenix / Ecto gives us:

 - Dedicated DB to run tests in
 - Tests, by default, run in a DB transaction
   and are rolled back after running

How do we end up with the initial state of the
DB before running a test?

Plan A: Just use a single seeds file.

 - This is pretty simple.
 - Downside: This must be sufficient for every
   test we plan to run.

Plan B: Seeds + Fixtures

 - Some groups of tests have extended seeds.

Plan C: Factories

 - A factory is a function that will preload
   the DB with generated data on request

## Photo Blog Context Tests

 - Seeds + Fixtures

## Controller Tests

 - This lets us test controller actions.

Why write context tests when controller tests
exercise the same code?
 
 - I dunno, don't maybe?
 - One of the major goals here is code coverage,
   to make sure that every line of code gets run
   in tests at least once so at least it doesn't
   have a trivial logic error.

If a controller test doesn't easily cover the
context module function, or not all the control
flow paths, then a context module test is useful.
 
## Integration Tests

We try to test a bunch of pieces of the system
at the same time.

 - Maybe several HTTP requests in sequence.
 - Maybe both Elixir and JS code.

This is important, because we need to make sure
that the app works together.

This is hard for a variety of reasons:

 - Can't do these in a DB transaction.
 - Can't take shortcuts in execution anymore,
   may need to really make network requests.
 - If we want to run JS, we'll need to run
   a JS engine. That may mean a full browser.

#### Simulated Integration Tests

These are like multi-step controller tests.

 - No browser, no JS
 - But we can simulate actions like clicking
   links on webpages to trigger multiple
   controller action calls.

Library: phoenix_integration 

#### Full Integration Testing

 - Pop up a browser
 - Run a real HTTP server
 - Remote control the browser to access the
   real HTTP server

Elixir library: Hound

Key moving part: Selenium

## Testing JavaScript

Write test functions.

 - Unit tests to test individual functions.
 - Pure functions are easy. 
 - React, Redux make us write a bunch of
   pure functions.





 
 
 

