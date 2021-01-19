defmodule Stack1 do
  use GenServer

  # Client

  def start_link(default) do
    GenServer.start_link(__MODULE__, default)
  end

  def push(pid, item) do
    GenServer.cast(pid, {:push, item})
  end

  def pop(pid) do
    GenServer.call(pid, :pop)
  end

  def print(pid) do
    state = GenServer.call(pid, :get)
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

