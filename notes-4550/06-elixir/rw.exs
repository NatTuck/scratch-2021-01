defmodule RW do
  def loop() do
    case IO.gets("input: ") do
      :eof ->
        IO.puts "All done"
      {:error, reason} ->
        IO.puts "Error: #{reason}"
      line ->
        IO.puts(reverse_words(line))
        loop()
    end
  end

  #def reverse_words(line) do
  #  words  = String.split(line)
  #  rwords = Enum.map(words, &reverse_chars/1)
  #  Enum.join(" ", rwords)
  #end

  def reverse_words(line) do
    line
    |> String.split()
    |> Enum.map(&reverse_chars/1)
    |> Enum.join(" ")
  end

  def reverse_chars(word) do
    word
    |> String.to_charlist
    |> Enum.reverse
    |> to_string
  end
end

RW.loop()
