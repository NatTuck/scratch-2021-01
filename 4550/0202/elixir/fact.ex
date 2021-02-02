defmodule Fact do
  # fact(1) = 1
  # fact(x) = x * fact(x-1)
  #
  # Accumulator:
  def fact(x) do
    fact(x, 1)
  end

  def fact(x, acc) when x <= 1 do
    acc
  end

  def fact(x, acc) do
    # Tail position
    fact(x - 1, x * acc)
  end
end
