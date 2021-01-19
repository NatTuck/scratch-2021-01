defmodule Tick do
  def start() do
    state0 = %{msg: "hello"}
    spawn(__MODULE__, :init, [state0])
  end

  def init(state) do
    schedule_tick()
    loop(state)
  end

  def schedule_tick() do
    Process.send_after(self(), :tick, 2000)
  end

  def loop(state) do
    receive do
      :tick ->
        IO.puts "tick: #{state.msg}"
        schedule_tick()
        loop(state)
      {:set_msg, msg} ->
        loop(%{ state | msg: msg })
    end
  end

  def set_message(pid, msg) do
    send(pid, {:set_msg, msg})
  end
end
