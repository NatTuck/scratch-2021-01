
## Upcoming Dates

 - Today: Distributed Elixir
 - Tomorrow: Projects Due
   - Watch for Piazza post on sharing your videos.
 - Next Tuesday: Semester Wrap-Up Lecture
 - Next Friday: Project Q&A Part 1
 - Final Tuesday: Project Q&A Part 2

## Project Questions


## Distributed Elixir

 - Erlang VM is for reliability.
 - That means hardware redundancy.
   - Specifically, to enable state replication.
 - Erlang has a built-in mechanism to build distributed
   clusters of multple Erlang VMs, posisbly across
   multiple machines, and passing messages between
   processes on separate machines.

## Replication in Distributed Systems

CAP Theorem

We'd like three properties:

### Consistency

A request to the system always gives the latest answer.

Ex. Our application maps names to phone numbers.

Our result is consistent if, after bob updates his
phone number, nobody get query our system and get
his old phone number.
  
Example solution:

 - Bob isn't done updating his phone number until
   all nodes have confirmed the change.
 - Therefore, after Bob gets his confirmation,
   there is no node that Alice could query for the
   old value.

### Availability

A write request to the system will always return a
successful response in finite time.

### Partition Tolerance

The system still works even if some nodes can't
communicate with other nodes.


### The CAP Theorem

If you want partition tolerance, you can't have
both consistency and avaiability. 








