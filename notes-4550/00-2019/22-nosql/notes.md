+++
title = "./22-nosql/notes.md"
+++

# First: Project Questions?

Presentations:

 - Both classes next week
 - Random teams will be selected to present each day
 - There will be a peer-evaluation component.
 - You need to show up both days.

# Today: Databases

## The Problem: Persistent Data Storage

Over the semester, we worried about a several different kinds of state:

 - Server-side, transient state: GenServer
 - Server-side, persistent state: Postgres DB
 - Client-side, transient state: React Component's state field, Redux
 - Client-side, persistent state: window.cookie, HTML5 Local Storage

A Postgres database isn't the only way to deal with server-side, persistent
state. Let's look at the problem in some more detail.

## Plan A: Files on the Filesystem

 - The normal way to store long-term persitent data on a computer
   is by saving a file.
 - Files are a reasonably efficient method of mapping heirarchical string
   keys to arbitrarily large chunks of data.
 
Problems:

 - Opening files is a bit slow - it takes syscalls and needs to maintain
   POSIX semantics.
 - Files have a non-trivial overhead: e.g., 4k block size
 - Concurrency isn't handled by default = data races.
 - Solving these two things results in building a key-value store.
 - Scales poorly beyond one machine. Network filesystems are a huge pile
   of bad compromises.
   
**Walk Through Solving Husky Shop**

Husky Shop Tables:

 * products
 * users
 * cart\_items ("carts"?)
 * ratings
 * suggestions

## Plan B: Key-Value Store

This is a library or service that stores values associated with keys.

Minimally, has two operations: get and put

Advantages:

 - Can optimize actions without conforming to the requirements for file
   operations.
 - Can enforce atomic get and put, but also atomic read-modify-write
   (e.g. with per-key locks).
 - Can be made to scale easily with key-based sharding.
 - Can be made redundant with m-of-n replicas.
 - CRUD operations map well to REST.

Disadvantages:

 - Multi-key operations usually aren't atomic (= data races).
 - Efficient and powerful queries can be difficult, especially
   joining data between separate records.
 - Can't get all the guarantees you really want for reliability,
   especially data consistency across multiple resources.

**Walk Through Solving Husky Shop**

## Plan C: Relational Database

A Relational Database Management System, or SQL Database, splits data up into a
set of tables, specifies a query language (SQL), and provides "transactions",
which offer a set of guaranteed reliability properties.

Examples:

 - PostgreSQL
 - MySQL / MariaDB
 - Oracle
 - Microsoft SQL Server
 - SQLite

ACID:

 - Atomic: All transactions - operations that mutate data in the database - are
   atomic, even if several records in different tables are effected.
 - Consistent: All queries see the data in a consistent state - each transaction
   has either happened or not. Transactions appear to be applied in some
   globally consistent sequential order.
 - Isolation: Queries don't see the result of transaction until those
   transactions are guaranteed to have successfully completed.
 - Durability: Once an transacton is successfully completed, even power loss
   won't unexpectedly undo it.

Advantages:

 - We want the ACID properties. They're really nice, especially if we care about
   our data at all.
 - SQL DBs validate schemas, including foreign key constraints. They protect us from
   ending up with an inconsisant (corrupt) database.
 - Foreign key references make join queries possible.
 - Transactions apply to mutation of multiple tables.

Disadvantages:

 - Scaling to multiple machines is hard. SQL databases largely can't handle
   write operations on multiple DB servers.
 - Providing a bunch of guarantees costs performance.
 - Schemas are mandatory and changing them is annoying.
 - Is innately pretty complicated.

**Scaling SQL Databases:**

 - First, buy a bigger server. They scale nicely with multiple cores.
 - Next, add read-only replicas that handle read requests. This can scale pretty far.
 - After that, you're stuck with either:
   - Cluster solutions, which are all annoying (e.g. require all data in RAM).
   - Manual sharding across multiple servers which loses you some transaction
     atomicity.
 - Update: I'll talk about CockroachDB more after the NoSQL stuff.

## NoSQL

NoSQL is the name for the broad idea that getting rid of features from SQL DBs
results in a useful datastore.

Generally these systems don't use SQL as the query language, and they don't
provide the ACID data reliability guarantees. 

### Example 1: Memcached

 - An in-memory string key to blob value data store.
 - For caching.
 - Can do key-based sharding to scale perfectly.
 - Doesn't care about losing data, it's primarily a cache.
 - Users: This makes a good chunk of the web go.

### Example 2: Redis

 - String key to structured value (e.g. list, hash table, score-based ordered set, etc)
 - Clusters by simple partitioning.
 - Operations that make sense on values (e.g. list prepend) are very efficient.
 - Data can be persisted to disk occaionally, but without hard guarantees.
 - Users: Commonly used for stuff like queues & small caches.

### Example 3: MongoDB

 - JSON Doucument Store
 - Optional schemas, which enable limited join queries.
 - Schema-based indexes.
 - Eventually synced to disk.
 - Bad reputation:
  - Hyped as an SQL replacement, isn't one.
  - Known for losing data for a variety of reasons, including a
    situation where the same query could see two differerent versions 
    of an object.
 - Popular, so might have gotten less bad by simple overinvestment,
   same as PHP.
 - Users: Ebay (media metadata)

Scaling:

 - Sharding by key
 - Replicas: Default majority read & write

Draw picture with 6 notes showing replicas (n=3) and sharding.

### CAP Theorem

 - Consistency: All reads complete in finite time and always return the latest value.
 - Availability: All writes complete in finite time.
 - Partition tolerance: The system can handle having the nodes partitioned into
   two groups that can't communicate for an unbounded amount of time.
   
You can't have both Consistency and Availablility in the same system.

Proof: Consider a data store with two nodes (A and B) that currently can't
communicate due to a netsplit. We do a write to node A and then read the same
key from node B.

The system must, before the netsplit is resolved:

 - Accept the write to A in order to maintain availablility.
 - Respond to B with the value written to A in order to maintain consistency.

This means that real-world distributed storage systems can be classfied
into one of two categories:

 - An AP system discards consistency: reads may return old values. This
   will only be resolved when any network partitions are fixed.
 - A CP system discards availability: writes may block until all replica
   nodes are available.
 - Third options: Writes always work, but reads may block.

When a system allows you to tune read and write quorms, you're trading off
immediate reads, immediate writes, and reads getting old data.

### Example 4: CouchDB
 
 - JSON Document Store
 - No transaction guarantees
 - Eventual Consistency by Versioning 
 - Nodes can write without permission, so scales very well.
 - Eventually synced to disk.
 - Map-Reduce Indexes
 - User: This is where NPM package metadata is stored.

Offline support:

 - You can put a CouchDB node on a client (e.g. a phone). This allows
   offline app usage.
 - You can put a PouchDB node on a user's browser. This does the same
   thing.
 - Of course, a local node would only contain documents for that user.

### Example 5: Riak KV

 - String key to BLOB value
 - This scales OK for writes and great for reads, but loses availability in case
   of a network partition.
 - Eventually synced to disk.
 - Map-Reduce Indexes
 - User: Uber (car dispatch)

Scaling:

 - Key-based sharding.
 - Eventually consistent by default: Any replica can accept a write. This means
   that queries can return multiple conflicting values and the reader is
   expected to resolve conflicts.
 - Can also be configured for majority-to-write or consensus-to-write and
   last-write-wins, which makes it act more like a CP system.

CRDTs:

 - Eventual consistency is really nice - it lets an app keep working even when
   a netsplit occurs.
 - Having to manually resolve conflicts is annoying.
 - Simple resolution heuristics can cause issues. "Keep latest timestamped version"
   will lose data.
 - Another solution is to use CRDTs or "conflict-free replicated data types".
 - Riak has some of these built-in for automatic conflict resolution on selected
   data fields.

Example: Counter

 - Simplest CRDT is (an integer) with two operations (inc-by and dec-by).
 - The merge operation is to simply add the numbers together.
 - Because addition is commutative, merging never loses data.
 - Show example with Youtube views

Example: Set

 - Consider a rental car store with an inventory of rental cars.
 - Core question: Which cars are available to rent.
 - Operations: Add a car to the set, remove a car from the set.

Single-use Set CRDT:

 {
   adds: // List of items added
   dels: // List of items removed
 }

 - To add or delete an item, add it to the appropriate list.
 - Duplicate adds or deletes are errors.
 - To merge, take the union of the adds and dels sets.

Using this for the car example:

 - Problem: What if a car is rented and then returned?
 - Solution: Treat it as a new object with a new unique key.
 - Problem: Since this only grows, won't we run out of space?
 - Solution: Assign each store a new map every day, adding the
   leftover inventory from the previous day as the starting value.

### Example 6: Cassandra

 - Cassandra is a lot like a SQL database without ACID transactions, so that it
   can do replication and sharding instead.
 - Query language is almost SQL
 - Column store: Table columns are stored together in memory; efficient for
   queries that don't access all columns.
 - Cassandra guarantees durability.
 - Replicas: Default majority read & write, configurable
 - Users: Netflix (shows)

## Other Options

### CockrochDB: A distributed SQL database?

 - Claims to be a SQL database with horizontal scaling, even for writes.
 - Appears to use sharding and replicas.
 - Writes require a majority of replicas to confirm (so this is consistency over
   availability).
 - Doesn't support all SQL queries that a monolithic server could handle.
 - Can be slow.
 - Will give poor performance repeatedly writing / updating the same row.

# Persistent Browser State

## WebStorage

 - window.localStorage - Anything you put in this object sticks around "forever".
 - window.sessionStorage - Sticks around until browser is closed.
 - indexedDB - provides better lookup behavior for complex data

## Service Worker

 - Persistent code in user's browser
 - Combined with webstorage allows for offline web apps
 - Be careful: If you want to change one after it's deployed, it may take several
   full browser restarts to get the new version unless you have explicit upgrade
   logic to handle this.

## PouchDB + CouchDB

 - clone husky\_shop\_spa, branch 1-deps
 - write a quick React TODO app
 - demo putting the data in window.localStorage
 - pull in PouchDB dep
 
```
npm install pouchdb
var PouchDB = require('pouchdb');
var db = new PouchDB('kittens');

var doc = {
  "_id": "mittens",
  "name": "Mittens",
  "occupation": "kitten",
  "age": 3,
  "hobbies": [
    "playing with balls of yarn",
    "chasing laser pointers",
    "looking cute"
  ]
};
db.put(doc);

db.get('mittens').then(function (doc) {
  console.log(doc);
});
```

