---
layout: default
---

## First: Project Questions?

Presentations:

 - Start after Thanksgiving next week
 - Random teams will be selected to present each day
 - There will be a peer-evaluation component.
 - You need to show up both days.

Remember to do your TRACE evals. 

# Today: Distributed Elixir

 - The Erlang VM was designed primarily for building reliable apps, even in the
   face of hardware failure.
 - That means redundant hardware, and means that the application state needs to
   be stored replicated across those machines.
 - So Erlang has a built in mechanism to form a distributed cluster of multiple
   Erlang VMs, possibly distributed across multiple machines, and allows message
   passing between processes within the cluster.

Simplest example, in two terminal windows:

```
   # Window 1
   $ iex --sname foo
   iex> node()
   
   # Window 2
   $ iex --sname bar
   iex> Node.ping(:foo@greyarea)
   iex> Node.list()
   
   # Window 1
   iex> Node.list()
   iex> hello = fn -> IO.puts("Hello") end
   iex> Node.spawn(:bar@greyarea, hello)   # Remembers source node for I/O.
   iex> Node.spawn(:bar@greyarea, fn -> IO.puts(node()) end) # Really on other node.
```

### Setup

Make sure we have installed: moreutils konqueror

## Let's build a replicated Key-Value Store

```
$ git clone https://github.com/NatTuck/kv_store
$ git checkout 1-map-server
```

 * This is an Elixir app generated with "mix new ... --sup"
 * Show lib/kv\_store/map\_server.ex
 * Show the server getting started in applicaiton.ex

Demo:

```
  $ iex -S mix
  iex> alias KvStore.MapServer
  iex> MapServer.put(:a, 5)
  iex> MapServer.get(:a)
```

### Allow map updates from other nodes.

Add additional interface in map\_server.ex that include
the name of the target node.

You can generally replace a process PID or registered name with a tuple of
{pid/name, node-name} for any sort of sent message in elixir.

```
  def put(node, k, v) do
    GenServer.call({__MODULE__, node}, {:put, k, v})
  end

  def get(node, k) do
    GenServer.call({__MODULE__, node}, {:get, k})
  end
```

Two terminal windows again:

```
  # Window 1:
  $ iex --sname alice -S mix
  
  # Window 2:
  $ iex --sname bob -S mix
  iex> Node.ping(:alice@greyarea)
  iex> KvStore.MapServer.put(:alice@greyarea, :a, 5)
  
  # Window 1:
  iex> KvStore.MapServer.get(:a, 5)
  iex> KvStore.MapServer.put(:a, 7)
  
  # Window 2:
  iex> KvStore.MapServer.get(:alice@greyarea, :a)
```

## Let's automate cluster startup.

```
git checkout 3-cluster
```

Show new dep, libcluster, in mix.exs

Run ```mix deps.get```

Add libcluster supervisor to application.ex

Show new startup logic in application.ex, especially the list
of hosts in the cluster.

Show start script: local-start.sh

Run this by sourcing it in bash so we keep job #'s.

```
 $ bash local-start.sh
 
 # In new node0 tab:
 iex> Node.self()
 iex> Node.list()
```

## Let's handle cluster reads / writes

We'll just go with the simplest model:

 * Writes go to every node.
 * That means that reads can be from any node.
 * If it can be any node, we'll just always use the local node.

In lib/kv_store.ex:

```
defmodule KvStore do
  def get(k) do
    # Need one node, so grab the local copy.
    KvStore.MapServer.get(k)
  end

  def put(k, v) do
    nodes = [Node.self() | Node.list()]
    results = Enum.map nodes, fn node ->
      KvStore.MapServer.put(node, k, v)
    end
    if Enum.all?(results, &(&1 == :ok)) do
      :ok
    else
      {:error, "didn't write to all replicas"}
    end
  end
end
```

Let's try it out:

```
 $ bash local-start.sh

node0> KvStore.put(:a, 5)

node1> KvStore.get(:a)
```


## Now let's do a real cluster

```
$ git checkout 6-real-cluster
```

 * On the same physical server that hosts Inkfish, I've got 5 virtual machines
   running, named bot00 through bot04.
 * If this were production, we'd run the nodes on each server as services with
   systemd, but we're just experimenting, so we'll replicate the local start
   script.
 * Show ssh-config
 * Show remote-start.sh
 * Ssh into bot00 and bot01 to demonstrate that they really exist.
 * Show push.sh
 * Erlang cookie: This makes sure that each node is authorized to join
   a cluster.
 * Inter-node erlang isn't encrypted - it's really a local network thing.
 * You *can* run distributed Erlang across the internet.
   * Consider alternatives, like having an HTTP API
   * If you decide to just link erlang nodes, use a VPN.

 * Show lib/kv_store/application.ex

```
$ ./push.sh
$ ./remote-start.sh

kv@node00> KvStore.put(:alice, "alice@example.com")
kv@node01> KvStore.get(:alice)
```

Problem: What happens if a node dies and restarts?

 * We only do distributed writes once.
 * The new node won't have old data, and so reads will be incorrect.

Solution: Nodes sync with the rest of the pool on startup.

 * Show lib/kv\_store/map\_server.ex: init funcition
 * multi_call does calls to a collection of nodes, properly handling
   errors and timeouts (by skipping those replies)

Let's try that:

```
kv@node02> KvStore.put(:bob, "bob@example.com")
kv@node01> KvStore.get(:alice)
kv@node03> KvStore.get(:bob)

(kill kv@node02 manually)

kv@node04> Kv.store.put(:carol, "carol@example.com")

(start kv@node02 manually)

kv@node02> KvStore.get(:alice)
kv@node02> KvStore.get(:bob)
kv@node02> KvStore.get(:carol)
```

 * And with that, we've built a distributed Key-Value store.

## Possible Enhancements:

 * Add configurable m-of-n for reads and writes.
 * Add version numbers, in case things somehow get out of sync.
   * Well, we actually probably want vector clocks.
 * Consider eventual consistency instead of CAP consistency.

## Conclusion

 - Elixir / Erlang / OTP gives us the essential baseline to build our
   applications as distributed systems.
 - Message passing is one of the options when you're building standalone
   concurrent programs. On a distributed system, it's mandatory. Erlang has that
   buit in with basic stuff like timeouts and stuff like multi_call built in.
 - GenServers, Agents, Tasks, etc are solid building blocks for managing state
   and concurrency that scale nicely into multiple nodes.
 - Setting up a cluster can be managed by pulling in a library.
 - You still need to actually design and build your system. There are design
   questions, like CAP consistency vs CAP availability, that your tools can't
   make for you. Your tools can't even hide those questions.
 - But the Erlang platform provides a good basis for building systems where you
   make those choices, without needing to build the baseline functionality
   shared by all distributed systems.






