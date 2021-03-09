defmodule Stack2 do
  use GenServer

  def start_registry do
    Registry.start_link(keys: :unique, name: Stack2.Registry)
  end

  def reg(id) do
    {:via, Registry, {Stack2.Registry, id}}
  end

  # Client

  def start_link(id) do
    GenServer.start_link(__MODULE__, [3], name: reg(id))
  end

  def push(id, item) do
    GenServer.cast(reg(id), {:push, item})
  end

  def pop(id) do
    GenServer.call(reg(id), :pop)
  end

  def print(id) do
    state = GenServer.call(reg(id), :get)
    IO.inspect(state)
    :ok
  end

  # Server (callbacks)

  #@impl true
  def init(state) do
    Process.send_after(self(), :next_odd, 5_000)
    {:ok, state}
  end

  def handle_info(:next_odd, [head | tail]) do
    Process.send_after(self(), :next_odd, 5_000)
    if rem(head, 2) == 0 do
      {:noreply, [head|tail]}
    else
      {:noreply, [head + 2 | tail]}
    end
  end

  def handle_call(:pop, _from, [h | t]) do
    {:reply, h, t}
  end

  def handle_call(:get, _from, state) do
    {:reply, state, state}
  end

  def handle_cast({:push, item}, state) do
    {:noreply, [item | state]}
  end
end

