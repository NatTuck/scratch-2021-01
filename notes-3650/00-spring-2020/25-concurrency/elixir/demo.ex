
# Promises
#  Demo.mapFib   vs.    Demo.pmapFib

# Messages
#  {:ok, pid} = Demo.startPrinter
#  send(pid, "Hello")
#  Demo.startSender(pid)

defmodule Demo do
  def fib(x) when x < 2 do
    1
  end

  def fib(x) do
    fib(x - 1) + fib(x - 2)
  end

  def mapFib() do
    {t, v} = :timer.tc(&Enum.map/2, [[38, 38, 38, 38], &Demo.fib/1])
    IO.puts "Time: #{t/1000000.0}s"
    v
  end

  def pmap(xs, op) do
    Enum.map(xs, fn x -> Task.async(fn -> op.(x) end) end)
    |> Enum.map(&Task.await/1)
  end

  def pmapFib() do
    {t, v} = :timer.tc(&Demo.pmap/2, [[38, 38, 38, 38], &Demo.fib/1])
    IO.puts "Time: #{t/1000000.0}s"
    v
  end

  def printer() do
    receive do
      {:say, msg} -> IO.puts msg
      other       -> IO.puts "Huh? #{other}"
    end
    printer()
  end

  def startPrinter() do
    Task.start(&printer/0)
  end

  def sender(pid, x) do
    send(pid, {:say, x})
    Process.sleep(1000)
    sender(pid, x + 1)
  end

  def startSender(pid) do
    Task.start(Demo, :sender, [pid, 0])
  end
end
