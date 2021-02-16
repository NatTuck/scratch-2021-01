defmodule Stack0 do
  # Client Interface

  def start_link(_arg) do
    state0 = [3]
    spawn(__MODULE__, :init, [state0])
  end

  def push(pid, item) do
    send(pid, {:push, item, self()})
    receive do
      {:ok, :push} -> :ok
    end
  end

  def pop(pid) do
    send(pid, {:pop, self()})
    receive do
      {:ok, :pop, num} -> {:ok, num}
    end
  end

  def print(pid) do
    send(pid, {:get, self()})
    receive do
      {:ok, :get, st} -> IO.inspect(st)
    end
    :ok
  end

  # "Server" Process Callbacks

  def init(state) do
    Process.send_after(self(), :next_odd, 5_000)
    loop(state)
  end

  def loop(state) do
    receive do
      {:push, item, cpid} ->
        send cpid, {:ok, :push}
        loop([item | state])
      {:pop, cpid} ->
        send cpid, {:ok, :pop, hd(state)}
        loop(tl(state))
      {:get, cpid} ->
        send cpid, {:ok, :get, state}
        loop(state)
      :next_odd ->
        Process.send_after(self(), :next_odd, 5_000)
        [head | tail] = state
        if rem(head, 2) == 0 do
          loop(state)
        else
          loop([head + 2 | tail])
        end
    end
  end
end

