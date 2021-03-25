---
layout: default
---

## First Thing

Project questions?

# Testing Web Applications

Automated testing is an important tool for making sure that your app does what
you want it to do.

Generally, automated tests provide value in two cases:

 - When you're adding functionality to your app, tests can help make sure that
   functionality is implemented correctly.
 - When you're making changes, tests can help avoid breaking existing
   functionality.

That second part is the really important thing. Especially when you're working
on a team, tests are a way to make sure that when you write a feature that
feature surives through other people modifying the code.

Tests are generally written as functions in separate files from you main
application code that cause your main code to be run and check that it behaved
as expected.

For our Elixir/Phoenix + JavaScript setup, we need to worry about two separate
sets of tests: One for our Elixir code, another for our JavaScript code. We also
want to make sure our Elixir and JS code are tested together.

## Testing Elixir Code



### Simple Unit Tests

It's always useful to be able to test individual functions.

Elixir makes that easier in general because it's a functional language and so
most functions are pure: to test them you just need to verify that the output is
as expected for a selection of inputs.



### Context Module Unit Tests

In our Phoenix apps, we generally manipulate the database using functions in
context modules. We'd like to be able to unit tests these functions.

Conceptually this is slightly complicated because we don't really want to modify
our database every time we run our tests.

Luckily, the test setup Phoenix gives us avoids most of that problem, as
follows:

 * We have a dedicated test database to run tests in.
 * Tests, by default, run inside DB transactions. After the test has completed,
   the transaction is rolled back, resulting in no persistent change to the DB.
 * This allows multiple tests to run concurrently - since transactions are
   isolated, different tests won't see each other's changes.

There's one more complicaton: Fixtures

Some functionality can be tested in isolation. To test inserting a User, you can
construct a User, insert it, and then verify that it was inserted correctly.

But other functionality requires existing data. To update a User, there first
must be a User in the DB. 

A common approach is to seed the database with a bunch of test data. This works
well, but as your database structure gets more complicated you can end up
needing to maintain a *lot* of test data. 

If one test requires a user who is an admin, registered in December, and made
exactly three submissions to two courses, that user must appear in the seed
data. Worse, as the tests change it's hard to keep track of which tests depend
on that particular set of records.

An alternative is to programatically generate records that are needed for tests
using a "factory" module. Inkfish uses a combination of these techniques, with
some pre-seeded data and the rest created using a factory module.

 * Show apps/inkfish/test/support/factory.ex

Let's look specifically at "assignments":

 - We construct an assignment object with default fields. These
   can be overridden by the consumer.
 - Assignments belong to buckets and teamsets, which both belong to the
   same course, so we need to explicitly construct this subgraph.

Another example is "team_member":

 - This constructs a new team and reg.
 - This implies a structure
   - team -> teamset -> course
   - reg -> course
 - Since the code doesn't explicitly specify, this probably ends up being
   two seperate newly created courses. Hopefully that doesn't break any
   tests that construct a team_member.

Now that we've seen how we're going to build test data, let's look at a context
module. Specifically, let's look at .../assignments.ex:

 - We've got some stuff in here with complex logic, let's skip it.
 - Moving down to "update\_assignment", this is the default scaffolding
   code from phx.gen.html.
   
Looking at .../assignments\_test.exs:

 - assigment_fixture calls our factory helper to insert an arbitrary
   assignment into the DB
 - Then we generate a params map with exactly one changed attribute
 - Then we call the update function we're testing
 - Then we verify that the values returned by the update function are correct.

This assumes that the update function correctly returns the new value. If we're
a little less trusting, we might want to refetch the item and check it.

### Controller Tests

Unit tests verify that single functions are correct, but we also want to make
sure that larger pieces of our application work as expected.

With Phoenix apps, the externally visible interface we're exposing is the
ability to make HTTP requests to paths in our application.

Controller tests test our application at this scale. We simulate an incoming
request, causing our pipeline plugs and controller action to be called, and then
verify that the response is correct.

Controller actions may modify the database, so these tests are again performed
in a transaction so the changes can be reverted when they're done.

Let's look at .../assignment_controller.ex:

 - One action: show
 - This is for normal users, that's the only available action.
 - To show a user an assignment, we first need to get:
   - The assignment
   - The user's submissions
   - The user's team
 - Then we can render the template.

To test this, let's look at .../assignment\_controller_test.ex:

 - First we create an assignment.
 - Then we login as a user (by faking a sesison)
 - Then we GET the assignment's path.
 - This tests the router, plugs, controller, referenced contexts, and templates.

Do we need context module unit tests if we have controller tests?

 - It depends.
 - Controller tests can potentially test the paths through the code
   that can currently be exercised.
 - Context unit tests can test the internal interface, possibly including
   code that isn't yet accessible.
 - Context unit tests can be easier to debug, and allow us to quickly test
   more cases for things like changeset validations.
 - If you need to pick only one, I'd go with controller tests. That's what's
   going on in a bunch of cases with Inkfish.

### Integration Tests

 - Controller tests test a single path, but that's not how users will actually
   use your application. They'll use several paths in a sequence, each one of
   which may change the application state in a way that the next path depends
   on.
 - This can be tested with integration tests.

With Elixir + Phoenix, we have two approaches for integration tests. Depending
on our application, we may want to use either or both.

#### Simulated Integration Tests

 - These are basically the same as controller tests, except:
 - They allow us to simulate user actions on the rendered pages, such as
   clicking links.
 - They maintain state for several steps.

Properties:

 - Similarly fast to controller tests.
 - No need for external tools.
 - Everything is still being simulated inside the Erlang VM. There's still no
   real web server or browser involved.
 - It won't execute JavaScript. That restricts it to standard page loading, link
   following, and form submissions.

Phoenix apps don't come with this test type built in, but it's provided by the
"phoenix_integration" library.

Example in .../integration/request\_reg\_test.exs:

 - Bob is the instructor; Gail isn't registered. 
 - First we log in as gail and make the request.
 - Then we log in as bob and approve it.
 - Then we log in as gail to confirm we have access to the course.

 - We provide test instructions as if we were users interacting with the site,
   using functions like "follow link". 
 - We're able to test an entire workflow, make sure we go through the right
   sequence of pages, and confirm that changes to the DB have the expected effect. 

#### Full Integration tests

Testing our Elixir code is great, but at a certain point we want to be able to
verify that:

 - The site works in a real browser.
 - We can do workflows that involve JavaScript code.

Again, this isn't provided by default in Phoenix apps. But we have two choices
for libraries to do this kind of testing:

 - wallaby
 - hound

Wallaby is focused on fast testing using a windowless browser called phantomjs.
This is a good idea. It also has a more "batteries-included" API than Hound.

We're using Hound. It exposes the flavor of Selenium a bit more directly, and I
found documentation to get it working with Firefox faster.

Before we can run this kind of test, we need to set up the mechanism that
actually automates the browser. A common tool for that is Selenium.

Let's take a look at .../test/selenium.sh:

 - This script downloads two files.
 - Selenium-server is the program that handles automating a browser.
 - Gecko-Driver is the connector that hooks it specifically to Firefox.
 - Selenium-server needs to be running when we run our Hound tests.

Let's take a look at .../integration/submit_test.exs:

 - This test logs in and submits a Git repo to an assignment.
 - It does this by popping up a Firefox window.
 - We need a JS-executing browser here, because the mechanism for creating
   a new upload and associating it with a submission is handled in JS.

Looking through the test code, there's a bunch going on:

 - Ecto's testing sandbox allows tests to run concurrently by tying database
   handles to a specific Erlang process. This doesn't work here - the process
   that clones the git repo and creates the upload runs seperately. This means
   that in order to maintain the transaction behavior, this test can't run
   concurrently with any other test.
 - Another option would be to ditch the sandbox, which might let us do tests
   concurrently, but would mean we'd actually be mutating the DB during the test run.
 - We need to start and then end the Hound session - that's what pops up the
   Firefox window.
 - Hound provides a somewhat minimal, procedural API, so we need some helper
   functions for this test.
 - Specifically, we want to be able to perform actions - especially initiating
   the git clone - and then wait for them to complete. We do this by polling the
   page text to see if we have what we want yet.
 - With all that set up, the test itself looks a lot like the previous
   integration test - we navigate through the site like a user until we get to
   the point where we've demonstrated that the workflow seems to work.

## Testing JavaScript code

For this section:

 * App is lens: https://github.com/NatTuck/lens
 * Branch: spa5-jest-tests

With selenium popping up Firefox instances we can test all of our code,
including our JavaScript. But that's expensive both in effort and test runtime,
so we'd like to be able to test our JS code directly.

That means pulling in a JavaScript testing library.

I'm going to show examples using Jest, which seems to be reasonably popular.

Steps to get testing set up:

 - Install Jest and whatever testing libraries (with npm --save-dev).
 - Create a test command in our package.json (show package.json).

Then we can run our JS tests with:

```
(from our assets directory)
$ npm test
```

We've written a lot of our app in a functional style, which makes unit tests for
those parts of our app pretty easy. Our tests can just call functions and verify
that they return reasonable results.

This applies to:

 * Stateless React components.
 * Redux reducers.
 * Pure helper functions.

### Example: Testing a React Component

Take a look at:

 * .../js/photos/card.jsx
 * .../js/photos/card.test.jsx

For a simple test, we can check:

 * Does this function run without errors?
 * Is the description for a photo we pass in really included in the rendered
   output?
 * This case is covered in 'photo card shows desc'

For testing react components specifically, the Jest devs made an interesting
observation: In many cases we don't want to hardcode result we're testing for.

 * Components change over time
 * What they're rendering naturally changes too.
 * Testing for a specific output just means needing to change things in two
   places.
 * It's useful to simply catch the case where the output changes when we didn't
   expect it to.
   
To handle this, Jest has "snapshot tests". You simply render a component and
then test that it matches a snapshot. On the first, run, it'll generate output
and you can double check that the output make sense. Subsequent runs will fail
if they produce different output. If the different output was intended, you can
just regenerate the snapshot to pin the new output.

Using a snapshot is shown in 'render photo card snapshot', and the snapshot
itself is stored in .../js/photos/\_\_snapshots\_\_/card.test.jsx.snap

 * Run the test
 * Change the test to render a different desc
 * Run the test again
 * We can fix this either by deleting the old snapshot or running:
 
```
$ npm test -- --updateSnapshot
```

### Testing the Redux Store

There are a couple approaches to testing the store:

 * Unit test the reducers individually.
 * Make a store and test dispatching on it.

Testing the reducers indivdually is simpler and can be a good idea, but it's
also pretty easy to test the whole reducer chain at once.

We just need to:

 * Instantiate a store
 * Dispatch some events
 * Check the resulting states

Let's take a look at .../js/store.test.js

 * We create a new store to test with for this one test.
 * We check its initial state.
 * We add a photo.
 * We check that we have one photo.

There's one complication here. We cheated a bit in building the store, and so
the reducer isn't a pure function: it's initialized by reading from
localStorage. We're not in a browser, so there's no localStorage object.

We can handle these issues of missing global state and even side effects using a
technique called mock objects: we create our own localStorage object that always
does what we expect.

In this case we pulled in a library called 'jest-localstorage-mock' that does it
for us, adding a fake localStorage object to the global state.

One thing to keep in mind is that if our test *changes* the mocked localStorage we
need to clean it up so that other tests looking at it don't get confused later.

### Handling Side Effects with Mocks

This idea of mock objects can be extended further to test all kinds of things
that normally would have side effects.

For example, we could test our code that creates or downloads Photos by mocking
the "fetch" function. We're not going to do that now, but we can go pretty far
with this strategy.

### Testing JavaScript: Summary

Overall we want to use the same basic strategy for testing JS in a as we used for
testing Elixir:

 * Write unit tests for small pieces.
 * Write whole-page tests for testing bigger features.
 * Make sure we test all of our user actions.
 * Use Selenium to test Browser <=> Server integration on full workflows.

## Continous Integration with Travis-CI

Tests are great, but they only work if people run them.

As I mentioned before, one of the key benefits to tests is to make sure that
many developers working together don't break each other's code.

One way to enforce this is to require that all proposed changes pass the tests
before they're accepted. This can be accomplished as a simple workflow rule, but
it's made simpler if you can have the tests run automatically for git pull
requests. Then you can follow the standard process for making any change to a
git app:

 * First, make a branch for the new change.
 * Make your changes in the branch.
 * Submit a pull request for the change.
 * Have someone else merge your pull request, optimally doing some code review
   in the process.
 * If you run the tests automatically, they can not pull changes that break
   the tests.

For apps hosted on Github, a really easy tool for this is Travis-CI. They're a
commercial service, but they're free for open source projects.

 - Show the integration-tests pull request to inkfish.
 - Clearly I shouldn't merge this pull request yet.

Show the .travis.yml file for the *master* branch on Inkfish.

 * Travis runs your tests in a VM.
 * We need to select what VM: In this case it runs on Ubuntu 18.04
 * We need to specify what language and runtime version.
 * Need to install some extra deps.
 * Set up the test environment (e.g. db, here we've got an LDAP server)
 * Travis knows that the command to test an Elixir app is "mix test", but
   we could change that if we wanted.
 * There's a couple different ways to run both Elixir and JS tests.
 * Running selenium requires some extra steps. I haven't fixed that yet, which
   is why the PR is broken.

There are some alternatives to Travis:

 * Gitlab has CI built in.
 * A common self-hosted CI solution is Jenkins, but it requires more complicated
   config.
 * ... lots of others


## Resources

 - https://github.com/elixir-wallaby/wallaby
 - https://github.com/HashNuke/hound
 - https://github.com/boydm/phoenix_integration
 - https://jestjs.io/
 - https://github.com/testing-library/react-testing-library
 - https://reactjs.org/docs/test-renderer.html
 - https://travis-ci.com/





