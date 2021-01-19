+++
title = "./23-dist-elixir/notes.md"
+++

## First: Project Questions?

Presentations:

 - Both classes next week
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

Add libcluster dep to mix.exs:

```
  defp deps do
    [
      {:libcluster, "~> 3.0"},
```

Run ```mix deps.get```

Add libcluster supervisor to application.ex

```
  def start(_type, _args) do
    topologies = [
      local: [
        strategy: Cluster.Strategy.Epmd,
        config: [
          hosts: [:node0@localhost, :node1@localhost, :node2@localhost]
        ],
      ],
    ]
    
    children = [
      {Cluster.Supervisor, [topologies, [name: KvStore.ClusterSupervisor]]},
```

Create a start script, local-start.sh

```
#!/bin/bash

export WD=`pwd`

cat > /tmp/tabs.$$ <<"EOF"
title: node0;; workdir: $WD;; command: iex --sname node0@localhost -S mix
title: node1;; workdir: $WD;; command: iex --sname node1@localhost -S mix
title: node2;; workdir: $WD;; command: iex --sname node2@localhost -S mix
EOF

konsole --hold --tabs-from-file /tmp/tabs.$$
```

Run this by sourcing it in bash so we keep job #'s.

```
 $ bash local-start.sh
 
 # In new node0 tab:
 iex> Node.self()
 iex> Node.list()
```



